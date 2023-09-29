#ifndef SERIAL_H_
#define SERIAL_H_

// Uncomment this to print stuff to the serial monitor:
#define DEBUG

/* Reads nbytes into data from the serial buffer. Returns a 0 upon
 * successful completion. Returns a 1 if the number of bytes in the serial
 * buffer is greater than nbytes*/
unsigned char ser_rx_bytes (unsigned char *data, int nbytes);

/* Processes any data found in the serial buffer. Returns 0 upon successful
 * completion. Returns a 1 if no valid input was found  */
unsigned char ser_process_rx (void);

#endif // SERIAL_H_
