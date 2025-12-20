/*This is the file that includes:   1.The declarations of all the functonions
                                    2.All The Global Integers externs and statics
                                    3.All the libraries used
*/
#include "raylib.h"
#include <stdbool.h>
#include <stdio.h>

static const int NUM_OF_RECTANGLES_X = 10, NUM_OF_RECTANGLES_Y = 6;       // Initialize the number of squares created
static const int SIZE_OF_RECTANGLES_X = 100, SIZE_OF_RECTANGLES_Y = 100; // Initialize the number of squares created

// Functions in all files. Syntax of comments is //(FILENAME_WHERE_FUNTCTION_IS_LOCATED) USE_AND_DEFINITION
void Initialize_Map(Rectangle (*map)[NUM_OF_RECTANGLES_Y][NUM_OF_RECTANGLES_X]); //(In layout.c) Initialize the map of the square blocks that will constitute the road
void DrawRectangles(Rectangle map[NUM_OF_RECTANGLES_Y][NUM_OF_RECTANGLES_X]);// (In layout.c) Draw the array of Rectangles Initialized as map in the main void function to create a map
