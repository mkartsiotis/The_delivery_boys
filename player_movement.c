#include "headers.h"
// Initialize external int speed which is vital for movement control
float speed = 2;

float delta_move(void)
{
    // Now lets create a speed altering logic based on the time the throttle is pressed.
    float delta = 0;
    if (IsKeyDown(KEY_RIGHT))
    {
        angleRad -= 0.03;
    }
    else if (IsKeyDown(KEY_LEFT))
    {
        angleRad += 0.03;
    }
    if (angleRad > 4 * 1.5707)
        angleRad = 0;
    if (angleRad < -4 * 1.5707)
        angleRad = 0;
    if (IsKeyDown(KEY_DOWN))
    {
        delta = 0;
        speed -= 0.1;
    }
    else if (IsKeyDown(KEY_UP))
    {
        speed += 0.03;
    }
    else if (speed > 0)
        speed -= 0.03;
    if (speed < 0)
        speed = 0;
    if (speed > 3)
        speed = 3;
    delta = -speed;
    return delta;
}

void keep_in_boundaries(Vector2 *pos)
{
    if (pos->x - MAN_RECTANGLE_WIDTH / 2.0f < 0)
    {
        float diff = pos->x - MAN_RECTANGLE_WIDTH / 2.0f;
        pos->x -= diff;
    }
    else if (pos->x > WINDOW_WIDTH - (MAN_RECTANGLE_WIDTH / 2.0f))
    {
        float diff = pos->x + MAN_RECTANGLE_WIDTH / 2.0f - WINDOW_WIDTH;
        pos->x -= diff;
    }
    if (pos->y < MAN_RECTANGLE_HEIGHT / 2.0f)
    {
        float diff = pos->y - MAN_RECTANGLE_HEIGHT / 2.0f;
        pos->y -= diff;
    }
    else if (pos->y > WINDOW_HEIGHT - (MAN_RECTANGLE_HEIGHT / 2.0f))
    {
        float diff = pos->y + MAN_RECTANGLE_HEIGHT / 2.0f - WINDOW_HEIGHT;
        pos->y -= diff;
    }
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
