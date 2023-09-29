#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "input.h"
#include "game.h"
#include "neopixel.h"


void
ms_delay_ (unsigned int t)
{
  unsigned int i, j;

  for (i = 0; i < t; i++)
    for (j = 0; j < 4000; j++)
      ;
}

void sercom_send (unsigned char data);


void
neopixel_set (unsigned char pixel, unsigned char r, unsigned char g,
              unsigned char b)
{
  sercom_send ('A');
  sercom_send (pixel);
  sercom_send (r);
  sercom_send (g);
  sercom_send (b);
}

void
neopixel_reset (unsigned char pixel)
{
  sercom_send ('B');
  sercom_send (pixel);
}

void
neopixel_draw (void)
{
  sercom_send ('C');
}

void
neopixel_clear (void)
{
  sercom_send ('D');
}

void
sercom_send (unsigned char data)
{
  while (!(SCI1SR1 & 0x80))
    ;
  SCI1DRL = data;
  ms_delay_ (10); //FIXME: might need bigger delay
}

void
neopixel_setup (void)
{
  SCI1BDH = 0;
  SCI1BDL = 26;
  SCI1CR1 = 0;
  SCI1CR2 = 0x0C;

  neopixel_clear ();
  neopixel_draw ();
}
