#include "include/serial.h"
#include "include/neopixel.h"

#include <Arduino.h>
#include <stdlib.h>
#include <sys/types.h>

unsigned char
ser_rx_bytes (unsigned char *data, int nbytes)
{
  int status = 0;

  while (Serial.available () < 0)
    ;
  int rx_flen = Serial.readBytes (data, nbytes);

  if (rx_flen > 0)
    {
#ifdef DEBUG
      Serial.print ("\n**INFO => number of bytes in this frame: ");
      Serial.println (rx_flen, HEX);

      Serial.print ("**INFO => data retrieved: ");
      for (int i = 0; i < rx_flen; i++)
        Serial.print (data[i], HEX);
      Serial.print ("\n");
#endif
    }

  if (rx_flen > nbytes)
    {
      Serial.println ("\n**WARNING => number of bytes in frame is greater "
                      "than bytes pulled");
      return 1;
    }
  return status;
}

unsigned char
ser_process_rx (void)
{
  unsigned char status = 0;

  unsigned char cmd;
  ser_rx_bytes (&cmd, 1);

  switch (cmd)
    {
    case 'A':
      unsigned char data_set[4];
      ser_rx_bytes (data_set, 4);
      neopixel_set (data_set[0], data_set[1], data_set[2], data_set[3]);
      break;

    case 'B':
      unsigned char data_reset;
      ser_rx_bytes (&data_reset, 1);
      neopixel_reset (data_reset);
      break;

    case 'C':
      neopixel_draw ();
      break;

    case 'D':
      neopixel_clear ();
      break;

    default:
      status = 1;
      break;
    }

  return status;
}
