#include "headers.h"

enum Sides
{
    TOP1,
    TOP2,
    BOTTOM1,
    BOTTOM2,
    LEFT1,
    LEFT2,
    RIGHT1,
    RIGHT2
};

int score_for_current_mission = 0; // Variable that is responsible for storing the score for a mission.
float gas = (float)INITIAL_GASOLINE;

grid_and_map_coords initialize_pickup_location(Rectangle map[NUM_OF_RECTANGLES_Y][NUM_OF_RECTANGLES_X])
{
    grid_and_map_coords coordinates;
    enum Sides PICKUP_SIDE;
    int block_x, block_y;
    srand(time(NULL));
    block_x = rand() % NUM_OF_RECTANGLES_X;
    block_y = rand() % NUM_OF_RECTANGLES_Y;
    PICKUP_SIDE = rand() % 8;
    (coordinates.REAL).x = map[block_y][block_x].x;
    (coordinates.REAL).y = map[block_y][block_x].y;
    if (PICKUP_SIDE == TOP2)
    {
        (coordinates.REAL).x += 2 * SIZE_OF_RECTANGLES_X / 3.0f;
        coordinates.grid_x = block_x * 4 + 2; // We know we owe you an explanation...But in short this is the conversion algorithm(See docs).
        coordinates.grid_y = block_y * 4;
    }
    if (PICKUP_SIDE == TOP1)
    {
        (coordinates.REAL).x += SIZE_OF_RECTANGLES_X / 3.0f;
        coordinates.grid_x = block_x * 4 + 1; // Similar to before(See docs).
        coordinates.grid_y = block_y * 4;
    }
    if (PICKUP_SIDE == BOTTOM1)
    {
        (coordinates.REAL).x += SIZE_OF_RECTANGLES_X / 3.0f;
        (coordinates.REAL).y += SIZE_OF_RECTANGLES_Y;
        coordinates.grid_x = block_x * 4 + 1; // Similar to before(See docs).
        coordinates.grid_y = block_y * 4 + 3;
    }
    if (PICKUP_SIDE == BOTTOM2)
    {
        (coordinates.REAL).x += 2 * SIZE_OF_RECTANGLES_X / 3.0f;
        (coordinates.REAL).y += SIZE_OF_RECTANGLES_Y;
        coordinates.grid_x = block_x * 4 + 2; // Similar to before(See docs).
        coordinates.grid_y = block_y * 4 + 3;
    }
    if (PICKUP_SIDE == LEFT1)
    {
        (coordinates.REAL).y += SIZE_OF_RECTANGLES_Y / 3.0f;
        coordinates.grid_x = block_x * 4; // Similar to before(See docs).
        coordinates.grid_y = block_y * 4 + 1;
    }
    if (PICKUP_SIDE == LEFT2)
    {
        (coordinates.REAL).y += 2 * SIZE_OF_RECTANGLES_Y / 3.0f;
        coordinates.grid_x = block_x * 4; // Similar to before(See docs).
        coordinates.grid_y = block_y * 4 + 2;
    }
    if (PICKUP_SIDE == RIGHT1)
    {
        (coordinates.REAL).x += SIZE_OF_RECTANGLES_X;
        (coordinates.REAL).y += SIZE_OF_RECTANGLES_Y / 3.0f;
        coordinates.grid_x = block_x * 4 + 3; // Similar to before(See docs).
        coordinates.grid_y = block_y * 4 + 1;
    }
    if (PICKUP_SIDE == RIGHT2)
    {
        (coordinates.REAL).x += SIZE_OF_RECTANGLES_X;
        (coordinates.REAL).y += 2 * SIZE_OF_RECTANGLES_Y / 3.0f;
        coordinates.grid_x = block_x * 4 + 3; // Similar to before(See docs).
        coordinates.grid_y = block_y * 4 + 2;
    }

    return coordinates;
}
grid_and_map_coords initialize_dropoff_location(Rectangle map[NUM_OF_RECTANGLES_Y][NUM_OF_RECTANGLES_X], Vector2 PICKUP)
{
    grid_and_map_coords coordinates;
    enum Sides DROPOFF_SIDE;
    int block_x, block_y;
    block_x = rand() % NUM_OF_RECTANGLES_X;
    block_y = rand() % NUM_OF_RECTANGLES_Y;

    DROPOFF_SIDE = rand() % 4;
    (coordinates.REAL).x = map[block_y][block_x].x;
    (coordinates.REAL).y = map[block_y][block_x].y;
    if (DROPOFF_SIDE == TOP2)
    {
        (coordinates.REAL).x += 2 * SIZE_OF_RECTANGLES_X / 3.0f;
        coordinates.grid_x = block_x * 4 + 2; // We know we owe you an explanation...But in short this is the conversion algorithm(See docs).
        coordinates.grid_y = block_y * 4;
    }
    if (DROPOFF_SIDE == TOP1)
    {
        (coordinates.REAL).x += SIZE_OF_RECTANGLES_X / 3.0f;
        coordinates.grid_x = block_x * 4 + 1; // Similar to before(See docs).
        coordinates.grid_y = block_y * 4;
    }
    if (DROPOFF_SIDE == BOTTOM1)
    {
        (coordinates.REAL).x += SIZE_OF_RECTANGLES_X / 3.0f;
        (coordinates.REAL).y += SIZE_OF_RECTANGLES_Y;
        coordinates.grid_x = block_x * 4 + 1; // Similar to before(See docs).
        coordinates.grid_y = block_y * 4 + 3;
    }
    if (DROPOFF_SIDE == BOTTOM2)
    {
        (coordinates.REAL).x += 2 * SIZE_OF_RECTANGLES_X / 3.0f;
        (coordinates.REAL).y += SIZE_OF_RECTANGLES_Y;
        coordinates.grid_x = block_x * 4 + 2; // Similar to before(See docs).
        coordinates.grid_y = block_y * 4 + 3;
    }
    if (DROPOFF_SIDE == LEFT1)
    {
        (coordinates.REAL).y += SIZE_OF_RECTANGLES_Y / 3.0f;
        coordinates.grid_x = block_x * 4; // Similar to before(See docs).
        coordinates.grid_y = block_y * 4 + 1;
    }
    if (DROPOFF_SIDE == LEFT2)
    {
        (coordinates.REAL).y += 2 * SIZE_OF_RECTANGLES_Y / 3.0f;
        coordinates.grid_x = block_x * 4; // Similar to before(See docs).
        coordinates.grid_y = block_y * 4 + 2;
    }
    if (DROPOFF_SIDE == RIGHT1)
    {
        (coordinates.REAL).x += SIZE_OF_RECTANGLES_X;
        (coordinates.REAL).y += SIZE_OF_RECTANGLES_Y / 3.0f;
        coordinates.grid_x = block_x * 4 + 3; // Similar to before(See docs).
        coordinates.grid_y = block_y * 4 + 1;
    }
    if (DROPOFF_SIDE == RIGHT2)
    {
        (coordinates.REAL).x += SIZE_OF_RECTANGLES_X;
        (coordinates.REAL).y += 2 * SIZE_OF_RECTANGLES_Y / 3.0f;
        coordinates.grid_x = block_x * 4 + 3; // Similar to before(See docs).
        coordinates.grid_y = block_y * 4 + 2;
    }
    // If we see that dropoff = pickup we just run the function again
    while ((coordinates.REAL).x == PICKUP.x && (coordinates.REAL).y == PICKUP.y)
    {
        initialize_dropoff_location(map, PICKUP);
    }
    return coordinates;
}

void burn_fuel(void)                       //(In gamehandling.c)Decreases the fuel amount.
{
    gas -= speed / 50;//This formula calculates the amount of fuel that is burnt in every frame. 
}

void set_score_for_current_mission(Vector2 pos, Vector2 PICKUP, Vector2 DROPOFF) // Calculates the score for a given mission and assigns it to the global external variable score_for_current_mission.
{
    float points_award = 0;
    points_award = (fabs(pos.x - PICKUP.x) + fabs(pos.y - PICKUP.y) + fabs(DROPOFF.x - PICKUP.x) + fabs(DROPOFF.y - PICKUP.y)) / 10.0f;
    score_for_current_mission = (int)points_award; // Assign the score to the global variable score_for_current_mission
}
void deduce_score_for_mission(int n) // Deduces score for current mission when called in main(for time deduction and punishment when hitting a car or when doing something illegal)
{
    if (score_for_current_mission > 0)
        score_for_current_mission -= n; // Probably an unecessary function but ok...
}