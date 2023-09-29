#include "include/neopixel.h"
#include "include/serial.h"
#include <FastLED.h>

#define LED_PIN     2
#define NUM_LEDS    48

void animation_startup (void); 

CRGB leds[NUM_LEDS];

void animation_startup (void)
{
  delay(10);
  int number =0;
  int number1 =0;
  int number2 =0;
  int number3 =0;
  
 
   leds[0 ] = CRGB(0, 240, 0);
   FastLED.show();
   delay(5);
   leds[1] = CRGB(0, 0, 240);
   FastLED.show();
   delay(5);
   leds[2 ] = CRGB(0, 240, 0);
   FastLED.show();
   delay(5);  
   leds[ 3] = CRGB(0, 0, 240);
   FastLED.show();
   delay(5);
   leds[4] = CRGB(0, 0, 240);
   FastLED.show();
   delay(5);
  
   leds[5 ] = CRGB(240, 0, 0);
   FastLED.show();
   delay(5);  
   leds[6 ] = CRGB(0, 240, 0);
   FastLED.show();
   delay(5);
   leds[7 ] = CRGB(0, 0, 240);
   FastLED.show();
   delay(5);
   
   leds[8 ] = CRGB(240, 0, 0);
   FastLED.show();
   delay(5);  
   leds[9 ] = CRGB(0, 240, 0);
   FastLED.show();
   delay(5);
   leds[10] = CRGB(0, 0, 240);
   FastLED.show();
   delay(5);
   leds[11 ] = CRGB(240, 0, 0);
   FastLED.show();
   delay(5);  
   leds[ 12] = CRGB(0, 240, 0);
   FastLED.show();
   delay(5);
   leds[13] = CRGB(0, 0, 240);
   FastLED.show();
   delay(5);
   leds[14] = CRGB(150, 0, 230);
   FastLED.show();
   delay(5);
   leds[15] = CRGB(230, 200, 20);
   FastLED.show();
   delay(5);
   leds[16] = CRGB(85, 60, 180);
   FastLED.show();
   delay(5);
   leds[17] = CRGB(50, 230, 20);
   FastLED.show();
   delay(5);
  
   leds[18 ] = CRGB(240, 0, 0);
   FastLED.show();
   delay(5);  
   leds[19 ] = CRGB(0, 240, 0);
   FastLED.show();
   delay(5);
   leds[20 ] = CRGB(0, 0, 240);
   FastLED.show();
   delay(5);
    leds[21 ] = CRGB(240, 0, 0);
   FastLED.show();
   delay(5);  
   leds[ 22] = CRGB(0, 240, 0);
   FastLED.show();
   delay(5);
   leds[23] = CRGB(240, 0, 0);
   FastLED.show();
   delay(5);
  
   leds[24 ] = CRGB(240, 0, 0);
   FastLED.show();
   delay(5);  
   leds[25 ] = CRGB(0, 240, 0);
   FastLED.show();
   delay(5);
   leds[26 ] = CRGB(0, 0, 240);
   FastLED.show();
   delay(5);
    leds[27 ] = CRGB(240, 0, 0);
   FastLED.show();
   delay(5);  
   leds[ 28] = CRGB(0, 240, 0);
   FastLED.show();
   delay(5);
   leds[29] = CRGB(0, 0, 240);
   FastLED.show();
   delay(5);
   
   leds[30 ] = CRGB(240, 0, 0);
   FastLED.show();
   delay(5);  
   leds[31 ] = CRGB(0, 240, 0);
   FastLED.show();
   delay(5);
   leds[32] = CRGB(0, 0, 240);
   FastLED.show();
    delay(5);
   leds[33] = CRGB(0, 240,0);
   FastLED.show();
   delay(5);
    leds[34 ] = CRGB(240, 0, 0);
   FastLED.show();
   delay(5);  
   leds[35] = CRGB(0, 240, 0);
   FastLED.show();
   delay(5);
   leds[36] = CRGB(0, 0, 240);
   FastLED.show();
   delay(5);
  
   leds[37 ] = CRGB(240, 0, 0);
   FastLED.show();
   delay(5);  
   leds[38 ] = CRGB(0, 240, 0);
   FastLED.show();
   delay(5);
   leds[39 ] = CRGB(0, 0, 240);
   FastLED.show();
   delay(5);
   leds[40 ] = CRGB(240, 0, 0);
   FastLED.show();
   delay(5);  
   leds[ 41] = CRGB(0, 240, 0);
   FastLED.show();
   delay(5);
   leds[42] = CRGB(0, 0, 240);
   FastLED.show();
   delay(5);
  
   leds[17 ] = CRGB(240, 0, 0);
   FastLED.show();
   delay(5);  
   leds[18 ] = CRGB(0, 240, 0);
   FastLED.show();
   delay(500);
   leds[19 ] = CRGB(0, 0, 240);
   FastLED.show();
   delay(5);
   leds[27 ] = CRGB(0, 240, 0);
   FastLED.show();
   delay(5);  
   leds[ 28] = CRGB(0, 0, 240);
   FastLED.show();
   delay(5);
   leds[29] = CRGB(0, 0, 240);
   FastLED.show();
   delay(5);
  
   leds[37 ] = CRGB(240, 0, 0);
   FastLED.show();
   delay(5);  
   leds[38 ] = CRGB(0, 240, 0);
   FastLED.show();
   delay(5);
   leds[39 ] = CRGB(0, 0, 240);
   FastLED.show();
   delay(5);
   leds[29] = CRGB(0, 0, 0);
   FastLED.show();
   //delay(50);
   leds[28] = CRGB(0, 0, 0);
   FastLED.show();
   delay(2);
   leds[29] = CRGB(0, 0, 240);
   FastLED.show();
   //delay(50);
   leds[28] = CRGB(0, 0, 240);
   FastLED.show();
   delay(10);
   for (int number = 0; number < 45; number++)
     {
       leds[number + 3] = CRGB(0, 0, 0);
       leds[number - 3] = CRGB(0, 0, 0);
    
       delay(10); 
       FastLED.show();
       delay(100);
       leds[number] = CRGB(128, 96, 120);
  
       FastLED.show();
       delay(10);
     }

  for (int number1 = 0; number1 <45; number1++)
    {
      leds[number1] = CRGB(240, 0, 0);
    
      delay(5);
    
      FastLED.show();
      delay(10);
    }

   for (int number3 = 0; number3 < 45; number3++){
    leds[number3] = CRGB(0, 240, 0);
    delay(5);
    FastLED.show();
  delay(10);  
  }

    for (int number2 = 0; number2 < 45; number2++){
    leds[number2] = CRGB(0, 0, 240);
    delay(5);
    FastLED.show();
  delay(10);
  }

   for (int number = 0; number < 45; number++){
    //leds[number] = CRGB(0, 0, 0);
   
    leds[number] = CRGB(120, 96, 120);
    delay(5);
    FastLED.show();
  delay(10);
   }
  
  FastLED.show();
  delay(10);
}


void
setup ()
{
  Serial.begin (9600);
  
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);

  delay(1000);

  animation_startup ();
  neopixel_begin ();
}

void
loop ()
{
  ser_process_rx ();
}

