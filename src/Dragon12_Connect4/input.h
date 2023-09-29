#ifndef INPUT_H_
#define INPUT_H_

#include "game.h"

//========================================================
// Joystick pinouts + sensitivity settings
// These pinouts NEED to be defined for each joystick
//========================================================

// Joystick 1
#define JOYSTICK_1
#define JOYSTICK_1_X 0x83
#define JOYSTICK_1_Y 0x84
#define JOYSTICK_1_SW_DDR DDRA_BIT0
#define JOYSTICK_1_SW_PORT PORTA
#define JOYSTICK_1_SW_PORT_BIT PORTA_BIT0

// Joystick 2
#define JOYSTICK_2
#define JOYSTICK_2_X 0x83
#define JOYSTICK_2_Y 0x84
#define JOYSTICK_2_SW_DDR DDRA_BIT0
#define JOYSTICK_2_SW_PORT PORTA
#define JOYSTICK_2_SW_PORT_BIT PORTA_BIT0

/* Joystick 3:
#define JOYSTICK_3
#define JOYSTICK_3_X
#define JOYSTICK_3_Y
#define JOYSTICK_3_SW_DDR
#define JOYSTICK_3_SW_PORT
#define JOYSTICK_3_SW_PORT_BIT
*/

/* Joystick sensitivity */
#define LEFT_THRESHOLD 400
#define RIGHT_THRESHOLD 800
#define UP_THRESHOLD 400
#define DOWN_THRESHOLD 800

/* Joystick command definitions */
#define COMMAND_NULL 0x00
#define COMMAND_LEFT 0x01
#define COMMAND_RIGHT 0x02
#define COMMAND_UP 0x04
#define COMMAND_DOWN 0x08

//==============
// Structs
//==============

typedef struct
{
  unsigned char id; // Joystick ID
  int x;            // ADC value of the X axis
  int y;            // ADC value of the Y axis

  int pin_x;  // Pin on the D12 to joystick X pin
  int pin_y;  // Pin on the D12 to joystick Y pin
  int pin_sw; // Pin on the D12 to joystick SW pin

  int command; // Input to command processing
} Joystick;

//==============
// Functions
//==============

/* Reads + interprets analog values of a given joystick. Joysticks
 * passed to this function **must** be defined in this file above. Returns 'u',
 * 'd', 'l', 'r' for up, down, left, right directions and 'b' for a button
 * press. Returns 1 if the given joystick is invalid. Returns 2 if no input was
 * found */
unsigned char joystick_get_input (unsigned char joystick_num);

/* Sets up a Joystick struct for each defined joystick. Joysticks
 * and their pinouts should be defined above */
void joystick_setup (void);

/* Processes the joystick input for a given player. Allows for free 2D
 * movement. Returns 0 on success, 1 if there is no input and 2 if the player
 * id is invalid */
unsigned char joystick_free_move (Player *player);

/* Processes the joystick input for a given player. Movement is restricted to
 * the left and right directions. Returns 0 on success, 1 if there is no input
 * and 2 if the player id is invalid */
unsigned char joystick_lr_move (Player *player);
//unsigned char joystick_lr_move (Player *player, Game *game);

void ms_delay (unsigned int t);

#endif // INPUT_H_
