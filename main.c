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
    bool picked_order = false;
    int sucessful_deliveries = 0;
    // Variable used to check if the player is in his first run or not
    bool is_first_time = true;
    // Variables for delivery activation
    Vector2 PICKUP = {-100, -100}, DROPOFF = {-100, -100};
    // Set the colour of the player just for debugging purposes
    Color col = BLUE;
    // Get the time of the start of the clock
    time_t start_timer_time = 0; // This is a type of of variable that saves the time(included in time.h)
    int timer_diff = 100;        // Variable for storing the time difference in a given moment of the game.

    while (!WindowShouldClose())
    {
        /*What we need to do is:
        A. No Mapping Yet Y(Draw specified number of rectangles on screen)
        B. Single Full screen window Υ
        C.Draw some rectangles Y
        D.Implement object avoidance Y
        E.Set up a timer
        F.Set up random pickup
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

        // This is the portion of the code where we implement the time logic
        if (is_first_time == true) // If it is the first mission Initialize_timer
            start_timer_time = time(NULL);
        else
            timer_diff = current_timer_difference(start_timer_time); // Calculate timer diffence if distibutor is active(on duty)
        // Now we need to add the time limitation which we will also print on the window with the score
        // We are in the mission, so what we need is: A. to get time(which we get with a difference from the initial time of the delivery) and
        //                                            B. terminate the mission if we reached the time limit.
        // For this we will use two functions the current_timer_difference(INITIAL TIME) and the draw_current_timer(int time). Check the headers for the definitions.

        if (timer_diff <= 0) // If time_diff<=0 terminate mission due to reaching the time limit
        {
            col = BLUE;                      // Reset colour
            mission_active = false;          // Reset mission
            picked_order = false;            // Reset var for picking orders
            DROPOFF = (Vector2){-100, -100}; // Moving away pickup and dropoff locations
            PICKUP = (Vector2){-100, -100};
            // Now We are bad developers so we say game over!
            break; // Breaks from the loop of the game = TOTAL DISASTER but for now lets say its ok...
        }

        // In this section we will implement delivery handling techniques and NPC creation afterwards.
        if (mission_active == false && IsKeyPressed(KEY_SPACE)) // If we do not have a mission create one
        {
            PICKUP = initialize_pickup_location(map);
            DROPOFF = initialize_dropoff_location(map, PICKUP);
            // Start mission
            mission_active = true;
            // Tell the programm that the game has started(Delivery man is on duty)
            is_first_time = false;
        }
        else if (mission_active == true) // If we have the mission check if we acomplished it.
        {
            col = WHITE; // Have we picked a order?If so have we completed delivery
            if (picked_order == false && pos.x - PICKUP.x < MAN_RECTANGLE_WIDTH && pos.x - PICKUP.x > -MAN_RECTANGLE_WIDTH && pos.y - PICKUP.y < MAN_RECTANGLE_HEIGHT && pos.y - PICKUP.y > -MAN_RECTANGLE_HEIGHT)
            {
                picked_order = true;
                PICKUP = (Vector2){-100, -100};
            }
            else if (pos.x - DROPOFF.x < MAN_RECTANGLE_WIDTH && pos.x - DROPOFF.x > -MAN_RECTANGLE_WIDTH && pos.y - DROPOFF.y < MAN_RECTANGLE_HEIGHT && pos.y - DROPOFF.y > -MAN_RECTANGLE_HEIGHT && picked_order == true)
            {
                col = BLUE;
                mission_active = false;
                picked_order = false;
                sucessful_deliveries++;
                DROPOFF = (Vector2){-100, -100};
            }
        }

        // Draw section
        BeginDrawing();
        ClearBackground(DARKGRAY);
        DrawRectangles(map);
        DrawRectangle(Player.x, Player.y, Player.width, Player.height, col);
        Draw_and_update_score_window(sucessful_deliveries);
        if (mission_active == true)
            draw_pickup_and_dropoff(PICKUP, DROPOFF);
        if (is_first_time == false)
            draw_current_timer(timer_diff);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}