/*This is the file that includes:   1.The declarations of all the functonions
                                    2.All The Global Integers externs and statics
                                    3.All the libraries used
*/
#include "raylib.h"
#include "raymath.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h> //for the A* and especiallly for the heuristic function h(n) that will be used.
// Notation:
// For moving objects:
// First we name what we want(e.g. SIZE). All words seperated by '_'.See why we opted to use #DEFINE in the documentatetion
//  Define some preset constatnts
#define TIME_LIMIT 180 // Number of seconds in which the player has to fulfill all orders.
#define WINDOW_WIDTH 1900
#define WINDOW_HEIGHT 1000
#define MAP_WIDTH WINDOW_WIDTH
#define MAP_HEIGHT WINDOW_HEIGHT

#define NUM_OF_RECTANGLES_X 15
#define NUM_OF_RECTANGLES_Y 6
#define COLS (4 * NUM_OF_RECTANGLES_X) // 60
#define ROWS (4 * NUM_OF_RECTANGLES_Y) // 24

#define SIZE_OF_RECTANGLES_X (MAP_WIDTH / (2.0f * NUM_OF_RECTANGLES_X))
#define SIZE_OF_RECTANGLES_Y (MAP_HEIGHT / (2.0f * NUM_OF_RECTANGLES_Y))
#define SIZE_OF_RECTANGLES_3DHEIGHT 50

#define MINIMAP_WIDTH 400
#define MINIMAP_HEIGHT 200

#define NPC_SMART_DELAY 100
#define SIZE_OF_CAR_X 5
#define SIZE_OF_CAR_Y 5
#define SIZE_OF_CAR_Z 4
#define NUM_OF_NPC_CARS_ON_X_ROAD 10
#define NUM_OF_NPC_CARS_ON_Y_ROAD 10
// In this section we define all the structure and types needed
typedef struct // This is a type that we use to store the A* results.
{
    int result;                     // This is where we are going to encode the successful or not result of the A* and use it for debugging messages.
    int MATRIX_OUT[ROWS * COLS][2]; // Which is the max Number of poins????We have in total ROWS X COLUMNS - OBSTACLES.MULTIPLY BY 4 since printing has the format (%d,%d)- LETS put more for safety!
    int number_of_points;           // Returns the number of repetitions and rows in the MATRIX_OUT that need to be read
} best_possible_path;
// Define a Node Structure
//  This holds all the necessary stats for a single tile on the grid
typedef struct
{
    int x, y;             // Coordinate on the grid
    int parentX, parentY; // Parent coordinates (to trace the path back)
    double g;             // Cost from start to here
    double h;             // Heuristic (estimated cost to goal)
    double f;             // Total cost (g + h)
    bool isObstacle;      // Is this a wall?
    bool isClosed;        // Have we already checked this node?
    bool isOpen;          // Is this node currently in the "To-Do" list?
} Node;
/*
Implementing the first step to create straight line moving NPC's.(quick reminder of the step)
1. An array of special type(typedefined in the header file) where all the precious data for the npc will be stored and namely:
    -Current state(active or passive)
    -Current coordinates
    -Start position and end position coordiantes
    -Size for drawing. SizeX, sizeY, sizeZ.
    -Colour for drawing.*/
typedef struct
{
    bool is_visible;         // Visibility control for update and print logic use
    Vector2 pos;             // position of the NPC car
    Vector2 start_pos;       // starting position of the car
    Vector2 end_pos;         // Final position of the car before it becomes invisible
    int sizeX, sizeY, sizeZ; // Size of the NPC car defined with #DEFINE(check above)
    Color col;               // colour of the NPC car
} npc_car;                   // This is our object type.

// We are going to need a structure for communication between world and grid coordinates. Used in the return value of the set_pickup_and_dropoof_location.
typedef struct
{
    Vector2 REAL;
    int grid_x;
    int grid_y;
} grid_and_map_coords; // please do not judge me...Lack of fantasy in the name creation:)
// This is the structure used by functions that return grid coordinates.
typedef struct
{
    int gridX;
    int gridY;
} grid_coordinates;
// NPC structure
typedef struct
{
    Vector2 position;
    int WIDTH;
    int HEIGHT;
    float speed;
} NPC;
// This is a structure that is used to store the npc vehicle data.

// This is used in the main for the main screen, the before and after of the game.
enum Screen
{
    PREVIEW,
    GAMEON,
    GAMEOVER
};
// Global Grid of Nodes
extern Node grid[COLS][ROWS]; // We declare grid as external and initialize it in layout.c

// Declaring and  initializing constants and other main parameters
static const int MAN_RECTANGLE_WIDTH = 5, MAN_RECTANGLE_HEIGHT = 5; // Initialize player height and width
static const int MAN_3D_HEIGHT = 4;                                 // Height of player

extern float speed; // Declare speed of the player as a global external int accessible and modifiable by all functions in all files

extern int npc_smart_counter;                                                                                                                          // This is the counter variable that tracks the calls of the updateNPC function.
extern Vector2 target_npc_pos;                                                                                                                         // This is the old pos used in the update NPC and is the NPC target pos
static const Vector2 NPC_CAR_CEMETARY = (Vector2){-2000, -2000};                                                                                       // Set the place where all the invisible cars will stay and this is used primarily for initialization purposes(might very well be unnecessary)
extern npc_car cars_vertical[NUM_OF_RECTANGLES_X + 1][NUM_OF_NPC_CARS_ON_Y_ROAD], cars_horizontal[NUM_OF_RECTANGLES_Y + 1][NUM_OF_NPC_CARS_ON_X_ROAD]; // Create an array of cars for the X and Y axis respectively.
// Functions in all files. Syntax of comments is //(FILENAME_WHERE_FUNTCTION_IS_LOCATED) USE_AND_DEFINITION
// Initialization functions
void Initialize_Map(Rectangle (*map)[NUM_OF_RECTANGLES_Y][NUM_OF_RECTANGLES_X]); //(In layout.c) Initialize the map of the square blocks that will constitute the road

// Drawing functions
void draw_pickup_and_dropoff(Vector2 PICKUP, Vector2 DROPOFF);                // Draws small circles around dropoff and pickup locations
void draw_pickup_and_dropoff3D(Vector2 PICKUP, Vector2 DROPOFF);              // Draws small circles around dropoff and pickup locations
void DrawRectangles(Rectangle map[NUM_OF_RECTANGLES_Y][NUM_OF_RECTANGLES_X]); // (In draw.c) Draw the array of Rectangles Initialized as map in the main void function to create a map
void Draw_and_update_score_window(int sucessful_deliveries);                  //(In draw.c)Draws a score window
void draw_astar_results(best_possible_path A_STAR_RESULT);                    // Draws A* results in 2D.
void draw_astar_results3D(best_possible_path A_STAR_RESULT);                  // Draws A* results in 3D.
void draw_npc(NPC chaser);                                                    // Draws an NPC
void drawspeed(void);                                                         //(in draw.c). Draws a speedometer.
// Game logic functions
bool check_for_collisions(Rectangle Player, Rectangle map[NUM_OF_RECTANGLES_Y][NUM_OF_RECTANGLES_X]); // (In player_movement.c) Check for collisions between the player and the grid objects
void keep_in_boundaries(Vector2 *pos);                                                                //(In playe_movement)Checks and modifies the pos.x and pos.y if player is out of the window
// For rotation logic
extern double angleRad; // Initialized in cam.c
// UI
float delta_move(void); //(In player_movement.c) Calculates according to user input the required movement
// Delivery and pickup handling
grid_and_map_coords initialize_pickup_location(Rectangle map[NUM_OF_RECTANGLES_Y][NUM_OF_RECTANGLES_X]);                  // Sets pickup location.
grid_and_map_coords initialize_dropoff_location(Rectangle map[NUM_OF_RECTANGLES_Y][NUM_OF_RECTANGLES_X], Vector2 PICKUP); // Sets dropoff location.
// Timer limitation functions
int current_timer_difference(time_t INITIAL_TIME);                       //(In gamehandling.c)Calculates current timer diffence.
void draw_current_timer(int CURRENT_TIME_DIFFERNCE);                     //(In draw.c)Draws the time diffrence in the UI interface window.
void draw_grid(void);                                                    //(In draw.c)Draws the grid of the big map in world-map coordinates. Note that this function does not draw the lines of the coordinates of the grid[i][j] but the outside sides of the rectangles that represent a 2D division of the map plane.
void DrawCubes(Rectangle map[NUM_OF_RECTANGLES_Y][NUM_OF_RECTANGLES_X]); // Draws the cubes for the 3D version.
void draw_npc3D(NPC chaser);                                             // Draws the NPC in 3d
void draw_cars(void);//Draws all the cars.
// A* functions
//  Initialize all functions(All in astar_search.c)
void initGrid(void);                                                          // Initializes the grid that will be used for finding the best path
void CreateWalls(void);                                                       // Sets the walls where they need to be.
double calculateH(int x, int y, int destX, int destY);                        // Calculates heuristic function. More abouth the A* algorith in the docs.
bool isValid(int x, int y);                                                   // Check validity of coordinates given
best_possible_path aStarSearch(int startX, int startY, int destX, int destY); // Output the A* results
grid_coordinates RealToGrid(Vector2 pos);                                     //(In astar_search.c) converts real map coordinates to grid coordinates for a_star_search. Remember pos is the center position not the top-left.
Vector2 GridToReal(int gridX, int gridY);                                     //(In astar_search.c) converts grid coordinates to real map ones.
// This is the set of functions used in the NPC creation and movement
void updateNPC(NPC *chaser, Vector2 player_pos, Rectangle map[NUM_OF_RECTANGLES_Y][NUM_OF_RECTANGLES_X]);                                                                       //(In npc.c)Checks conditions and recalculates path if needed.
int check_if_caught(Vector2 playerpos, NPC npc);                                                                                                                                //(Inn npc.c)Checks if they come in contact
void update_npc_cars(void); //(In npc.c)Controls npc car movement and specifically APPEARNCE - DISAPPEARENCE - POSITION - COLOUR - START AND END POSITION.
void init_cars(void);//Initializes the array of cars
// Camera logic(IN cam.c file)
void TurnCam(Camera3D *camera3d, Vector2 pos); // Turns 3D cam.
