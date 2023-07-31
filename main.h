#ifndef MAIN_H
#define MAIN_H

#include "gba.h"

// TODO: Create any necessary structs

/*
* For example, for a Snake game, one could be:
*
* struct snake {
*   int heading;
*   int length;
*   int row;
*   int col;
* };
*
*/

#define GOAL_WIDTH 25
#define GOAL_HEIGHT 40
#define BUFFER_SIZE 5

struct image{
    int x;
    int y;
};

void increaseScore(int *score, int delta);
void delay(int n);


#endif
