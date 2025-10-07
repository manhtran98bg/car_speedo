#include "audio_service.h"
#include "user_config.h"
#include "driver/i2s.h"
#include "LittleFS.h"
// ==== Decoder state ====
static libhelix::MP3DecoderHelix mp3_decoder;
static libhelix::AACDecoderHelix aac_decoder;
static i2s_port_t i2s_num = I2S_PORT;
static int last_samplerate = 0;
static QueueHandle_t audio_queue = NULL;
static TaskHandle_t audio_task_handle = NULL;

// ==== Timing metrics (optional) ====
static unsigned long total_read_ms = 0;
static unsigned long total_decode_ms = 0;
static unsigned long total_play_ms = 0;

// ==== Audio frame buffer ====
static uint8_t frame_buf[MP3_MAX_FRAME_SIZE];

// ==== Callback from Helix ====
static void mp3_data_callback(MP3FrameInfo &info, int16_t *samples, size_t len, void *ref)
{
    unsigned long s = millis();
    if (last_samplerate != info.samprate)
    {
        log_i("MP3 info: %dkbps, %dch, %dHz, %dbits",
              info.bitrate, info.nChans, info.samprate, info.bitsPerSample);
        i2s_set_clk(i2s_num, info.samprate, info.bitsPerSample,
                    (info.nChans == 2) ? I2S_CHANNEL_STEREO : I2S_CHANNEL_MONO);
        last_samplerate = info.samprate;
    }

    size_t bytes_written = 0;
    i2s_write(i2s_num, samples, len * 2, &bytes_written, portMAX_DELAY);
    total_play_ms += millis() - s;
}

static void aacAudioDataCallback(AACFrameInfo &info, int16_t *pwm_buffer, size_t len, void *ref)
{
    unsigned long s = millis();
    if (last_samplerate != info.sampRateOut)
    {
        // log_i("bitRate: %d, nChans: %d, sampRateCore: %d, sampRateOut: %d, bitsPerSample: %d, outputSamps: %d, profile: %d, tnsUsed: %d, pnsUsed: %d",
        //       info.bitRate, info.nChans, info.sampRateCore, info.sampRateOut, info.bitsPerSample, info.outputSamps, info.profile, info.tnsUsed, info.pnsUsed);
        i2s_set_clk(i2s_num, info.sampRateOut /* sample_rate */, info.bitsPerSample /* bits_cfg */, (info.nChans == 2) ? I2S_CHANNEL_STEREO : I2S_CHANNEL_MONO /* channel */);
        last_samplerate = info.sampRateOut;
    }
    size_t i2s_bytes_written = 0;
    i2s_write(i2s_num, pwm_buffer, len * 2, &i2s_bytes_written, portMAX_DELAY);
    // log_i("len: %d, i2s_bytes_written: %d", len, i2s_bytes_written);
    total_play_ms  += millis() - s;
}
// ==== I2S init ====
static esp_err_t i2s_init_config(uint32_t sample_rate)
{
    i2s_config_t cfg = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
        .sample_rate = sample_rate,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
        .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
        .communication_format = I2S_COMM_FORMAT_STAND_I2S,
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
        .dma_buf_count = 8,
        .dma_buf_len = 160,
        .use_apll = false,
        .tx_desc_auto_clear = true,
        .fixed_mclk = 0,
        .mclk_multiple = I2S_MCLK_MULTIPLE_DEFAULT,
        .bits_per_chan = I2S_BITS_PER_CHAN_16BIT};

    i2s_pin_config_t pins = {
        .mck_io_num = -1,
        .bck_io_num = I2S_BCLK,
        .ws_io_num = I2S_WS,
        .data_out_num = I2S_DIN,
        .data_in_num = -1};

    esp_err_t err = i2s_driver_install(i2s_num, &cfg, 0, NULL);
    err |= i2s_set_pin(i2s_num, &pins);
    return err;
}

static const char *get_file_ext(const char *filename)
{
    const char *dot = strrchr(filename, '.');  // tìm dấu '.' cuối cùng
    if (!dot || dot == filename)
        return "";  // không có phần mở rộng
    return dot + 1;  // bỏ dấu '.' đầu
}

static void aac_decoder_run(Stream *input) {
    int r, w;
    aac_decoder.begin();
    while (input && input->available())
    {
        r = input->readBytes((char *)frame_buf, MP3_MAX_FRAME_SIZE);
        if (r <= 0)
            break;

        unsigned long ms = millis();
        int remaining = r;
        while (remaining > 0)
        {
            w = aac_decoder.write(frame_buf + (r - remaining), remaining);
            remaining -= w;
        }
        total_decode_ms += millis() - ms;
        vTaskDelay(1);
    }
    Serial.println("AAC playback done");
    aac_decoder.end();
    if (input)
        input->flush();
}

static void mp3_decoder_run(Stream *input) {
    int r, w;
    mp3_decoder.begin();
    while (input && input->available())
    {
        r = input->readBytes((char *)frame_buf, MP3_MAX_FRAME_SIZE);
        if (r <= 0)
            break;

        unsigned long ms = millis();
        int remaining = r;
        while (remaining > 0)
        {
            w = mp3_decoder.write(frame_buf + (r - remaining), remaining);
            remaining -= w;
        }
        total_decode_ms += millis() - ms;
        vTaskDelay(1);
    }
    Serial.println("mp3 playback done");
    mp3_decoder.end();
    if (input)
        input->flush();
}
// ==== Worker task ====
static void audio_task(void *param)
{
    audio_msg_t msg;
    Stream *input = nullptr;
    audio_ext_t ext = AUDIO_NOT_SUPPORTED;
    int r, w;
    void *decode = nullptr;
    for (;;)
    {
        if (xQueueReceive(audio_queue, &msg, portMAX_DELAY))
        {
            switch (msg.cmd)
            {
            case AUDIO_CMD_PLAY:
                input = msg.input;
                ext = msg.ext;
                Serial.println("🎵 Start playback");
                if (ext == AUDIO_ACC) {
                    aac_decoder_run(input);
                }
                else if (ext == AUDIO_MP3) {
                    mp3_decoder_run(input);
                }               
                break;

            case AUDIO_CMD_STOP:
                Serial.println("⏹ Stop playback");
                aac_decoder.end();
                break;

            default:
                break;
            }
        }
    }
}

// ==== Public API ====
void audio_service_init()
{
    if (audio_queue)
        return;
    i2s_init_config(SAMPLE_RATE);
    i2s_zero_dma_buffer(i2s_num);
    mp3_decoder.setDataCallback(mp3_data_callback);
    aac_decoder.setDataCallback(aacAudioDataCallback);
    audio_queue = xQueueCreate(4, sizeof(audio_msg_t));
    xTaskCreatePinnedToCore(audio_task, "audio_service_task", 4096, NULL,
                            configMAX_PRIORITIES - 3, &audio_task_handle, 0);

    Serial.println("Audio service initialized");
}

bool audio_send_command(audio_msg_t msg)
{
    if (!audio_queue)
        return false;
    return (xQueueSend(audio_queue, &msg, 0) == pdTRUE);
}

void audio_play_file(const char *file)
{
    File *f = new File(LittleFS.open(file));
    if (!f) return;
    audio_msg_t msg = {AUDIO_CMD_PLAY, f};
    const char *ext = get_file_ext(file);
    if (strcasecmp(ext, "aac") == 0) {
        msg.ext = AUDIO_ACC;
    }
    else if (strcasecmp(ext, "mp3") == 0) {
        msg.ext = AUDIO_MP3;
    }
    audio_send_command(msg);
}