#include "headers.h"
// Initialize external int speed which is vital for movement control
float speed = 4;

float delta_move(void)
{
    float delta = 0;
    if (IsKeyDown(KEY_RIGHT))
    {
        angleRad -= 0.05;
    }
    else if (IsKeyDown(KEY_LEFT))
    {
        angleRad += 0.05;
    }
    if (angleRad > 4 * 1.5707)
        angleRad = 0;
    if (angleRad < -4 * 1.5707)
        angleRad = 0;
    if (IsKeyDown(KEY_DOWN))
    {
        delta = speed;
    }
    else if (IsKeyDown(KEY_UP))
    {
        delta = -speed;
    }
    return delta;
}

void keep_in_boundaries(Vector2 *pos)
{
    if (pos->x < MAN_RECTANGLE_WIDTH / 2.0f)
        pos->x += speed;
    else if (pos->x > WINDOW_WIDTH - (MAN_RECTANGLE_WIDTH / 2.0f))
        pos->x -= speed;
    if (pos->y < MAN_RECTANGLE_HEIGHT / 2.0f)
        pos->y += speed;
    else if (pos->y > WINDOW_HEIGHT - (MAN_RECTANGLE_HEIGHT / 2.0f))
        pos->y -= speed;
}

bool check_for_collisions(Rectangle Player, Rectangle map[NUM_OF_RECTANGLES_Y][NUM_OF_RECTANGLES_X])
{
    bool overlap = false;
    for (int i = 0; i < NUM_OF_RECTANGLES_Y; i++)
        for (int j = 0; j < NUM_OF_RECTANGLES_X; j++)
            if (CheckCollisionRecs(Player, map[i][j]) == 1)
            {
                overlap = true;
                break;
            }
    return overlap;
}
