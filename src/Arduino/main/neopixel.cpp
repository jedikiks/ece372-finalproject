#include "neopixel.h"

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

static Adafruit_NeoPixel NeoPixel[PIX_ROW];

void
neopixel_begin (void)
{
  for (int i = 0; i < PIX_ROW; i++)
    {
      NeoPixel[i].setPin (i + 2);
      NeoPixel[i].updateLength (PIX_COL);
      NeoPixel[i].updateType (NEO_GRB + NEO_KHZ800);
      NeoPixel[i].begin ();
    }
}

void
neopixel_clear (void)
{
  for (int i = 0; i < PIX_ROW; i++)
    NeoPixel[i].clear ();
}

void
neopixel_set_color (int x, int y, unsigned char r, unsigned char g,
                    unsigned char b)
{
  NeoPixel[y].setPixelColor (x, NeoPixel[y].Color (r, g, b));
}

void
neopixel_set_color_player (int x, int y, unsigned char player)
{
  switch (player)
    {
    case 1:
      NeoPixel[y].setPixelColor (x, NeoPixel[y].Color (PLAYER_1_COLOR));
      break;
    case 2:
      NeoPixel[y].setPixelColor (x, NeoPixel[y].Color (PLAYER_2_COLOR));
      break;
    case 3:
      NeoPixel[y].setPixelColor (x, NeoPixel[y].Color (PLAYER_3_COLOR));
      break;
    default:
      break;
    }
}

void
neopixel_update (void)
{
  for (int i = 0; i < PIX_ROW; i++)
    NeoPixel[i].show ();
}
