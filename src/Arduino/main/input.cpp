#include "input.h"
#include "neopixel.h"

#include <stddef.h>

static int pixel_flash = 1;

static unsigned long pressedTime = 0;
static unsigned long releasedTime = 0;

void
debounce (Player *player)
{
  Joystick *Js = &player->js;

  Js->btn_pressed = 0;
  Js->btn_current = digitalRead (BUTTON_PIN);

  if (Js->btn_last == HIGH && Js->btn_current == LOW) // button is pressed
    pressedTime = millis ();
  else if (Js->btn_last == LOW && Js->btn_current == HIGH)
    { // button is released
      releasedTime = millis ();

      long pressDuration = releasedTime - pressedTime;

      if (pressDuration < DB_DELAY)
        {
          Serial.println ("A short press is detected");
          Js->btn_pressed = 1;
        }
    }

  // save the the last state
  Js->btn_last = Js->btn_current;

  if (Js->btn_pressed == 1)
    {
      neopixel_set_color_player (player->x, player->y, 1);
    }
  else
    {
      // RGB idle flash
      neopixel_set_color_player (player->x, player->y, pixel_flash);

      if (pixel_flash < 3)
        pixel_flash++;
      else
        pixel_flash = 0;

      delay (50);
    }
}

void
input_process (Player *player)
{
  Joystick *Js = &player->js;

  // update joystick position values
  Js->x = analogRead (VRX_PIN);
  Js->y = analogRead (VRY_PIN);

  // check if joystick button is pressed
  // Js->btn_pressed = 0;
  debounce (player);

  // converts the analog value to commands
  // reset commands
  Js->command = COMMAND_NULL;

  // check left/right commands
  if (Js->x < LEFT_THRESHOLD)
    Js->command = Js->command | COMMAND_LEFT;
  else if (Js->x > RIGHT_THRESHOLD)
    Js->command = Js->command | COMMAND_RIGHT;

  // check up/down commands
  if (Js->y < UP_THRESHOLD)
    Js->command = Js->command | COMMAND_UP;
  else if (Js->y > DOWN_THRESHOLD)
    Js->command = Js->command | COMMAND_DOWN;

  // NOTE: AT A TIME, THERE MAY BE NO COMMAND, ONE COMMAND OR TWO COMMANDS

  // print command to serial and process command
  if (Js->command & COMMAND_LEFT)
    {
      Serial.println ("COMMAND LEFT");
      if (player->x < PIX_COL - 1)
        {
          neopixel_set_color (player->x, player->y, 0, 0, 0);
          player->x += 1;
          neopixel_set_color_player (player->x, player->y, player->id);
        }
    }

  if (Js->command & COMMAND_RIGHT)
    {
      Serial.println ("COMMAND LEFT");
      if (player->x > 0)
        {
          neopixel_set_color (player->x, player->y, 0, 0, 0);
          player->x -= 1;
          neopixel_set_color_player (player->x, player->y, player->id);
        }
    }

  if (Js->command & COMMAND_UP)
    {
      Serial.println ("COMMAND UP");
      if (player->y > 0)
        {
          neopixel_set_color (player->x, player->y, 0, 0, 0);
          player->y -= 1;
          neopixel_set_color_player (player->x, player->y, player->id);
        }
    }

  if (Js->command & COMMAND_DOWN)
    {
      Serial.println ("COMMAND DOWN");
      if (player->y < PIX_ROW - 1)
        {
          neopixel_set_color (player->x, player->y, 0, 0, 0);
          player->y += 1;
          neopixel_set_color_player (player->x, player->y, player->id);
        }
    }

  neopixel_update ();
}
