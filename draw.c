#include "headers.h"

void DrawRectangles(Rectangle map[NUM_OF_RECTANGLES_Y][NUM_OF_RECTANGLES_X])
{
    for (int i = 0; i < NUM_OF_RECTANGLES_Y; i++)
        for (int j = 0; j < NUM_OF_RECTANGLES_X; j++)
            DrawRectangle(map[i][j].x, map[i][j].y, SIZE_OF_RECTANGLES_X, SIZE_OF_RECTANGLES_Y, RED);
}

void draw_pickup_and_dropoff(Vector2 PICKUP, Vector2 DROPOFF)
{
    DrawCircle(PICKUP.x, PICKUP.y, 4, YELLOW);
    DrawCircle(DROPOFF.x, DROPOFF.y, 4, GREEN);
}

void Draw_and_update_score_window(int sucessful_deliveries)
{
    Color Mycolour = Fade(BLUE, 0.2ssf);
    DrawRectangle(0, 0, WINDOW_WIDTH, 50, Mycolour);
    char score[15] = {0};
    sprintf(score, "SCORE IS : %d", sucessful_deliveries);
    DrawText(score, 25, 25, 20, WHITE);
}
void draw_current_timer(int CURRENT_TIME_DIFFERNCE)
{
    char time_differnce[20] = {0};                                    // Create an array for storing the time_diffence
    sprintf(time_differnce, "Time left: %d", CURRENT_TIME_DIFFERNCE); // Store the time in the string
    DrawText(time_differnce, 600, 25, 20, WHITE);                     // Draw
}
void draw_grid(void) //(In draw.c)Draws the grid of the big map in world-map coordinates. Note that this function does not draw the lines of the coordinates of the grid[i][j] but the outside sides of the rectangles that represent a 2D division of the map plane.
{
    float stepX = (float)MAP_WIDTH / (4 * NUM_OF_RECTANGLES_X);
    float stepY = (float)MAP_HEIGHT / (4 * NUM_OF_RECTANGLES_Y);
    for (int i = 1; i < 4 * NUM_OF_RECTANGLES_Y; i++) // For every iteration of the number of divisions of the plane:
        DrawLine(0, i * stepY, MAP_WIDTH, i * stepY, YELLOW);

    for (int j = 1; j < 4 * NUM_OF_RECTANGLES_X; j++) // Be carefull!We are starting from 1(If I use the function later)

        DrawLine(j * stepX, 0, j * stepX, MAP_HEIGHT, YELLOW);
}