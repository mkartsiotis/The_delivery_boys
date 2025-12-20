#include "headers.h" //Including the header file

int main(void)
{
    InitWindow(GetScreenWidth(), GetScreenHeight(), "The Delivery Man V0.2.0");
    HideCursor();
    SetTargetFPS(60);
    /*This is the section where we initialize all the variables*/
    // First Create an array of RECTANGLES
    // For this we need to define a global constant variable which can be static since is constant and inline to just manage memmory efficiently
    // See the header file for more info
    Rectangle map[NUM_OF_RECTANGLES_Y][NUM_OF_RECTANGLES_X];
    Initialize_Map(&map);
    // Now we create the rectangle of the player with parameters defined in headers
    Vector2 pos = {1400, 900}; // Remember pos is the center so for the top-left corner we need adjustments
    Rectangle Player = {pos.x - (MAN_RECTANGLE_WIDTH / 2.0f), pos.y - (MAN_RECTANGLE_HEIGHT / 2.0f), MAN_RECTANGLE_WIDTH, MAN_RECTANGLE_HEIGHT};

    while (!WindowShouldClose())
    {
        /*What we need to do is:
        A. No Mapping Yet Y(Draw specified number of rectangles on screen)
        B. Single Full screen window Υ
        C.Draw some rectangles Y
        D.Implement object avoidance
        E.Set up a timer
        F.Set up random pickup location
        G.Set up a drop of location
        H.Award the player*/

        // Update Variables Section
        float dx = delta_move().x, dy = delta_move().y; // Save the requested moves on x and y axis as dx and dy respectively
        pos.x += dx;
        Player.x = pos.x - (MAN_RECTANGLE_WIDTH / 2.0f);
        // After applying we need to check for collisions independently on each axis and if needed undo the requested movement
        if (check_for_collisions(Player, map) == true)
        {
            pos.x -= dx; // Reverse movement in x axis
            Player.x = pos.x - (MAN_RECTANGLE_WIDTH / 2.0f);
        }
        // Do the same for the y axis
        pos.y += dy;
        Player.y = pos.y - (MAN_RECTANGLE_HEIGHT / 2.0f);
        if (check_for_collisions(Player, map) == true)
        {
            pos.y -= dy; // Reverse movement in y axis
            Player.y = pos.y - (MAN_RECTANGLE_HEIGHT / 2.0f);
        }

        // Draw section
        BeginDrawing();
        ClearBackground(DARKGRAY);
        DrawRectangles(map);
        DrawRectangle(Player.x, Player.y, Player.width, Player.height, BLUE);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}