#include "derivative.h"
#include <hidef.h>
#include <stdlib.h>

#include "include/input.h"
#include "include/lcd.h"
#include "include/neopixel.h"

#define LCD_DATA PORTK
#define LCD_CTRL PORTK
#define RS 0x01
#define EN 0x02

void COMWRT4 (unsigned char);
void DATWRT4 (unsigned char);
void update_num (Player *player);
void str_split (char string[]);
void ascii_conv (unsigned char num, unsigned char *to_ascii);

void
update_num (Player *player)
{
  unsigned char to_ascii[3], i;

  lcd_update_cursor (0);

  str_split ("R");
  ascii_conv (player->color[0], to_ascii);
  for (i = 0; i < 3; i++)
    lcd_print (to_ascii[i]);

  str_split (" G");
  ascii_conv (player->color[1], to_ascii);
  for (i = 0; i < 3; i++)
    lcd_print (to_ascii[i]);

  str_split (" B");
  ascii_conv (player->color[2], to_ascii);
  for (i = 0; i < 3; i++)
    lcd_print (to_ascii[i]);
}

void
ascii_conv (unsigned char num, unsigned char *to_ascii)
{
  to_ascii[2] = (num % 10) + 0x30;
  to_ascii[1] = ((num / 10) % 10) + 0x30;
  to_ascii[0] = ((num / 100) % 10) + 0x30;
}

void
lcd_print (unsigned char uchar)
{
  DATWRT4 (uchar);
  ms_delay (1);
}

void
str_split (char string[])
{
  int idx = 0;

  while (string[idx] != '\0')
    {
      if (idx > 65536)
        break;

      lcd_print (string[idx]);
      ms_delay (1);

      idx++;
    }
}

void
lcd_update_cursor (unsigned char rgb_sel)
{
  switch (rgb_sel)
    {
    /* Very beginning of the bottom row */
    case 0:
      COMWRT4 (0xC0);
      break;

    /* 5 spaces away from beginning of bottom row */
    case 1:
      COMWRT4 (0xC5);
      break;

    /* 10 spaces away from beginning of bottom row */
    case 2:
      COMWRT4 (0xCA);
      break;

    default:
      return;
      break;
    }
}

void
lcd_clear (void)
{
  COMWRT4 (0x01); // Clear display
  ms_delay (1);
  COMWRT4 (0x80); // set start posistion, home position
  ms_delay (1);
}

void
lcd_setup (void)
{
  DDRH = 0x00;
  DDRK = 0xFF;

  COMWRT4 (0x33); // reset sequence provided by data sheet
  ms_delay (1);
  COMWRT4 (0x32); // reset sequence provided by data sheet
  ms_delay (1);
  COMWRT4 (0x28); // Function set to four bit data length
                  // 2 line, 5 x 7 dot format
  ms_delay (1);
  COMWRT4 (0x06); // entry mode set, increment, (no) shift
  ms_delay (1);
  COMWRT4 (0x0F); // Display set, disp on, cursor on, blink on
  ms_delay (1);

  lcd_clear ();
}

void
lcd_color_mix (Player *player)
{
  unsigned char rgb_sel, lock;
  rgb_sel = 0;
  lock = 1;

  switch (player->id)
    {
    case 1:
      str_split ("PLAYER 1 COLOR");
      break;

    case 2:
      str_split ("PLAYER 2 COLOR");
      break;

    case 3:
      str_split ("PLAYER 3 COLOR");
      break;

    default:
      break;
    }

  update_num (player);
  lcd_update_cursor (rgb_sel);

  while (lock == 1)
    {
      switch (joystick_get_input (player->id))
        {
        case 'u':
          player->color[rgb_sel] += 10;
          update_num (player);
          lcd_update_cursor (rgb_sel);
          break;

        case 'd':
          player->color[rgb_sel] -= 10;
          update_num (player);
          lcd_update_cursor (rgb_sel);
          break;

        case 'l':
          if (rgb_sel == 0)
            rgb_sel = 2;
          else
            rgb_sel--;

          lcd_update_cursor (rgb_sel);
          ms_delay (150);
          break;

        case 'r':
          if (rgb_sel > 1)
            rgb_sel = 0;
          else
            rgb_sel++;

          lcd_update_cursor (rgb_sel);
          ms_delay (150);
          break;

        case 'b':
          lock = 0;
          break;

        default:
          break;
        }
    }
}

void
COMWRT4 (unsigned char command)
{
  unsigned char x;

  x = (command & 0xF0) >> 2; // shift high nibble to center of byte for Pk5-Pk2
  LCD_DATA = LCD_DATA & ~0x3C; // clear bits Pk5-Pk2
  LCD_DATA = LCD_DATA | x;     // sends high nibble to PORTK
  ms_delay (1);
  LCD_CTRL = LCD_CTRL & ~RS; // set RS to command (RS=0)
  ms_delay (1);
  LCD_CTRL = LCD_CTRL | EN; // rais enable
  ms_delay (5);
  LCD_CTRL = LCD_CTRL & ~EN; // Drop enable to capture command
  ms_delay (15);             // wait
  x = (command & 0x0F) << 2; // shift low nibble to center of byte for Pk5-Pk2
  LCD_DATA = LCD_DATA & ~0x3C; // clear bits Pk5-Pk2
  LCD_DATA = LCD_DATA | x;     // send low nibble to PORTK
  LCD_CTRL = LCD_CTRL | EN;    // rais enable
  ms_delay (5);
  LCD_CTRL = LCD_CTRL & ~EN; // drop enable to capture command
  ms_delay (15);
}

void
DATWRT4 (unsigned char data)
{
  unsigned char x;

  x = (data & 0xF0) >> 2;
  LCD_DATA = LCD_DATA & ~0x3C;
  LCD_DATA = LCD_DATA | x;
  ms_delay (1);
  LCD_CTRL = LCD_CTRL | RS;
  ms_delay (1);
  LCD_CTRL = LCD_CTRL | EN;
  ms_delay (1);
  LCD_CTRL = LCD_CTRL & ~EN;
  ms_delay (5);

  x = (data & 0x0F) << 2;
  LCD_DATA = LCD_DATA & ~0x3C;
  LCD_DATA = LCD_DATA | x;
  LCD_CTRL = LCD_CTRL | EN;
  ms_delay (1);
  LCD_CTRL = LCD_CTRL & ~EN;
  ms_delay (15);
}

/*
void
main (void)
{
  Player p1, p2;
  joystick_setup ();
  lcd_setup ();

  p1.id = 1;
  p1.pos = 41;
  p1.color[0] = 255;
  p1.color[1] = 0;
  p1.color[2] = 0;

  p2.id = 2;
  p2.pos = 41;
  p2.color[0] = 255;
  p2.color[1] = 0;
  p2.color[2] = 0;

  lcd_clear ();
  lcd_color_mix (&p1);

  lcd_clear ();
  lcd_color_mix (&p2);

  lcd_clear ();
}
*/
