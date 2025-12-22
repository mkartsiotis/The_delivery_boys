#include "headers.h"

int grid[4 * NUM_OF_RECTANGLES_Y][4 * NUM_OF_RECTANGLES_X] = {0}; // Initialize the grid array

void Initialize_Map(Rectangle (*map)[NUM_OF_RECTANGLES_Y][NUM_OF_RECTANGLES_X])
{

                                                                   // Here we create a map of blocks in a desired area which in this case is the screen window and in later versions will be the whole map.
    float stepX = (float)MAP_WIDTH / (NUM_OF_RECTANGLES_X * 4.0f); // Calculate stepX and StepY in the same way as the screen is spit into cells with the grid.
    float stepY = (float)MAP_HEIGHT / (NUM_OF_RECTANGLES_Y * 4.0f);
    for (int i = 0; i < NUM_OF_RECTANGLES_Y; i++) // Temporary solution:Just split the window in the same way as grid to make it work. Later we will add random generation.        
    {//REMEMBER IN RAYLIB A POSITION IS TOP+LEFT!!!!!!
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
void initialize_grid(void) // Splits the map into a grid for debugging.
{
    // For now we'll just use the same algorithm as above later we will improove by letting the above algorithm know the rectangles' existence from the grid.
    for (int i = 0; i < 4 * NUM_OF_RECTANGLES_Y; i++)
        for (int j = 0; j < 4 * NUM_OF_RECTANGLES_X; j++)
        {
            if (((i - 1) % 4 == 0 || (i - 2) % 4 == 0) && ((j - 1) % 4 == 0 || (j - 2) % 4 == 0))
                grid[i][j] = 1; // Set to 1 only the cells that are inside a rectangle(see docs)
        }
    return;
}