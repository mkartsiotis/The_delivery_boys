#include "headers.h"
Node grid[COLS][ROWS] = {0}; // Initialize the Node we declared in the header files.

void Initialize_Map(Rectangle (*map)[NUM_OF_RECTANGLES_Y][NUM_OF_RECTANGLES_X])
{
    // Here we create a map of blocks in a desired area which in this case is the screen window and in later versions will be the whole map.
    float stepX = (float)MAP_WIDTH / (NUM_OF_RECTANGLES_X * 4.0f); // Calculate stepX and StepY in the same way as the screen is spit into cells with the grid.
    float stepY = (float)MAP_HEIGHT / (NUM_OF_RECTANGLES_Y * 4.0f);
    for (int i = 0; i < NUM_OF_RECTANGLES_Y; i++) // Temporary solution:Just split the window in the same way as grid to make it work. Later we will add random generation.
    {                                             // REMEMBER IN RAYLIB A POSITION IS TOP+LEFT!!!!!!
        for (int j = 0; j < NUM_OF_RECTANGLES_X; j++)
        {
            float x_value = (1 + 4 * j) * stepX;
            float y_value = (1 + 4 * i) * stepY;

            (*map)[i][j] = (Rectangle){x_value, y_value, SIZE_OF_RECTANGLES_X, SIZE_OF_RECTANGLES_Y};
        }
    }
}
// Next up we will do some very-very important stuff.
// A.Implement the A* algorithm for finding the fastest route around the blocks
// B.Draw the route
// But first we need to do something much more important. And that is drawing the grid
// More about the A* algorithm and how we are going to implement it you can see in the documentation.
