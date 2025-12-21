/*This is the file that includes:   1.The declarations of all the functonions
                                    2.All The Global Integers externs and statics
                                    3.All the libraries used
*/
#include "raylib.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include<time.h>

#define TIME_LIMIT  180//Number of seconds in which the player has to fulfill all orders.

// Declaring and  initializing constants and other main parameters
extern int WINDOW_WIDTH, WINDOW_HEIGHT;                                  // Size of the window
static const int NUM_OF_RECTANGLES_X = 10, NUM_OF_RECTANGLES_Y = 6;      // Initialize the number of squares created
static const int SIZE_OF_RECTANGLES_X = 100, SIZE_OF_RECTANGLES_Y = 100; // Initialize the number of squares created
static const int MAN_RECTANGLE_WIDTH = 25, MAN_RECTANGLE_HEIGHT = 25;    // Initialize player height and width

extern float speed;//Declare speed of the player as a global external int accessible and modifiable by all functions in all files

// Functions in all files. Syntax of comments is //(FILENAME_WHERE_FUNTCTION_IS_LOCATED) USE_AND_DEFINITION
// Initialization functions
void Initialize_Map(Rectangle (*map)[NUM_OF_RECTANGLES_Y][NUM_OF_RECTANGLES_X]); //(In layout.c) Initialize the map of the square blocks that will constitute the road
// Drawing functions
void draw_pickup_and_dropoff(Vector2 PICKUP, Vector2 DROPOFF);//Draws small circles around dropoff and pickup locations
void DrawRectangles(Rectangle map[NUM_OF_RECTANGLES_Y][NUM_OF_RECTANGLES_X]); // (In draw.c) Draw the array of Rectangles Initialized as map in the main void function to create a map
void Draw_and_update_score_window(int sucessful_deliveries);//(In draw.c)Draws a score window
// Game logic functions
bool check_for_collisions(Rectangle Player, Rectangle map[NUM_OF_RECTANGLES_Y][NUM_OF_RECTANGLES_X]); // (In player_movement.c) Check for collisions between the player and the grid objects
void keep_in_boundaries(Vector2 *pos);                                                                //(In playe_movement)Checks and modifies the pos.x and pos.y if player is out of the window
// UI
Vector2 delta_move(void); //(In player_movement.c) Calculates according to user input the required movement
// Delivery and pickup handling
Vector2 initialize_pickup_location(Rectangle map[NUM_OF_RECTANGLES_Y][NUM_OF_RECTANGLES_X]);                  // Sets pickup location.
Vector2 initialize_dropoff_location(Rectangle map[NUM_OF_RECTANGLES_Y][NUM_OF_RECTANGLES_X], Vector2 PICKUP); // Sets dropoff location.
//Timer limitation functions
int current_timer_difference(time_t INITIAL_TIME);//(In gamehandling.c)Calculates current timer diffence.
void draw_current_timer(int CURRENT_TIME_DIFFERNCE);//(In draw.c)Draws the time diffrence in the UI interface window.