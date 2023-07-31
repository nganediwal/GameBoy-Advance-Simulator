
#include "main.h"

#include <stdio.h>
#include <stdlib.h>

#include "gba.h"
#include "images/garbage.h"
#include "images/game.h"
#include "images/gameover.h"


enum gba_state {
  START,
  PLAY,
  WIN,
  LOSE,
};

int main(void) {
  REG_DISPCNT = BG2_ENABLE | MODE3; // video mode

  // Save current and previous state of button input.
  u32 previousButtons = BUTTONS;
  u32 currentButtons = BUTTONS;

  // Load initial application state
  enum gba_state state = START;
  int score = 0;
  int goalX = 0;
  int goalY = 0;
  char scoreline[51];

  struct image image;
  
  int curr[2];
  int prev[2] = {0, 0};
  int delta = 5;


  while (1) {
    currentButtons = BUTTONS; // Load the current state of the buttons

    waitForVBlank(); //call wait for vblank before draw


    switch (state) {
      case START:
      waitForVBlank();
      drawFullScreenImageDMA(game); //to start the game
      drawRectDMA(150, 0, 240, 10, YELLOW);
      drawCenteredString(150, 0, 240, 10, "Press START to start", BLACK);
      if (KEY_JUST_PRESSED(BUTTON_START, currentButtons, previousButtons)) {
        state = PLAY;
        goalX = randint(WIDTH - 50, WIDTH - GOAL_WIDTH); //target zone
        goalY = randint(0, HEIGHT - GARBAGE_HEIGHT - 25); //target zone
        fillScreenDMA(BLACK); //remainder of screen to black

        image.x = 10; //initial image hor position
        image.y = HEIGHT - GARBAGE_HEIGHT - 20 - BUFFER_SIZE; //initial image ver position
        curr[0] = image.x;
        curr[1] = image.y;

        score = 0;

        prev[0] = 0;
        prev[1] = 0;
      }
        break;

      case PLAY:
        snprintf(scoreline, 51, "Score: %d", score);
        if (prev[0] != curr[0] || prev[1] != curr[1]) {
          if (prev[0] != curr[0]) {
              if (curr[0] > prev[0]){
                  drawRectDMA(prev[1], prev[0], 5, GARBAGE_HEIGHT, BLACK);
              }
              else {
                  drawRectDMA(curr[1], curr[0] + GARBAGE_WIDTH, 5, GARBAGE_HEIGHT, BLACK);
              }
          }
           else {
              if (curr[1] > prev[1]) {
                drawRectDMA(prev[1], prev[0], GARBAGE_WIDTH, 5, BLACK);
              }
              else {
                drawRectDMA(curr[1] + GARBAGE_HEIGHT, curr[0], GARBAGE_WIDTH, 5, BLACK);
              }
            }
        }

        prev[0] = curr[0];
        prev[1] = curr[1];

        waitForVBlank();
        drawRectDMA(150, 0, 240, 10, GRAY);
        drawRectDMA(140, 0, 240, 10, WHITE);
        drawCenteredString(140, 0, 240, 10, "Press SELECT to exit", BLACK);
        drawCenteredString(150, 0, 240, 10, scoreline, WHITE);
        drawRectDMA(goalY, goalX, GOAL_WIDTH, GOAL_HEIGHT, RED);
        drawRectDMA(goalY + BUFFER_SIZE, goalX + BUFFER_SIZE, 15, 15, BLACK);
        drawImageDMA(image.y, image.x, GARBAGE_WIDTH, GARBAGE_HEIGHT, garbage);

        if (KEY_JUST_PRESSED(BUTTON_SELECT, currentButtons, previousButtons)) {
            state = START;
        }
        if (KEY_JUST_PRESSED(BUTTON_RIGHT, currentButtons, previousButtons)) {
            image.x += delta;
            increaseScore(&score, delta);
        }
        if (KEY_JUST_PRESSED(BUTTON_LEFT, currentButtons, previousButtons)) {
            image.x -= delta;
            increaseScore(&score, delta);
        }
        if (KEY_JUST_PRESSED(BUTTON_UP, currentButtons, previousButtons)) {
          image.y -= delta;
          increaseScore(&score, delta);
        }
        if (KEY_JUST_PRESSED(BUTTON_DOWN, currentButtons, previousButtons)) {
          image.y += delta;
          increaseScore(&score, delta);
        }
        if ((image.y >= goalY && image.y <= goalY + GOAL_HEIGHT && image.x + GARBAGE_WIDTH >= goalX && image.x + GARBAGE_WIDTH <= goalX + GOAL_WIDTH) 
          || (image.y + GARBAGE_HEIGHT >= goalY && image.y + GARBAGE_HEIGHT <= goalY + GOAL_HEIGHT && image.x + GARBAGE_WIDTH >= goalX && image.x 
            + GARBAGE_WIDTH <= goalX + GOAL_WIDTH)){
            delay(5);
            state = WIN;
        }

        if (score > 250){
          state = LOSE;
        }

        curr[0] = image.x;
        curr[1] = image.y;
        break;
      case WIN:
        waitForVBlank();
        drawFullScreenImageDMA(gameover);
        drawRectDMA(150, 0, 240, 10, YELLOW);
        drawCenteredString(150, 0, 240, 10, "You won! Press ENTER to start again", BLACK);
        if (KEY_JUST_PRESSED(BUTTON_START, currentButtons, previousButtons)){
            state = START;
        }
        // state = ?
        break;
      case LOSE:
        waitForVBlank();
        drawFullScreenImageDMA(gameover);
        drawRectDMA(150, 0, 240, 10, YELLOW);
        drawCenteredString(150, 0, 240, 10, "You lost. Press ENTER to start again", BLACK);
        if (KEY_JUST_PRESSED(BUTTON_START, currentButtons, previousButtons)){
            state = START;
        }
        break;
    }

    previousButtons = currentButtons; // Store the current state of the buttons
  }

  return 0;
}

void increaseScore(int *score, int update) {
  *score = *score + update;
}

void delay(int n) {
  //delay for n tenths of a second
  volatile int x = 0;
  for (int i = 0; i < n * 8000; i++)
    x++;
}
