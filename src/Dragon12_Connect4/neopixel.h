#ifndef NEOPIXEL_H_
#define NEOPIXEL_H_

//========================
// Board dimensions
//========================
/* Both of these must be defined and match
 * values in /arduino/include/neopixel.h */
#define PIX_COL 7
#define PIX_ROW 6

#define NUM_PIX (PIX_COL * PIX_ROW)

//===============
// Functions
//===============
/* Sets up SCI ports. Must be called once and only once */
void neopixel_setup (void);

/* Sets a pixel to a color given by red, green and blue values */
void neopixel_set (unsigned char pixel, unsigned char r, unsigned char g,
                   unsigned char b);

/* Turns a pixel off */
void neopixel_reset (unsigned char pixel);

/* Updates each pixel with new RGB values */
void neopixel_draw (void);

/* Turns all pixels off */
void neopixel_clear (void);

void sercom_send (unsigned char data);


#endif // NEOPIXEL_H_
