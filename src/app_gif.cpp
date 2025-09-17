// GIFDraw is called by AnimatedGIF library frame to screen
#include <Arduino.h>
#include <AnimatedGIF.h>           // GIF decoder library
#include "Drivers/screen_driver.h" // TFT_eSPI instance
#include "Assets/splash_honda.h"
#include "user_config.h"

#define DISPLAY_WIDTH TFT_HOR_RES
#define DISPLAY_HEIGHT TFT_VER_RES
#define BUFFER_SIZE 256 // Optimum is >= GIF width or integral division of width

// Static variables
static uint16_t usTemp[2][BUFFER_SIZE]; // Double buffer
static bool dmaBuf = 0;
static AnimatedGIF gif;

static void GIFDraw(GIFDRAW *pDraw)
{
  uint8_t *s;
  uint16_t *d, *usPalette;
  int x, y, iWidth, iCount;

  // Displ;ay bounds chech and cropping
  iWidth = pDraw->iWidth;
  if (iWidth + pDraw->iX > DISPLAY_WIDTH)
    iWidth = DISPLAY_WIDTH - pDraw->iX;
  usPalette = pDraw->pPalette;
  y = pDraw->iY + pDraw->y; // current line
  if (y >= DISPLAY_HEIGHT || pDraw->iX >= DISPLAY_WIDTH || iWidth < 1)
    return;

  // Old image disposal
  s = pDraw->pPixels;
  if (pDraw->ucDisposalMethod == 2) // restore to background color
  {
    for (x = 0; x < iWidth; x++)
    {
      if (s[x] == pDraw->ucTransparent)
        s[x] = pDraw->ucBackground;
    }
    pDraw->ucHasTransparency = 0;
  }

  // Apply the new pixels to the main image
  if (pDraw->ucHasTransparency) // if transparency used
  {
    uint8_t *pEnd, c, ucTransparent = pDraw->ucTransparent;
    pEnd = s + iWidth;
    x = 0;
    iCount = 0; // count non-transparent pixels
    while (x < iWidth)
    {
      c = ucTransparent - 1;
      d = &usTemp[0][0];
      while (c != ucTransparent && s < pEnd && iCount < BUFFER_SIZE)
      {
        c = *s++;
        if (c == ucTransparent) // done, stop
        {
          s--; // back up to treat it like transparent
        }
        else // opaque
        {
          *d++ = usPalette[c];
          iCount++;
        }
      } // while looking for opaque pixels
      if (iCount) // any opaque pixels?
      {
        // DMA would degrtade performance here due to short line segments
        display_flush_data_1(usTemp[0], pDraw->iX + x, y, iCount, 1);
        x += iCount;
        iCount = 0;
      }
      // no, look for a run of transparent pixels
      c = ucTransparent;
      while (c == ucTransparent && s < pEnd)
      {
        c = *s++;
        if (c == ucTransparent)
          x++;
        else
          s--;
      }
    }
  }
  else
  {
    s = pDraw->pPixels;

    // Unroll the first pass to boost DMA performance
    // Translate the 8-bit pixels through the RGB565 palette (already byte reversed)
    if (iWidth <= BUFFER_SIZE)
      for (iCount = 0; iCount < iWidth; iCount++)
        usTemp[dmaBuf][iCount] = usPalette[*s++];
    else
      for (iCount = 0; iCount < BUFFER_SIZE; iCount++)
        usTemp[dmaBuf][iCount] = usPalette[*s++];
    display_flush_data_1(&usTemp[dmaBuf][0], pDraw->iX, y, iCount, 1);
    dmaBuf = !dmaBuf;
    iWidth -= iCount;
    // Loop if pixel buffer smaller than width
    while (iWidth > 0)
    {
      // Translate the 8-bit pixels through the RGB565 palette (already byte reversed)
      if (iWidth <= BUFFER_SIZE)
        for (iCount = 0; iCount < iWidth; iCount++)
          usTemp[dmaBuf][iCount] = usPalette[*s++];
      else
        for (iCount = 0; iCount < BUFFER_SIZE; iCount++)
          usTemp[dmaBuf][iCount] = usPalette[*s++];
      display_flush_data_1(&usTemp[dmaBuf][0], pDraw->iX, y, iCount, 1);
      dmaBuf = !dmaBuf;
      iWidth -= iCount;
    }
  }
}
void gif_splash_view_init()
{
  gif.begin(LITTLE_ENDIAN_PIXELS);
  if (gif.open((uint8_t *)splash_honda, sizeof(splash_honda), GIFDraw))
  {
    // Serial.printf("Successfully opened GIF; Canvas size = %d x %d\n", gif.getCanvasWidth(), gif.getCanvasHeight());
    while (gif.playFrame(true, NULL))
    {
      yield();
    }
    gif.close();
  }
}