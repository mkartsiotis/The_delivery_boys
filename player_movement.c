#include "headers.h"
// Initialize external int speed which is vital for movement control
float speed = 4;

Vector2 delta_move(void)
{
    Vector2 delta = {0, 0};
    if (IsKeyDown(KEY_RIGHT))
    {
        delta.x = speed;
    }
    else if (IsKeyDown(KEY_LEFT))
    {
        delta.x = -speed;
    }
    if (IsKeyDown(KEY_DOWN))
    {
        delta.y = speed;
    }
    else if (IsKeyDown(KEY_UP))
    {
        delta.y = -speed;
    }
    return delta;
}

void keep_in_boundaries(Vector2 *pos)
{
    if (pos->x < MAN_RECTANGLE_WIDTH / 2.0f)
        pos->x +=  speed;
    else if (pos->x > WINDOW_WIDTH - (MAN_RECTANGLE_WIDTH / 2.0f))
        pos->x -= speed;
    if (pos->y < MAN_RECTANGLE_HEIGHT / 2.0f)
        pos->y +=  speed;
    else if (pos->y > WINDOW_HEIGHT - (MAN_RECTANGLE_HEIGHT / 2.0f))
        pos->y -= speed;
}
