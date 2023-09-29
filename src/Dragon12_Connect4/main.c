#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "input.h"
#include "game.h"
#include "neopixel.h"
#include "lcd.h"

   const unsigned char horizontalLEDs[42] = {0,1,2,3,4,5,6,7,8,9,10,11,
   12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,
   33,34,35,36,37,38,39,40,41};
   const unsigned char verticalLEDs[42] = {0,13,14,27,28,41,40,29,26,15,12,
   1,2,11,16,25,30,39,38,31,24,17,10,3,4,9,18,23,32,37,36,33,22,19,8,5,
   6,7,20,21,34,35};
   const unsigned char diagonal1LEDs[30] = {14,26,30,38,37,31,25,15,13,0,12,16,24,32,36,35,
   33,23,17,11,1,2,10,18,22,34,21,19,9,3};
   const unsigned char diagonal2LEDs[30] = {20,22,32,38,39,31,23,19,7,6,8,18,24,30,40,
   41,29,25,17,9,5,4,10,16,26,28,27,15,11,3};
   
    
void diagonal_check (Player *player, Game *game, unsigned char *led_arr, int len);   
int fillcolumn (Player *player,int top,int x,int max, Game *connect_4);
void vh_check (Player *player, Game *game, unsigned char *led_arr, int len, int maxcount);
//void horizontalcheck(Player *player, Game *game);


void
main (void)
{                                         
  //========================
  // Setup
  //========================
  int i;
  Game connect_4;
  Player p1, p2;
  Player* current_player = &p1; 
   
    /* define positions you want to blink here */ 

  int top = 41; 
  int x = 0;
  int max = 5;
  
  int top2 = 40; 
  int x2 = 6;
  int max2 = 11;
  
  int top3 = 39; 
  int x3 = 12;
  int max3 = 17;
  
  int top4 = 38; 
  int x4 = 18;
  int max4 = 23;
  
  int top5 = 37; 
  int x5 = 24;
  int max5 = 29;
  
  int top6 = 36; 
  int x6 = 30;
  int max6 = 35;
  
  int top7 = 35; 
  int x7 = 36;
  int max7 = 41;
  
 
  
//int current_player = 0;
int COLUMN1; 
int COLUMN2;
int COLUMN3; 
int COLUMN4;
int COLUMN5; 
int COLUMN6;
int COLUMN7;

int lendiag = 30;
int len = 42;
int maxcount = 8;
int maxcountvert = 7;  


  p2.id = 2;
  p2.pos = NUM_PIX - 1; // Start p2 at the top
  p2.color[0] = 0;
  p2.color[1] = 0;
  p2.color[2] = 30;
  
  
  p1.id = 1;
  p1.pos = NUM_PIX - 1; // Start p1 at the top
  p1.color[0] = 30;
  p1.color[1] = 0;
  p1.color[2] = 0;

  

  for (i = 0; i < NUM_PIX; i++)
    connect_4.board[i] = 0;
  
  for (i = 0; i < 3; i++)
	  connect_4.score[i] = 0;
  
  neopixel_setup ();
  joystick_setup ();
  lcd_setup ();  
  
  lcd_clear ();
  lcd_color_mix (&p1);

  lcd_clear ();
  lcd_color_mix (&p2);

  lcd_clear ();

  neopixel_clear ();
  neopixel_set (p2.pos, p2.color[0], p2.color[1], p2.color[2]);
  neopixel_set (p1.pos, p1.color[0], p1.color[1], p1.color[2]);
  neopixel_draw ();
  
  // LCD: print 'Player 1 Go' 
  lcd_clear();
  str_split("PLAYER 1 GO");
  
 
  //========================
  // Loop
  //========================
   /*while (1) {
   
        // Check joystick input and move the LED
        if (joystick_lr_move(current_player) == 0) {
            // Draw the updated LED position
            neopixel_draw();
        }

         COLUMN1 = fillcolumn(current_player,top, x, max, &connect_4);
         COLUMN2 = fillcolumn(current_player,top2,x2,max2, &connect_4);
         COLUMN3 = fillcolumn(current_player,top3,x3,max3, &connect_4);
         COLUMN4 = fillcolumn(current_player,top4,x4,max4, &connect_4);
         COLUMN5 = fillcolumn(current_player,top5,x5,max5, &connect_4);
         COLUMN6 = fillcolumn(current_player,top6,x6,max6, &connect_4);
         COLUMN7 = fillcolumn(current_player,top7,x7,max7, &connect_4);
         
         if (COLUMN1==1||COLUMN2==1){   //||COLUMN3==1||COLUMN4==1||COLUMN5==1||COLUMN6==1||COLUMN7==1){
         current_player = &p2;
         } else if (COLUMN1==2||COLUMN2==2){  //||COLUMN3==2||COLUMN4==2||COLUMN5==2||COLUMN6==2||COLUMN7==2){  
         current_player = &p1;
         }
         
           
         
    }

    
}   */

   while (1) {
        // Check joystick input and move the LED
        if (joystick_lr_move(current_player) == 0) {
            // Draw the updated LED position
            neopixel_draw();
        }

         COLUMN1 = fillcolumn(current_player,top, x, max, &connect_4);
         COLUMN2 = fillcolumn(current_player,top2,x2,max2, &connect_4);
         COLUMN3 = fillcolumn(current_player,top3,x3,max3, &connect_4);
         COLUMN4 = fillcolumn(current_player,top4,x4,max4, &connect_4);
         COLUMN5 = fillcolumn(current_player,top5,x5,max5, &connect_4);
         COLUMN6 = fillcolumn(current_player,top6,x6,max6, &connect_4);
         COLUMN7 = fillcolumn(current_player,top7,x7,max7, &connect_4);
         
            //horizontalcheck(&p1, &game);
            //horizontalcheck(&p2, &game);
            
                if ((COLUMN1==1||COLUMN2==1||COLUMN3==1||COLUMN4==1||COLUMN5==1||
                COLUMN6==1||COLUMN7==1) && current_player == &p1) {
                    //horizontalcheck(&p1, &connect_4);
                    vh_check(&p1, &connect_4, horizontalLEDs, len, maxcount);
                    vh_check(&p1, &connect_4, verticalLEDs, len, maxcountvert);
                    diagonal_check(&p1,&connect_4,diagonal1LEDs,lendiag);
                    diagonal_check(&p1,&connect_4,diagonal2LEDs,lendiag);
                } 
                else if ((COLUMN1==2||COLUMN2==2||COLUMN3==2||COLUMN4==2||COLUMN5==2||
                COLUMN6==2||COLUMN7==2) && current_player == &p2) {
                    //horizontalcheck(&p2, &connect_4);
                     vh_check(&p2, &connect_4, horizontalLEDs, len, maxcount);
                     vh_check(&p2, &connect_4, verticalLEDs, len, maxcountvert);
                     diagonal_check(&p2,&connect_4,diagonal1LEDs,lendiag);
                     diagonal_check(&p2,&connect_4,diagonal2LEDs,lendiag);
                } 
                                                   
         
            if (COLUMN1==1||COLUMN2==1||COLUMN3==1||COLUMN4==1||COLUMN5==1||COLUMN6==1||COLUMN7==1) {
             current_player = &p2;            
             neopixel_reset (p1.pos);
             neopixel_set (p2.pos = 41, p2.color[0], p2.color[1], p2.color[2]);
             neopixel_draw ();

			 // LCD: print 'Player 2 Go' 
             lcd_clear();
			 str_split("PLAYER 2 GO");

            }
            if (COLUMN1==2||COLUMN2==2||COLUMN3==2||COLUMN4==2||COLUMN5==2||COLUMN6==2||COLUMN7==2) {
             current_player = &p1;             
             neopixel_reset (p2.pos);
             neopixel_set (p1.pos = 41, p1.color[0], p1.color[1], p1.color[2]);
             neopixel_draw ();
			 
			 // LCD: print 'Player 1 Go' 
			 lcd_clear();
             str_split("PLAYER 1 GO");
            }
    }
}






