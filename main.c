#include "headers.h" //Including the header file

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
    CreateWalls();
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
    grid_and_map_coords PICKUP = {(Vector2){-100, -100}, -1, -1}, DROPOFF = {(Vector2){-100, -100}, -1, -1};
    // Set the colour of the player just for debugging purposes
    Color col = BLUE;
    // Get the time of the start of the clock
    time_t start_timer_time = 0; // This is a type of of variable that saves the time(included in time.h)
    int timer_diff = 100;        // Variable for storing the time difference in a given moment of the game.
    // For debugging we add a variable that controls the grid drawing
    bool should_draw_grid = false;
    // Variables used in A*
    best_possible_path a_star_results = {0};    // Strores the A*results in a same type variable
    grid_coordinates current_grid_pos = {0, 0}; // For updating current_grid_pos every time;
    // NPC VARIABLES
    NPC npc = {0};
    npc.position.x = MAP_WIDTH - npc.WIDTH;
    npc.HEIGHT = MAN_RECTANGLE_HEIGHT;
    npc.WIDTH = MAN_RECTANGLE_WIDTH;
    npc.speed = 2.5;
    // Gamescreen logic
    enum Screen GameScreen = PREVIEW;
    // Main game loop
    while (!WindowShouldClose())
    {
        // Use a switch to tell which screen we are in
        switch (GameScreen)
        {
        case PREVIEW:
            if (IsKeyPressed(KEY_ENTER))
            {
                GameScreen = GAMEON;
            }
            break;
        case GAMEON:
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
            CreateWalls(); // Creates the walls for the initGrid an other functions
            // 1. MOVE PLAYER SECTION
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
            current_grid_pos = RealToGrid(pos); // Calcuate grid position of the player.

            // 2. TIME SECTION AND SET PICKUP AND DROPOFF POSITIONS
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
                col = BLUE;             // Reset colour
                mission_active = false; // Reset mission
                picked_order = false;   // Reset var for picking orders
                DROPOFF.grid_x = -1;
                DROPOFF.grid_y = -1;
                DROPOFF.REAL = (Vector2){-100, -100}; // Moving away pickup and dropoff locations
                PICKUP.grid_x = -1;
                PICKUP.grid_y = -1;
                PICKUP.REAL = (Vector2){-100, -100};
                // Now We are bad developers so we say game over!
                GameScreen = GAMEOVER;
                break; // Breaks from the switch.
            }

            // Strores the A*results in a same type variable
            // In this section we will implement delivery handling techniques and NPC creation afterwards.
            // Get the players grid position

            if (mission_active == false && IsKeyPressed(KEY_SPACE)) // If we do not have a mission create one
            {
                PICKUP = initialize_pickup_location(map);
                DROPOFF = initialize_dropoff_location(map, PICKUP.REAL);
                // Start mission
                mission_active = true;
                // Tell the programm that the game has started(Delivery man is on duty)
                is_first_time = false;
                // Find shortest path and assign it to a_star_results
                initGrid();
                a_star_results = aStarSearch(current_grid_pos.gridX, current_grid_pos.gridY, PICKUP.grid_x, PICKUP.grid_y);
            }
            else if (mission_active == true) // If we have the mission check if we acomplished it.
            {
                col = WHITE; // Have we picked a order?If so have we completed delivery
                if (picked_order == false && pos.x - (PICKUP.REAL).x < MAN_RECTANGLE_WIDTH && pos.x - (PICKUP.REAL).x > -MAN_RECTANGLE_WIDTH && pos.y - (PICKUP.REAL).y < MAN_RECTANGLE_HEIGHT && pos.y - (PICKUP.REAL).y > -MAN_RECTANGLE_HEIGHT)
                {
                    picked_order = true;
                    PICKUP = (grid_and_map_coords){(Vector2){-100, -100}, -1, -1};
                    // Get the fastest route
                }
                else if (pos.x - (DROPOFF.REAL).x < MAN_RECTANGLE_WIDTH && pos.x - (DROPOFF.REAL).x > -MAN_RECTANGLE_WIDTH && pos.y - (DROPOFF.REAL).y < MAN_RECTANGLE_HEIGHT && pos.y - (DROPOFF.REAL).y > -MAN_RECTANGLE_HEIGHT && picked_order == true)
                {
                    col = BLUE;
                    mission_active = false;
                    picked_order = false;
                    sucessful_deliveries++;
                    DROPOFF = (grid_and_map_coords){(Vector2){-100, -100}, -1, -1};
                }
                // If we have a mission check if where we are and draw the fastest route accordingly
                if (mission_active == true && picked_order == false)
                {
                    initGrid(); // First need to initialize the grid
                    a_star_results = aStarSearch(current_grid_pos.gridX, current_grid_pos.gridY, PICKUP.grid_x, PICKUP.grid_y);
                }
                else if (mission_active == true && picked_order == true)
                {
                    initGrid(); // First need to initialize the grid
                    a_star_results = aStarSearch(current_grid_pos.gridX, current_grid_pos.gridY, DROPOFF.grid_x, DROPOFF.grid_y);
                }
            }
            // 4. DRAW GRID FOR DEBUGGING
            //  Check before printing the grid
            if (IsKeyPressed(KEY_G))
            {
                // Change the state of the boolean
                should_draw_grid = !should_draw_grid;
            }

            // 5. NPC section
            updateNPC(&npc, pos, map);

            // Check if the npc has caught the player
            if (check_if_caught(pos, npc) == 1)
                GameScreen = GAMEOVER;
            break; // Breaks from the screen loop

        default:
            break;
        }

        // Draw section
        BeginDrawing();
        ClearBackground(DARKGRAY);
        switch (GameScreen)
        {
        case PREVIEW:
            DrawText("WELCOME PRESS ENTER TO CONTINUE", 500, 500, 50, BLACK);
            break;
        case GAMEON:
            DrawRectangles(map);                                                 // Draws map
            DrawRectangle(Player.x, Player.y, Player.width, Player.height, col); // Draw player
            Draw_and_update_score_window(sucessful_deliveries);                  // Draw score
            if (mission_active == true)
            {
                draw_pickup_and_dropoff(PICKUP.REAL, DROPOFF.REAL);
                draw_astar_results(a_star_results);
            }
            if (is_first_time == false)
                draw_current_timer(timer_diff);
            char ch[50] = {0};
            sprintf(ch, "GRID COORDS ARE %d %d", current_grid_pos.gridX, current_grid_pos.gridY);
            DrawText(ch, 1400, 25, 20, WHITE);
            draw_npc(npc);
            if (should_draw_grid == true)
                draw_grid();
            break; // Breaks from the switch loop
        case GAMEOVER:
            DrawText("GAME OVER", 500, 500, 50, RED);
            break;
        default:
            break;
        }
        EndDrawing();
    }
    CloseWindow();
    return 0;
}