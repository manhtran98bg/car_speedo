#pragma once
#include <Arduino.h>
#include <FS.h>
#include "MP3DecoderHelix.h"
#include "AACDecoderHelix.h"

// Audio commands
typedef enum {
    AUDIO_CMD_NONE = 0,
    AUDIO_CMD_PLAY,
    AUDIO_CMD_STOP
} audio_cmd_t;

typedef enum {
    AUDIO_MP3,
    AUDIO_ACC,
    AUDIO_NOT_SUPPORTED
} audio_ext_t;
// Command struct
typedef struct {
    audio_cmd_t cmd;
    Stream *input; 
    audio_ext_t ext;
} audio_msg_t;

void audio_service_init();
bool audio_send_command(audio_msg_t msg);
void audio_play_file(const char *file);
