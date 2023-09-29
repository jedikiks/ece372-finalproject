#include "include/neopixel.h"
#include "include/serial.h"

#include <Adafruit_NeoPixel.h>
#include <stdlib.h>

#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

static Adafruit_NeoPixel NeoPixel;

void
neopixel_begin (void)
{
  NeoPixel.setPin (NEOPIXEL_PIN);
  NeoPixel.updateLength (NUM_PIX);
  NeoPixel.updateType (NEO_GRB + NEO_KHZ800);
  NeoPixel.begin ();
}

void
neopixel_clear (void)
{
#ifdef DEBUG
  Serial.println ("\n**INFO => called neopixel_clear ()");
#endif
  NeoPixel.clear ();
}

unsigned char
neopixel_set (unsigned char pixel, unsigned char r, unsigned char g,
              unsigned char b)
{
  unsigned char status = 0;

#ifdef DEBUG
  Serial.println ("\n**INFO => called neopixel_set ()");
  Serial.print ("**INFO => data passed: ");
  Serial.print (pixel, HEX);
  Serial.print (", ");
  Serial.print (r, HEX);
  Serial.print (", ");
  Serial.print (g, HEX);
  Serial.print (", ");
  Serial.print (b, HEX);
  Serial.print ("\n");
#endif

  if (pixel > NUM_PIX)
    {
#ifdef DEBUG
      Serial.println ("**WARNING => invalid pixel");
#endif
      return 1;
    }
  else
    {
      NeoPixel.setPixelColor (pixel, NeoPixel.Color (r, g, b));
    }
  return status;
}

void
neopixel_reset (unsigned char pixel)
{
  NeoPixel.setPixelColor (pixel, NeoPixel.Color (0, 0, 0));
}

void
neopixel_draw (void)
{
#ifdef DEBUG
  Serial.println ("**INFO => calling neopixel_draw ()");
#endif
  NeoPixel.show ();
}
