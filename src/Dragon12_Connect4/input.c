#include "input.h"
#include "derivative.h" /* derivative-specific definitions */
#include "game.h"
#include "lcd.h"
#include "neopixel.h"
#include <hidef.h> /* common defines and macros */

// unsigned char toprowLEDs[7]={41,40,39,38,37,36,35};
// unsigned char subtractor = 13;
// int j = 0;

/*  const unsigned char horizontalLEDs[42] = {0,1,2,3,4,5,6,7,8,9,10,11,
  12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,
  33,34,35,36,37,38,39,40,41};
  const unsigned char verticalLEDs[42] = {0,13,14,27,28,41,40,29,26,15,12,
  1,2,11,16,25,30,39,38,31,24,17,10,3,4,9,18,23,32,37,36,33,22,19,8,5,
  6,7,20,21,34,35};
  */
const unsigned char LEDs[42]
    = { 0,  13, 14, 27, 28, 41, 1,  12, 15, 26, 29, 40, 2,  11,
        16, 25, 30, 39, 3,  10, 17, 24, 31, 38, 4,  9,  18, 23,
        32, 37, 5,  8,  19, 22, 33, 36, 6,  7,  20, 21, 34, 35 };

int n = 0;
unsigned char i;
int count = 0;
int count2 = 0;

#ifdef JOYSTICK_1
static Joystick j1;
#endif
#ifdef JOYSTICK_2
static Joystick j2;
#endif
#ifdef JOYSTICK_3
static Joystick j3;
#endif

void counter (int counta);
void vh_check (Player *player, Game *game, unsigned char *led_arr, int len,
               int maxcount);
void update_sw (void);
unsigned char debounce (Joystick *js);
int analog_read (unsigned char pin);
void free_move_up (Player *player);
void free_move_down (Player *player);
void free_move_left (Player *player);
void free_move_right (Player *player);
void press_button (Player *player);
int fillcolumn (Player *player, int top, int x, int max, Game *connect_4);
// void horizontalcheck(Player *player, Game *game);
void blink (Player *player, unsigned char pos[], unsigned int delay);
void blink2 (Player *player, unsigned char led);
void sercom_send (unsigned char data);

void
ms_delay (unsigned int t)
{
  unsigned int i, j;

  for (i = 0; i < t; i++)
    for (j = 0; j < 4000; j++)
      ;
}

int
analog_read (unsigned char pin)
{
  ATD1CTL5 = pin;

  while (!(ATD1STAT0 & 0x80))
    ;

  return ((ATD1DR0H << 8) | ATD1DR0L); // combine both
}

unsigned char
debounce (Joystick *js)
{
  unsigned char initial_read;
  update_sw ();
  initial_read = js->pin_sw;

  while (initial_read == 0)
    {
      if (initial_read != js->pin_sw)
        {
          return 1;
          break;
        }

      ms_delay (50);
      update_sw ();
    }

  return 0;
}

void
joystick_setup (void)
{
  // ADC setup
  ATD1CTL2 = 0x80; // Turn on ADC,..No Interrupt
  ATD1CTL3 = 0x08; // one conversion, no FIFO
  ATD1CTL4 = 0x6B; // E8-bit resolu, 16-clock for 2nd phase,
                   // prescaler of 24 for Conversion Freq=1MHz

#ifdef JOYSTICK_1
  j1.pin_x = JOYSTICK_1_X;
  j1.pin_y = JOYSTICK_1_Y;
  j1.pin_sw = JOYSTICK_1_SW_PORT;
  JOYSTICK_1_SW_DDR = 0;
#endif
#ifdef JOYSTICK_2
  j2.pin_x = JOYSTICK_2_X;
  j2.pin_y = JOYSTICK_2_Y;
  j2.pin_sw = JOYSTICK_2_SW_PORT;
  JOYSTICK_2_SW_DDR = 0;
#endif
#ifdef JOYSTICK_3
  j3.pin_x = JOYSTICK_3_X;
  j3.pin_y = JOYSTICK_3_Y;
  j3.pin_sw = JOYSTICK_3_SW_PORT;
  JOYSTICK_3_SW_DDR 0;
#endif
}

void
update_sw (void)
{
#ifdef JOYSTICK_1
  j1.pin_sw = JOYSTICK_1_SW_PORT;
#endif
#ifdef JOYSTICK_2
  j2.pin_sw = JOYSTICK_2_SW_PORT;
#endif
#ifdef JOYSTICK_3
  j3.pin_sw = JOYSTICK_3_SW_PORT;
#endif
}

unsigned char
joystick_get_input (unsigned char joystick_num)
{
  unsigned char status = 0;
  Joystick *js;

  switch (joystick_num)
    {
#ifdef JOYSTICK_1
    case 1:
      js = &j1;
      break;
#endif
#ifdef JOYSTICK_2
    case 2:
      js = &j2;
      break;
#endif
#ifdef JOYSTICK_3
    case 3:
      js = &j3;
      break;
#endif
    default:
      return 1;
      break;
    }

  // update joystick position values
  js->x = analog_read (js->pin_x);
  js->y = analog_read (js->pin_y);

  // check if joystick button is pressed
  if (debounce (js) == 1)
    return 'b';

  // converts the analog value to commands
  // reset commands
  js->command = COMMAND_NULL;

  // check left/right commands
  if (js->x < LEFT_THRESHOLD)
    js->command = js->command | COMMAND_LEFT;
  else if (js->x > RIGHT_THRESHOLD)
    js->command = js->command | COMMAND_RIGHT;

  // check up/down commands
  if (js->y < UP_THRESHOLD)
    js->command = js->command | COMMAND_UP;
  else if (js->y > DOWN_THRESHOLD)
    js->command = js->command | COMMAND_DOWN;

  // Command processing
  if (js->command & COMMAND_LEFT)
    return 'l';
  else if (js->command & COMMAND_RIGHT)
    return 'r';
  else if (js->command & COMMAND_UP)
    return 'u';
  else if (js->command & COMMAND_DOWN)
    return 'd';

  return 0;
}

unsigned char
joystick_lr_move (Player *player)
{
  unsigned char status = 0;

  if (player->id == 0 || player->id > 3)
    {
      return 2;
    }
  switch (joystick_get_input (player->id))
    {
    case 'r':
      free_move_right (player);
      break;
    case 'l':
      free_move_left (player);
      break;

    case 'd':
      return player->pos;
      break;

    default:
      status = 1;
      break;
    }

  return status;
}

unsigned char
joystick_free_move (Player *player)
{
  unsigned char status = 0;

  if (player->id == 0 || player->id > 3)
    return 2;

  switch (joystick_get_input (player->id))
    {
    case 'u':
      free_move_up (player);
      break;

    case 'd':
      free_move_down (player);
      break;

    case 'r':
      free_move_right (player);
      break;

    case 'l':
      free_move_left (player);
      break;

    case 'b':
      press_button (player);
      break;

    default:
      status = 1;
      break;
    }

  return status;
}

void
free_move_up (Player *player)
{
  if ((player->pos + ((2 * (PIX_COL - ((player->pos) % PIX_COL))) + 1))
      <= (NUM_PIX + 1)) // Check if the space above can be free_moved into
    {
      switch (player->pos % PIX_COL) // Check if this is an edge
        {
        case 0:                                                   // == PIX_COL
          if (((player->pos - (player->pos % PIX_COL)) % 2) == 0) // Even strip
            {
              neopixel_reset (player->pos);
              player->pos += ((2 * PIX_COL) - 1);
              neopixel_set (player->pos, player->color[0], player->color[1],
                            player->color[2]);
            }
          else // Odd strip
            {
              neopixel_reset (player->pos);
              player->pos += ((2 * PIX_COL) - 1);
              neopixel_set (player->pos, player->color[0], player->color[1],
                            player->color[2]);
            }
          break;
        case (PIX_COL - 1): // == PIX_COL - 1
          if (((player->pos - (player->pos % PIX_COL)) % 2) == 0) // Even strip
            {
              neopixel_reset (player->pos);
              player->pos += 1;
              neopixel_set (player->pos, player->color[0], player->color[1],
                            player->color[2]);
            }
          else // Odd strip
            {
              neopixel_reset (player->pos);
              player->pos += 1;
              neopixel_set (player->pos, player->color[0], player->color[1],
                            player->color[2]);
            }
          break;
        default:
          neopixel_reset (player->pos);
          player->pos += ((2 * (PIX_COL - ((player->pos + 1) % PIX_COL))) + 1);
          neopixel_set (player->pos, player->color[0], player->color[1],
                        player->color[2]);
          break;
        }
    }
}

void
free_move_down (Player *player)
{
  if (player->pos == (PIX_COL - 1))
    return;
  if ((player->pos - ((2 * ((player->pos + 1) % PIX_COL)) - 1))
      > 0) // Check 1 row down
    {
      switch (player->pos % PIX_COL) // Is this an edge
        {
        case 0:
          if (((player->pos - (player->pos % PIX_COL)) % 2) == 0) // Even strip
            {
              neopixel_reset (player->pos);
              player->pos -= 1;
              neopixel_set (player->pos, player->color[0], player->color[1],
                            player->color[2]);
            }
          else // Odd strip
            {
              neopixel_reset (player->pos);
              player->pos -= 1;
              neopixel_set (player->pos, player->color[0], player->color[1],
                            player->color[2]);
            }
          break;
        case (PIX_COL - 1):
          if (((player->pos - (player->pos % PIX_COL)) % 2) == 0) // Even strip
            {
              neopixel_reset (player->pos);
              player->pos -= ((2 * PIX_COL) - 1);
              neopixel_set (player->pos, player->color[0], player->color[1],
                            player->color[2]);
            }
          else // Odd strip
            {
              neopixel_reset (player->pos);
              player->pos -= ((2 * PIX_COL) - 1);
              neopixel_set (player->pos, player->color[0], player->color[1],
                            player->color[2]);
            }
          break;
        default:
          neopixel_reset (player->pos);
          player->pos -= ((2 * ((player->pos + 1) % PIX_COL)) - 1);
          neopixel_set (player->pos, player->color[0], player->color[1],
                        player->color[2]);
          break;
        }
    }
}
void
free_move_left (Player *player)
{
  switch ((player->pos - (player->pos % PIX_COL)) % 2)
    {
    case 0:
      if (player->pos < NUM_PIX - 1)
        {
          if ((player->pos % PIX_COL) == 0)
            break;
          else
            {
              neopixel_reset (player->pos);
              player->pos -= 1;
              neopixel_set (player->pos, player->color[0], player->color[1],
                            player->color[2]);
            }
        }
      break;
    default:
      if (player->pos > 0)
        {
          if ((player->pos % PIX_COL) == (PIX_COL - 1))
            break;
          else
            {
              neopixel_reset (player->pos);
              player->pos += 1;
              neopixel_set (player->pos, player->color[0], player->color[1],
                            player->color[2]);
            }
        }
      break;
    }
}

void
free_move_right (Player *player)
{
  switch ((player->pos - (player->pos % PIX_COL)) % 2)
    {
    case 0: // if on an even row
      if (player->pos < NUM_PIX - 1)
        {
          if ((player->pos % PIX_COL) == (PIX_COL - 1))
            break;
          else
            {
              neopixel_reset (player->pos);
              player->pos += 1;
              neopixel_set (player->pos, player->color[0], player->color[1],
                            player->color[2]);
            }
        }
      break;
    default:
      if (player->pos <= NUM_PIX - 1)
        {
          if ((player->pos % PIX_COL) == 0)
            break;
          else
            {
              neopixel_reset (player->pos);
              player->pos -= 1;
              neopixel_set (player->pos, player->color[0], player->color[1],
                            player->color[2]);
            }
        }
      break;
    }
}
int
fillcolumn (Player *player, int top, int x, int max, Game *connect_4)
{
  unsigned char LEDs[42]
      = { 0,  13, 14, 27, 28, 41, 1,  12, 15, 26, 29, 40, 2,  11,
          16, 25, 30, 39, 3,  10, 17, 24, 31, 38, 4,  9,  18, 23,
          32, 37, 5,  8,  19, 22, 33, 36, 6,  7,  20, 21, 34, 35 };
  unsigned char toprowLEDs[7] = { 41, 40, 39, 38, 37, 36, 35 };
  unsigned char subtractor = 13;
  int j = 0;
  DDRB = 0xFF;

  if (joystick_get_input (player->id) == 'd' && player->pos == top)
    {

      for (x; x <= max; x++)
        {

          if (connect_4->board[LEDs[x]] != 0)
            {
              // DONOTHING
            }
          else
            { // LIGHTLED

              if (player->id == 1)
                { // LIGHTREDLED
                  neopixel_set (LEDs[x], player->color[0], player->color[1],
                                player->color[2]);
                  neopixel_draw ();
                  connect_4->board[LEDs[x]] = player->id;

                  /*   if(connect_4->board[horizontalLEDs[x]] == player->id){
                        PORTB = 2;
                        ms_delay(1000);
                        PORTB = 0;
                     }     */

                  // return connect_4->current_player = 1;
                  return 1;
                }
              else if (player->id == 2)
                { // LIGHTBLUELED
                  neopixel_set (LEDs[x], player->color[0], player->color[1],
                                player->color[2]);
                  neopixel_draw ();
                  connect_4->board[LEDs[x]] = player->id;
                  // return connect_4->current_player = 2;
                  return 2;
                }
            }
        }
    }
  return 0;
}
/*
void horizontalcheck(Player *player, Game *game){
   //unsigned char i;

   unsigned char horizontalLEDs[42] = {0,1,2,3,4,5,6,7,8,9,10,11,
   12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,
   33,34,35,36,37,38,39,40,41};
   unsigned char WinArray[4]={0,0,0,0};
   int n = 0;
   DDRB = 0xFF;

     for(n=0;n<42;n++){
      count+=1;

       //PORTB = count2;
       //ms_delay(10);
       //PORTB = 0;


      if(count==8){

       count2 = 0;
       count = 1;

      }
      if(game->board[horizontalLEDs[n]] == player->id){
       count2 += 1;

      } else{
       count2 = 0;
       WinArray[0] = 0;
       WinArray[1] = 0;
       WinArray[2] = 0;
       WinArray[3] = 0;
      }
      if (count2 == 1){
        WinArray[0] = horizontalLEDs[n];
      PORTB = WinArray[0];
      ms_delay(100);
      PORTB = 0;
      }
      if (count2 == 2){
        WinArray[1] = horizontalLEDs[n];
      PORTB = WinArray[1];
      ms_delay(100);
      PORTB = 0;
      }
      if (count2 == 3){
        WinArray[2] = horizontalLEDs[n];
        PORTB = WinArray[2];
        ms_delay(100);
        PORTB = 0;
      }
      if (count2 == 4){

        WinArray[3] = horizontalLEDs[n];
        PORTB = WinArray[3];
        ms_delay(100);
        PORTB = 0;

        for (i = 0; i < 5; i++)
          {
            blink2(player, WinArray[0]);
            blink2(player, WinArray[1]);
            blink2(player, WinArray[2]);
            blink2(player, WinArray[3]);
            //blink(player, WinArray,200);
          }

        neopixel_clear ();
      }


     }


} */
void
vh_check (Player *player, Game *game, unsigned char *led_arr, int len,
          int maxcount)
{
  // unsigned char i;

  unsigned char WinArray[4] = { 0, 0, 0, 0 };
  int n = 0;
  int count2 = 0;
  int count = 0;
  DDRB = 0xFF;

  for (n = 0; n < len; n++)
    {
      count += 1;
      if (count == maxcount)
        {

          count2 = 0;
          count = 1;
        }
      if (game->board[led_arr[n]] == player->id)
        {
          count2 += 1;
        }
      else
        {
          count2 = 0;
          WinArray[0] = 0;
          WinArray[1] = 0;
          WinArray[2] = 0;
          WinArray[3] = 0;
        }
      if (count2 == 1)
        {
          WinArray[0] = led_arr[n];
        }
      if (count2 == 2)
        {
          WinArray[1] = led_arr[n];
        }
      if (count2 == 3)
        {
          WinArray[2] = led_arr[n];
        }
      if (count2 == 4)
        {

          WinArray[3] = led_arr[n];

          // Print win msg
          lcd_clear ();

          str_split ("PLAYER ");
          switch (player->id)
            {
            case 1:
              str_split ("1");
              break;
            case 2:
              str_split ("2");
              break;
            case 3:
              str_split ("3");
              break;
            }
          str_split (" WINS");

          for (i = 0; i < 5; i++)
            {
              blink (player, WinArray, 100);
            }
          neopixel_clear ();
          for (i = 0; i < 42; i++)
            game->board[i] = 0;
          game->score[player->id]++;
        }
    }
}
void
diagonal_check (Player *player, Game *game, unsigned char *led_arr, int len)
{
  int h = 0;
  int count = 0;
  int count2 = 0;
  int count3 = 0;
  int count4 = 0;
  int count5 = 0;
  int count6 = 0;
  int count7 = 0;
  unsigned char WinArray1[4] = { 0, 0, 0, 0 };
  unsigned char WinArray2[4] = { 0, 0, 0, 0 };
  unsigned char WinArray3[4] = { 0, 0, 0, 0 };
  unsigned char WinArray4[4] = { 0, 0, 0, 0 };
  unsigned char WinArray5[4] = { 0, 0, 0, 0 };
  unsigned char WinArray6[4] = { 0, 0, 0, 0 };

  for (h = 0; h < len; h++)
    {

      count += 1;

      if (game->board[led_arr[h]] == player->id && count <= 4)
        {
          count2 += 1;
        }
      else
        {
          count2 = 0;
          WinArray1[0] = 0;
          WinArray1[1] = 0;
          WinArray1[2] = 0;
          WinArray1[3] = 0;
        }
      if (count2 == 1)
        {
          WinArray1[0] = led_arr[h];
        }
      if (count2 == 2)
        {
          WinArray1[1] = led_arr[h];
        }
      if (count2 == 3)
        {
          WinArray1[2] = led_arr[h];
        }
      if (count2 == 4)
        {
          WinArray1[3] = led_arr[h];

          // Print win msg
          lcd_clear ();

          str_split ("PLAYER ");
          switch (player->id)
            {
            case 1:
              str_split ("1");
              break;
            case 2:
              str_split ("2");
              break;
            case 3:
              str_split ("3");
              break;
            }
          str_split (" WINS");

          for (i = 0; i < 5; i++)
            {
              blink (player, WinArray1, 100);
            }
          neopixel_clear ();
          for (i = 0; i < 42; i++)
            game->board[i] = 0;
          game->score[player->id]++;
        }
      if (game->board[led_arr[h]] == player->id && count >= 5 && count <= 9)
        {
          count3 += 1;
        }
      else
        {
          count3 = 0;
          WinArray2[0] = 0;
          WinArray2[1] = 0;
          WinArray2[2] = 0;
          WinArray2[3] = 0;
        }
      if (count3 == 1)
        {
          WinArray2[0] = led_arr[h];
        }
      if (count3 == 2)
        {
          WinArray2[1] = led_arr[h];
        }
      if (count3 == 3)
        {
          WinArray2[2] = led_arr[h];
        }
      if (count3 == 4)
        {
          WinArray2[3] = led_arr[h];

          // Print win msg
          lcd_clear ();

          str_split ("PLAYER ");
          switch (player->id)
            {
            case 1:
              str_split ("1");
              break;
            case 2:
              str_split ("2");
              break;
            case 3:
              str_split ("3");
              break;
            }
          str_split (" WINS");

          for (i = 0; i < 5; i++)
            {
              blink (player, WinArray2, 100);
            }
          neopixel_clear ();
          for (i = 0; i < 42; i++)
            game->board[i] = 0;
          game->score[player->id]++;
        }
      if (game->board[led_arr[h]] == player->id && count >= 10 && count <= 15)
        {
          count4 += 1;
        }
      else
        {
          count4 = 0;
          WinArray3[0] = 0;
          WinArray3[1] = 0;
          WinArray3[2] = 0;
          WinArray3[3] = 0;
        }
      if (count4 == 1)
        {
          WinArray3[0] = led_arr[h];
        }
      if (count4 == 2)
        {
          WinArray3[1] = led_arr[h];
        }
      if (count4 == 3)
        {
          WinArray3[2] = led_arr[h];
        }
      if (count4 == 4)
        {
          WinArray3[3] = led_arr[h];

          // Print win msg
          lcd_clear ();

          str_split ("PLAYER ");
          switch (player->id)
            {
            case 1:
              str_split ("1");
              break;
            case 2:
              str_split ("2");
              break;
            case 3:
              str_split ("3");
              break;
            }
          str_split (" WINS");

          for (i = 0; i < 5; i++)
            {
              blink (player, WinArray3, 100);
            }
          neopixel_clear ();
          for (i = 0; i < 42; i++)
            game->board[i] = 0;
          game->score[player->id]++;
        }
      if (game->board[led_arr[h]] == player->id && count >= 16 && count <= 21)
        {
          count5 += 1;
        }
      else
        {
          count5 = 0;
          WinArray4[0] = 0;
          WinArray4[1] = 0;
          WinArray4[2] = 0;
          WinArray4[3] = 0;
        }
      if (count5 == 1)
        {
          WinArray4[0] = led_arr[h];
        }
      if (count5 == 2)
        {
          WinArray4[1] = led_arr[h];
        }
      if (count5 == 3)
        {
          WinArray4[2] = led_arr[h];
        }
      if (count5 == 4)
        {
          WinArray4[3] = led_arr[h];

          // Print win msg
          lcd_clear ();

          str_split ("PLAYER ");
          switch (player->id)
            {
            case 1:
              str_split ("1");
              break;
            case 2:
              str_split ("2");
              break;
            case 3:
              str_split ("3");
              break;
            }
          str_split (" WINS");

          for (i = 0; i < 5; i++)
            {
              blink (player, WinArray4, 100);
            }
          neopixel_clear ();
          for (i = 0; i < 42; i++)
            game->board[i] = 0;
          game->score[player->id]++;
        }

      if (game->board[led_arr[h]] == player->id && count >= 22 && count <= 26)
        {
          count6 += 1;
        }
      else
        {
          count6 = 0;
          WinArray5[0] = 0;
          WinArray5[1] = 0;
          WinArray5[2] = 0;
          WinArray5[3] = 0;
        }
      if (count6 == 1)
        {
          WinArray5[0] = led_arr[h];
        }
      if (count6 == 2)
        {
          WinArray5[1] = led_arr[h];
        }
      if (count6 == 3)
        {
          WinArray5[2] = led_arr[h];
        }
      if (count6 == 4)
        {
          WinArray5[3] = led_arr[h];

          // Print win msg
          lcd_clear ();

          str_split ("PLAYER ");
          switch (player->id)
            {
            case 1:
              str_split ("1");
              break;
            case 2:
              str_split ("2");
              break;
            case 3:
              str_split ("3");
              break;
            }
          str_split (" WINS");

          for (i = 0; i < 5; i++)
            {
              blink (player, WinArray5, 100);
            }
          neopixel_clear ();
          for (i = 0; i < 42; i++)
            game->board[i] = 0;
          game->score[player->id]++;
        }
      if (game->board[led_arr[h]] == player->id && count >= 27 && count <= 30)
        {
          count7 += 1;
        }
      else
        {
          count7 = 0;
          WinArray6[0] = 0;
          WinArray6[1] = 0;
          WinArray6[2] = 0;
          WinArray6[3] = 0;
        }
      if (count7 == 1)
        {
          WinArray6[0] = led_arr[h];
        }
      if (count7 == 2)
        {
          WinArray6[1] = led_arr[h];
        }
      if (count7 == 3)
        {
          WinArray6[2] = led_arr[h];
        }
      if (count7 == 4)
        {
          WinArray6[3] = led_arr[h];

          // Print win msg
          lcd_clear ();

          str_split ("PLAYER ");
          switch (player->id)
            {
            case 1:
              str_split ("1");
              break;
            case 2:
              str_split ("2");
              break;
            case 3:
              str_split ("3");
              break;
            }
          str_split (" WINS");

          for (i = 0; i < 5; i++)
            {
              blink (player, WinArray5, 100);
            }
          neopixel_clear ();
          for (i = 0; i < 42; i++)
            game->board[i] = 0;
          game->score[player->id]++;
        }
    }
}

void
blink (Player *player, unsigned char pos[], unsigned int delay)
{
  unsigned int i;

  for (i = 0; i < 4; i++)
    {
      neopixel_set (pos[i], player->color[0], player->color[1],
                    player->color[2]);
    }
  neopixel_draw ();
  ms_delay (delay); // or however long the blink freq is

  for (i = 0; i < 4; i++)
    {
      neopixel_reset (pos[i]);
    }
  neopixel_draw ();
}

void
blink2 (Player *player, unsigned char led)
{

  neopixel_set (led, player->color[0], player->color[1], player->color[2]);
  neopixel_draw ();

  ms_delay (20);
  neopixel_reset (led);
  neopixel_draw ();

  neopixel_set (led, player->color[0], player->color[1], player->color[2]);
  neopixel_draw ();
}

void
press_button (Player *player)
{
  /* TODO: put a useful function here to interpret button presses. For now
   * it just takes the player back to the beginning */

  neopixel_reset (player->pos);
  player->pos = 0;
  neopixel_set (player->pos, player->color[0], player->color[1],
                player->color[2]);
}
