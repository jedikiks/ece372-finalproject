#ifndef LCD_H_
#define LCD_H_

#include "include/game.h"

/* Player color selector menu */
void lcd_color_mix (Player *player);

/* Clears the LCD, sets its cursor to the
 * beginning of the top row */
void lcd_clear (void);

/* Moves the LCDs cursor given by rgb_sel:
 * 0: Very beginning of the bottom row
 * 1: 5 spaces away from the start of the bottom row
 * 2: 10 spaces away from the start of the bottom row */
void lcd_update_cursor (unsigned char rgb_sel);

/* Initializes the LCD. This needs to be called at the
 * beginning of a program */
void lcd_setup (void);

 // prints a string of chars to LCD
void
str_split (char string[]);

/* Prints the passed character to the LCD */
void lcd_print (unsigned char uchar);

#endif // LCD_H_
