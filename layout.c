#include "headers.h"

void Initialize_Map(Rectangle (*map)[NUM_OF_RECTANGLES_Y][NUM_OF_RECTANGLES_X])
{
    // Here we create a map of blocks in a desired area which in this case is the screen window and in later versions will be the whole map.
    for (int i = 0; i < NUM_OF_RECTANGLES_Y; i++)
        for (int j = 0; j < NUM_OF_RECTANGLES_X; j++)
        {
            float x_value = (WINDOW_WIDTH * ((float)j + 1) / (NUM_OF_RECTANGLES_X + 1)) - SIZE_OF_RECTANGLES_X / 2.0f;
            float y_value = (WINDOW_HEIGHT * ((float)i + 1) / (NUM_OF_RECTANGLES_Y + 1)) - SIZE_OF_RECTANGLES_Y / 2.0f;

            (*map)[i][j] = (Rectangle){x_value, y_value, SIZE_OF_RECTANGLES_X, SIZE_OF_RECTANGLES_Y};
        }
}

bool check_for_collisions(Rectangle Player, Rectangle map[NUM_OF_RECTANGLES_Y][NUM_OF_RECTANGLES_X])
{
    bool overlap = false;
    for (int i = 0; i < NUM_OF_RECTANGLES_Y; i++)
        for (int j = 0; j < NUM_OF_RECTANGLES_X; j++)
            if (CheckCollisionRecs(Player, map[i][j]) == 1)
            {
                overlap = true;
                break;
            }
    return overlap;
}
