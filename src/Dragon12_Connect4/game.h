#ifndef GAME_H_
#define GAME_H_

#include "neopixel.h"

typedef enum
{
  GAME_START,
  GAME_SEL,
  GAME_MID,
  GAME_PAUSE,
  GAME_WIN,
} GAME_STATE;

typedef struct
{
  unsigned char id;       // Player ID
  unsigned char pos;      // Player's position in LED array. Initialize to 0
  unsigned char color[3]; // Color of the player's chip
} Player;

typedef struct
{
  GAME_STATE state;             // Where we're at
  unsigned char current_player; // Who's turn it is
  unsigned char score[3];                 // Number of matches won, per player
  unsigned char board[NUM_PIX]; // Board dimensions determined in neopixel.h
} Game;

void game_init (void);

#endif // GAME_H_
