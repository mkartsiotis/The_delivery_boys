#include "headers.h" //Including the header file

// Initialize window size
int WINDOW_HEIGHT = 1000;
int WINDOW_WIDTH = 1900;

int main(void)
{
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "The Delivery Man V0.2.0");
    HideCursor();
    SetTargetFPS(60);
    /*This is the section where we initialize all the variables*/
    // First Create an array of RECTANGLES
    // For this we need to define a global constant variable which can be static since is constant and inline to just manage memmory efficiently
    // See the header file for more info
    Rectangle map[NUM_OF_RECTANGLES_Y][NUM_OF_RECTANGLES_X];
    Initialize_Map(&map);
    // Now we create the rectangle of the player with parameters defined in headers
    Vector2 pos = {MAN_RECTANGLE_WIDTH / 2.0f, MAN_RECTANGLE_HEIGHT / 2.0f}; // Remember pos is the center so for the top-left corner we need adjustments
    Rectangle Player = {pos.x - (MAN_RECTANGLE_WIDTH / 2.0f), pos.y - (MAN_RECTANGLE_HEIGHT / 2.0f), MAN_RECTANGLE_WIDTH, MAN_RECTANGLE_HEIGHT};
    // Other variables(for delivery handling)
    bool mission_active = false;
    // Variables for delivery activation
    Vector2 PICKUP, DROPOFF;
    //Set the colour of the player just for debugging purposes
    Color col = SKYBLUE;
    while (!WindowShouldClose())
    {
        /*What we need to do is:
        A. No Mapping Yet Y(Draw specified number of rectangles on screen)
        B. Single Full screen window Υ
        C.Draw some rectangles Y
        D.Implement object avoidance Y
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

        keep_in_boundaries(&pos);                         // Check if pos is in boundaries and keep him in
        Player.y = pos.y - (MAN_RECTANGLE_HEIGHT / 2.0f); // Update x and y coordinates of the square after the keep in boundaries function
        Player.x = pos.x - (MAN_RECTANGLE_WIDTH / 2.0f);

        // In this section we will implement delivery handling techniques and NPC creation afterwards.
        if (mission_active == false)//If we do not have a mission create one
        {
            PICKUP = initialize_pickup_location(map);
            DROPOFF = initialize_dropoff_location(map, PICKUP);
        }
        else // If we have the mission check if we acomplished it.
        {
            col = WHITE;
            if (pos.x - DROPOFF.x < MAN_RECTANGLE_WIDTH && pos.x - DROPOFF.x > -MAN_RECTANGLE_WIDTH && pos.y - DROPOFF.y < MAN_RECTANGLE_HEIGHT && pos.x - DROPOFF.x < MAN_RECTANGLE_HEIGHT)
            {
                col = BLUE;
                mission_active = false;
            }
            
        }

        // Draw section
        BeginDrawing();
        ClearBackground(DARKGRAY);
        DrawRectangles(map);
        DrawRectangle(Player.x, Player.y, Player.width, Player.height, col);
        if(mission_active == true)draw_pickup_and_dropoff(PICKUP, DROPOFF);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}