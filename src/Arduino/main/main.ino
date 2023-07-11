#include "input.h"
#include "neopixel.h"

static Player p1;

void
setup (void)
{
  Serial.begin (9600);

  pinMode (BUTTON_PIN, INPUT_PULLUP);

  p1.id = 1;
  p1.x = 0;
  p1.y = 0;
  p1.js.x = 0;
  p1.js.y = 0;
  p1.js.btn_last = 0;

  neopixel_begin ();
  neopixel_clear ();
}

void
loop (void)
{
  input_process (&p1);
}
