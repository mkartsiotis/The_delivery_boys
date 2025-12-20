#include "headers.h"

void DrawRectangles(Rectangle map[NUM_OF_RECTANGLES_Y][NUM_OF_RECTANGLES_X])
{
    for (int i = 0; i < NUM_OF_RECTANGLES_X; i++)
        for (int j = 0; j < NUM_OF_RECTANGLES_Y; j++)
            DrawRectangle(map[i][j].x,map[i][j].y, SIZE_OF_RECTANGLES_X, SIZE_OF_RECTANGLES_Y, RED);
}

