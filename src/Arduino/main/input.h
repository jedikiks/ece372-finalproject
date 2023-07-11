#ifndef INPUT_H_
#define INPUT_H_

#include <Arduino.h>

#define VRX_PIN A0   // Arduino pin connected to VRX pin
#define VRY_PIN A1   // Arduino pin connected to VRY pin
#define BUTTON_PIN 8 // Arduino pin connected to SW  pin

#define LEFT_THRESHOLD 400
#define RIGHT_THRESHOLD 800
#define UP_THRESHOLD 400
#define DOWN_THRESHOLD 800

#define COMMAND_NULL 0x00
#define COMMAND_LEFT 0x01
#define COMMAND_RIGHT 0x02
#define COMMAND_UP 0x04
#define COMMAND_DOWN 0x08

#define DB_DELAY 500 // Delay used in debouncing

typedef struct
{
  int x; // To store value of the X axis
  int y; // To store value of the Y axis

  unsigned char btn_pressed; // To store value of the button
  unsigned char btn_last;    // Last state of button
  unsigned char btn_current; // Current state of button

  int command;
} Joystick;

typedef struct
{
  unsigned char id;
  unsigned char x;
  unsigned char y;
  Joystick js;
} Player;

/* Processes joystick information */
void input_process (Player *player);

#endif // INPUT_H_
