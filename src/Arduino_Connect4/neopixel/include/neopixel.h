#ifndef NEOPIXEL_H_
#define NEOPIXEL_H_

//====================
// Neopixel pinout
//===================
#define NEOPIXEL_PIN 2

//==================
// Board layout
//==================
/* These values match those found in
 * dragon12/neopixel/include/neopixel.h */
#define NUM_ROW 6
#define NUM_COL 7
#define NUM_PIX (NUM_ROW * NUM_COL)

//==================
// Functions
//==================
/* Initializes each NeoPixel strip */
void neopixel_begin (void);

/* Turns all pixels off */
void neopixel_clear (void);

/* Sets the color of a pixel each given through the
 * serial port. The information passed to the arduino have to be:
 * pixel number, red, green and blue values. Returns a 0 if successful. Returns
 * a 1 if the pixel specified is invalid */
unsigned char neopixel_set (unsigned char pixel, unsigned char r,
                            unsigned char g, unsigned char b);

/* Turns the specified pixel off */
void neopixel_reset (unsigned char pixel);

/* Sends updated pixel data to the hardware */
void neopixel_draw (void);

#endif // NEOPIXEL_H_
