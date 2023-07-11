#ifndef NEOPIXEL_H_
#define NEOPIXEL_H_

#define PLAYER_1_COLOR 255, 0, 0
#define PLAYER_2_COLOR 0, 255, 0
#define PLAYER_3_COLOR 0, 0, 255

#define PIX_ROW 1 // LEDs per row: change to 6
#define PIX_COL 8 // LEDs per column: change to 7

/* Initializes each NeoPixel strip defined using PIX_ROW and PIX_COL*/
void neopixel_begin (void);

/* Turns all pixels off */
void neopixel_clear (void);

/* Sets the color of a pixel */
void neopixel_set_color (int x, int y, unsigned char r, unsigned char g,
                         unsigned char b);

/* Sets the color of a pixel, depending on the player */
void neopixel_set_color_player (int x, int y, unsigned char player);

/* Sends updated pixel data to hardware. */
void neopixel_update (void);

#endif // NEOPIXEL_H_
