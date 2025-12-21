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
    Color Mycolour = Fade(BLUE, 0.5f);
    DrawRectangle(0, 0, WINDOW_WIDTH, 50, Mycolour);
    char score[15] = {0};
    sprintf(score, "SCORE IS : %d", sucessful_deliveries);
    DrawText(score, 25, 25, 20, WHITE);
}
void draw_current_timer(int CURRENT_TIME_DIFFERNCE)
{
    char time_differnce[20] = {0};//Create an array for storing the time_diffence
    sprintf(time_differnce, "Time left: %d", CURRENT_TIME_DIFFERNCE);//Store the time in the string
    DrawText(time_differnce, 600, 25, 20, WHITE);//Draw
}