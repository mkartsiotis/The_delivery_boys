#include "headers.h"
enum Sides
{
    TOP,
    BOTTOM,
    LEFT,
    RIGHT
};
Vector2 initialize_pickup_location(Rectangle map[NUM_OF_RECTANGLES_Y][NUM_OF_RECTANGLES_X])
{
    Vector2 coordinates;
    enum Sides PICKUP_SIDE;
    int block_x, block_y;
    srand(time(NULL));
    block_x = rand() % NUM_OF_RECTANGLES_X;
    block_y = rand() % NUM_OF_RECTANGLES_Y;
    PICKUP_SIDE = rand() % 4;
    coordinates.x = map[block_y][block_x].x;
    coordinates.y = map[block_y][block_x].y;
    if (PICKUP_SIDE == TOP)
    {
        coordinates.x += SIZE_OF_RECTANGLES_X / 2.0f;
    }
    if (PICKUP_SIDE == BOTTOM)
    {
        coordinates.x += SIZE_OF_RECTANGLES_X / 2.0f;
        coordinates.y += SIZE_OF_RECTANGLES_Y;
    }
    if (PICKUP_SIDE == LEFT)
    {
        coordinates.y += SIZE_OF_RECTANGLES_Y / 2.0f;
    }
    if (PICKUP_SIDE == RIGHT)
    {
        coordinates.x += SIZE_OF_RECTANGLES_X;
        coordinates.y += SIZE_OF_RECTANGLES_Y / 2.0f;
    }
    return coordinates;
}
Vector2 initialize_dropoff_location(Rectangle map[NUM_OF_RECTANGLES_Y][NUM_OF_RECTANGLES_X], Vector2 PICKUP)
{
    Vector2 coordinates;
    enum Sides DROPOFF_SIDE;
    int block_x, block_y;
    block_x = rand() % NUM_OF_RECTANGLES_X;
    block_y = rand() % NUM_OF_RECTANGLES_Y;
    
    DROPOFF_SIDE = rand() % 4;
    coordinates.x = map[block_y][block_x].x;
    coordinates.y = map[block_y][block_x].y;
    if (DROPOFF_SIDE == TOP)
    {
        coordinates.x += SIZE_OF_RECTANGLES_X / 2.0f;
    }
    if (DROPOFF_SIDE == BOTTOM)
    {
        coordinates.x += SIZE_OF_RECTANGLES_X / 2.0f;
        coordinates.y += SIZE_OF_RECTANGLES_Y;
    }
    if (DROPOFF_SIDE == LEFT)
    {
        coordinates.y += SIZE_OF_RECTANGLES_Y / 2.0f;
    }
    if (DROPOFF_SIDE == RIGHT)
    {
        coordinates.x += SIZE_OF_RECTANGLES_X;
        coordinates.y += SIZE_OF_RECTANGLES_Y / 2.0f;
    }
    //If we see that dropoff = pickup we just run the function again
    while (coordinates.x == PICKUP.x && coordinates.y == PICKUP.y)
    {
        initialize_dropoff_location(map, PICKUP);
    }
    return coordinates;

}

int current_timer_difference(time_t INITIAL_TIME)
{
    time_t END_TIME = INITIAL_TIME + TIME_LIMIT;
    time_t current_time = time(NULL);
    int seconds_passed = (int)difftime(END_TIME, current_time);
    return seconds_passed;
}
