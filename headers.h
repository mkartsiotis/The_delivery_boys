/*This is the file that includes:   1.The declarations of all the functonions
                                    2.All The Global Integers externs and statics
                                    3.All the libraries used
*/
#include "raylib.h"
#include <stdbool.h>
#include <stdio.h>

extern int WINDOW_WIDTH, WINDOW_HEIGHT;//Size of the window
static const int NUM_OF_RECTANGLES_X = 10, NUM_OF_RECTANGLES_Y = 6;       // Initialize the number of squares created
static const int SIZE_OF_RECTANGLES_X = 100, SIZE_OF_RECTANGLES_Y = 100; // Initialize the number of squares created
static const int MAN_RECTANGLE_WIDTH = 25, MAN_RECTANGLE_HEIGHT = 25; //Initialize player height and width


extern float speed;

// Functions in all files. Syntax of comments is //(FILENAME_WHERE_FUNTCTION_IS_LOCATED) USE_AND_DEFINITION
//Initialization functions
void Initialize_Map(Rectangle (*map)[NUM_OF_RECTANGLES_Y][NUM_OF_RECTANGLES_X]); //(In layout.c) Initialize the map of the square blocks that will constitute the road
//Drawing functions
void DrawRectangles(Rectangle map[NUM_OF_RECTANGLES_Y][NUM_OF_RECTANGLES_X]);// (In draw.c) Draw the array of Rectangles Initialized as map in the main void function to create a map
//Game logic functions
bool check_for_collisions(Rectangle Player, Rectangle map[NUM_OF_RECTANGLES_Y][NUM_OF_RECTANGLES_X]);// (In player_movement.c) Check for collisions between the player and the grid objects
void keep_in_boundaries(Vector2 *pos);//(In playe_movement)Checks and modifies the pos.x and pos.y if player is out of the window
//UI
Vector2 delta_move(void);//(In player_movement.c)