#include "headers.h"

void DrawRectangles(Rectangle map[NUM_OF_RECTANGLES_Y][NUM_OF_RECTANGLES_X])
{
    for (int i = 0; i < NUM_OF_RECTANGLES_Y; i++)
        for (int j = 0; j < NUM_OF_RECTANGLES_X; j++)
            DrawRectangle(map[i][j].x,map[i][j].y, SIZE_OF_RECTANGLES_X, SIZE_OF_RECTANGLES_Y, RED);
}
void draw_pickup_and_dropoff(Vector2 PICKUP, Vector2 DROPOFF)
{
    DrawCircle(PICKUP.x, PICKUP.y, 4, YELLOW);
    DrawCircle(DROPOFF.x, DROPOFF.y, 4, GREEN);
}
