#include "headers.h" //Including the header file

// Initialize some parameters
int WINDOW_WIDTH = 1900;
int WINDOW_HEIGHT = 1000;

int main(void)
{
    printf("WINDOW DIMENSIONS, %d, %d", WINDOW_WIDTH, WINDOW_HEIGHT);
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "The Delivery Man V0.2.0");
    ToggleFullscreen();
    int monitor = GetCurrentMonitor();
    WINDOW_WIDTH = GetMonitorWidth(monitor);
    WINDOW_HEIGHT = GetMonitorHeight(monitor);
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
    int deduce_score_counter = 0;
    // Variables for delivery activation
    grid_and_map_coords PICKUP = {(Vector2){-100, -100}, -1, -1}, DROPOFF = {(Vector2){-100, -100}, -1, -1};
    // Set the colour of the player just for debugging purposes
    Color col = GREEN;
    // For debugging we add a variable that controls the grid drawing
    bool should_draw_grid = false;
    // Variables used in A*
    best_possible_path a_star_results = {0};    // Strores the A*results in a same type variable
    grid_coordinates current_grid_pos = {0, 0}; // For updating current_grid_pos every time;
    int a_star_counter = 0;                     // Counts how many times before A star is called
    // NPC VARIABLES
    NPC npc = {0};
    npc.position.x = MAP_WIDTH - npc.WIDTH;
    npc.HEIGHT = MAN_RECTANGLE_HEIGHT;
    npc.WIDTH = MAN_RECTANGLE_WIDTH;
    npc.speed = 2;
    bool has_crushed_with_npc = false;
    init_cars(); // Initialize all cars
    // Camera logic and initializtion
    // IN MAIN SETUP
    Camera3D camera3d = {0};
    camera3d.position = (Vector3){
        0.0f, 800.0f, 800.0f}; // Camera is up in the sky (Y=20) and back a bit (Z=10)
    camera3d.target = (Vector3){
        0.0f, 0.0f, 0.0f}; // Looking at the center
    camera3d.up = (Vector3){
        0.0f, 1.0f, 0.0f};                    // "Up" is Y
    camera3d.fovy = 45.0f;                    // Field of view
    camera3d.projection = CAMERA_PERSPECTIVE; // Makes it look 3D
    // MiniMap logic
    Camera2D minimap_cam;
    minimap_cam.offset = (Vector2){
        1500 + MINIMAP_WIDTH / 2.0f, 50 + MINIMAP_HEIGHT / 2.0f};
    minimap_cam.rotation = 0.0f;
    minimap_cam.target = (Vector2){
        MAP_WIDTH / 2.0f, MAP_HEIGHT / 2.0f};
    minimap_cam.zoom = ((float)MINIMAP_WIDTH / MAP_WIDTH) < ((float)MINIMAP_HEIGHT / MAP_HEIGHT) ? ((float)MINIMAP_WIDTH / MAP_WIDTH) : ((float)MINIMAP_HEIGHT / MAP_HEIGHT);
    // Gamescreen logic
    ScreenStatus GameScreen = {PREVIEW, true, 0}; // Be careful!Here we set the locked levels to 0.
    enum Screen pre_load_screen = LEVEL1;
    // Models for 3d rendering
    // Player Cube(it is a model so that we can turn the cube by printing the model turned)
    Mesh cubeMesh = GenMeshCube(MAN_RECTANGLE_WIDTH, MAN_3D_HEIGHT, MAN_RECTANGLE_HEIGHT); //  Create a mesh (The geometry)
    Model playerModel = LoadModelFromMesh(cubeMesh);                                       // Load it into a Model
    // Bamboo house
    Bamboo_House = LoadModel("city_building.glb");
    if (Bamboo_House.meshCount == 0)
    {
        printf("ERROR: Bamboo House failed to load! Check filename/path.\n");
    }
    else
    {
        printf("SUCCESS: Bamboo House loaded with %d meshes.\n", Bamboo_House.meshCount);
    }
    // Gas Station
    GasStationModel = LoadModel("vintage_super_shell_gas_station_pump.glb");
    // 2D TExtures and models
    //  Model of minimap
    //   Draw walls on texture
    /*BoundingBox box = GetModelBoundingBox(Bamboo_House); //We need to find the model's bounding box, so that we can later scale it to fit in the blank cubes
    Vector3 modelSize = {
        box.max.x - box.min.x,
        box.max.y - box.min.y,
        box.max.z - box.min.z
    };*/
    RenderTexture2D minimap_texture = LoadRenderTexture(MAP_WIDTH, MAP_HEIGHT);
    Texture2D wallTexture = LoadTexture("cityskyline.png");
    // Start drawing onto this canvas (instead of the screen)
    BeginTextureMode(minimap_texture);
    ClearBackground(BLANK); // Make background transparent
    DrawRectangles(map);    // Draw the static red blocks ONCE
    EndTextureMode();
    // User logs. File format: -%HIGHSCORE1-%HIGHSCORE2-%HIGHSCORE3-
    FILE *file = fopen("userlogs.txt", "r+"); // Check if file is found and if so find it. If the file already exists it will open. If it does not exist a new file is automatically created.
    if (file == NULL)                         // If file is not found create one
    {
        file = fopen("userlogs.txt", "w+"); // Create a new file that can be read and written
        fprintf(file, "-%06d-%06d-%06d-", 0, 0, 0);
        fflush(file);
        fclose(file);
        file = fopen("userlogs.txt", "r+");
    }
    if (file == NULL) // If we are still unable to open a file just kill the programm
        exit(EXIT_FAILURE);
    // Locked levels and previous highscores!
    int HIGHSCORE1 = 0, HIGHSCORE2 = 0, HIGHSCORE3 = 0;
    fscanf(file, "-%d-%d-%d-", &HIGHSCORE1, &HIGHSCORE2, &HIGHSCORE3); // scans and assigns the values to the highscores.
    // Gas Refueling
    Gas_Station Gasoline_Refuel_Station = {0}; // This is a type gasstation and is used to set the parameters of the gas station that will be printed if neccessary.
    // Dark and vibrant modes
    bool turn_off_dark_mode = false, turn_on_dark_mode = false;
    Color backround_col = SKYBLUE;
    float night_progress = 0.0f; // Tells how much are we in the dark mode
    // Main game loop
    while (!WindowShouldClose())
    {
        // Use a switch to tell which screen we are in
        switch (GameScreen.CurrentScreen)
        {
        case PREVIEW:
            // Game locked level logic! 1.Read the thersholds and set them!2. Let the player play iff the highscore has been completed!
            if (IsKeyPressed(KEY_ENTER))
            {
                if (pre_load_screen == LEVEL1)
                    GameScreen.CurrentScreen = pre_load_screen;
                else if (pre_load_screen == LEVEL2 && HIGHSCORE1 > 10) // IMPORTANT!SET HIGHSCORE THRESHOLDS
                    GameScreen.CurrentScreen = pre_load_screen;
                else if (pre_load_screen == LEVEL3 && HIGHSCORE2 > 5)
                    GameScreen.CurrentScreen = pre_load_screen;
                set_game_parameters(&GameScreen, &npc);
            }
            else if (IsKeyPressed(KEY_UP) && pre_load_screen != LEVEL3)
            {
                pre_load_screen += 1;
            }
            else if (IsKeyPressed(KEY_DOWN) && pre_load_screen != LEVEL1)
            {
                pre_load_screen -= 1;
            }
            break;
        case GAMEOVER:
            if (GetKeyPressed() != 0)
                GameScreen.CurrentScreen = PREVIEW;
            break;
        default:
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
            // 1. MOVE PLAYER SECTION
            float movelength = delta_move();
            pos.x += movelength * sin(angleRad);
            Player.x = pos.x - (MAN_RECTANGLE_WIDTH / 2.0f);
            // After applying we need to check for collisions independently on each axis and if needed undo the requested movement
            if (check_for_collisions(Player, map) == true)
            {
                pos.x -= movelength * sin(angleRad); // Reverse movement in x axis
                Player.x = pos.x - (MAN_RECTANGLE_WIDTH / 2.0f);
            }
            // Do the same for the y axis
            pos.y += movelength * cos(angleRad);
            Player.y = pos.y - (MAN_RECTANGLE_HEIGHT / 2.0f);
            if (check_for_collisions(Player, map) == true)
            {
                pos.y -= movelength * cos(angleRad); // Reverse movement in y axis
                Player.y = pos.y - (MAN_RECTANGLE_HEIGHT / 2.0f);
            }
            // Check if the player has collided with the npc cars
            if (check_for_car_crashes(Player) == 1) // If we collide just set the speed to 0.
            {
                speed = 0;

                if (has_crushed_with_npc == false)
                {
                    deduce_score_for_mission(10); // Decrease the score if we ever crash}
                    has_crushed_with_npc = true;
                }
            }
            else
            {
                deduce_score_counter++;
                has_crushed_with_npc = false;
            }
            if (deduce_score_counter == 50)
                deduce_score_for_mission(1);

            // Turn the camera and set other parameters
            TurnCam(&camera3d, pos);
            // Continue with the game logic
            keep_in_boundaries(&pos);                         // Check if pos is in boundaries and keep him in
            Player.y = pos.y - (MAN_RECTANGLE_HEIGHT / 2.0f); // Update x and y coordinates of the square after the keep in boundaries function
            Player.x = pos.x - (MAN_RECTANGLE_WIDTH / 2.0f);
            current_grid_pos = RealToGrid(pos); // Calculate grid position of the player.

            // 2.SET PICKUP AND DROPOFF POSITIONS
            burn_fuel();


            // Burn the necessary fuel
            // Now we need to add the time limitation which we will also print on the window with the score
            // We are in the mission, so what we need is: A. We have the fuel
            //                                            B. terminate the mission if we reached the time limit.
            // For this we will use two functions the burn_fuel(void); and the draw_current_fuel(void). Check the headers for the definitions.

            if (gas <= 0) // If gas<=0 terminate mission due to reaching the gas limit
            {
                col = GREEN;            // Reset colour
                mission_active = false; // Reset mission
                picked_order = false;   // Reset var for picking orders
                DROPOFF.grid_x = -1;
                DROPOFF.grid_y = -1;
                DROPOFF.REAL = (Vector2){-10000, -10000}; // Moving away pickup and dropoff locations
                PICKUP.grid_x = -1;
                PICKUP.grid_y = -1;
                PICKUP.REAL = (Vector2){-10000, -10000};
                gas = INITIAL_GASOLINE;
                sucessful_deliveries = 0;
                pos = (Vector2){
                    0, 0};
                turn_off_dark_mode = false;
                turn_on_dark_mode = false;
                backround_col = SKYBLUE;
                night_progress = 0.0f;
                Gasoline_Refuel_Station.isvisible = false;
                // Now We are bad developers so we say game over!
                GameScreen.CurrentScreen = GAMEOVER;
                break; // Breaks from the switch.
            }

            // Strores the A*results in a same type variable
            // In this section we will implement delivery handling techniques and NPC creation afterwards.
            // Get the players grid position

            if (mission_active == false && IsKeyPressed(KEY_SPACE)) // If we do not have a mission create one
            {
                PICKUP = initialize_pickup_location(map);
                DROPOFF = initialize_dropoff_location(map, PICKUP.REAL);
                set_score_for_current_mission(pos, PICKUP.REAL, DROPOFF.REAL); // Set the score to the initial value
                // Start mission
                mission_active = true;
                // Find shortest path and assign it to a_star_results
                initGrid();
                a_star_results = aStarSearch(current_grid_pos.gridX, current_grid_pos.gridY, PICKUP.grid_x, PICKUP.grid_y);
                fseek(file, 0, 0);                                                 // Go to the begginig of the file
                fscanf(file, "-%d-%d-%d-", &HIGHSCORE1, &HIGHSCORE2, &HIGHSCORE3); // scans and assigns the values to the highscores.
            }
            else if (mission_active == true) // If we have the mission check if we acomplished it.
            {
                col = WHITE; // Have we picked a order?If so have we completed delivery
                if (picked_order == false && pos.x - (PICKUP.REAL).x < MAN_RECTANGLE_WIDTH && pos.x - (PICKUP.REAL).x > -MAN_RECTANGLE_WIDTH && pos.y - (PICKUP.REAL).y < MAN_RECTANGLE_HEIGHT && pos.y - (PICKUP.REAL).y > -MAN_RECTANGLE_HEIGHT)
                {
                    picked_order = true;
                    PICKUP = (grid_and_map_coords){
                        (Vector2){-10000, -1000}, -1, -1};
                }
                else if (pos.x - (DROPOFF.REAL).x < MAN_RECTANGLE_WIDTH && pos.x - (DROPOFF.REAL).x > -MAN_RECTANGLE_WIDTH && pos.y - (DROPOFF.REAL).y < MAN_RECTANGLE_HEIGHT && pos.y - (DROPOFF.REAL).y > -MAN_RECTANGLE_HEIGHT && picked_order == true)
                {
                    col = GREEN;
                    mission_active = false;
                    picked_order = false;
                    sucessful_deliveries += score_for_current_mission; // Increase score by the amount of things left
                    score_for_current_mission = 0;
                    // Check if we have surpassed the hich score and then if so write the new score
                    int HIGH_SCORE = 0;
                    fseek(file, 0, 0);
                    if (GameScreen.CurrentScreen == LEVEL1)
                        fscanf(file, "-%d-%*d-%*d-", &HIGH_SCORE);
                    else if (GameScreen.CurrentScreen == LEVEL2)
                        fscanf(file, "-%*d-%d-%*d-", &HIGH_SCORE);
                    if (GameScreen.CurrentScreen == LEVEL3)
                        fscanf(file, "-%*d-%*d-%d-", &HIGH_SCORE);

                    if (HIGH_SCORE < sucessful_deliveries)
                    {
                        // Move to the start of the second integer
                        //  We use fseek(file, 4, SEEK_SET) assuming -X--Y- where X is 1 digit
                        // We need to find the position!

                        fseek(file, ((GameScreen.CurrentScreen - 1) * 7 + 1), SEEK_SET);
                        // Write the new value
                        fprintf(file, "%06d-", sucessful_deliveries);
                        fflush(file);
                    }
                    DROPOFF = (grid_and_map_coords){
                        (Vector2){-100, -100}, -1, -1};
                }
                // If we have a mission check if where we are and draw the fastest route accordingly
                if (mission_active == true && picked_order == false)
                {
                    if (a_star_counter > 6) // Use the heavy a star less times so that the fps are at high levels and the programm runs smoother
                    {
                        initGrid(); // First need to initialize the grid
                        a_star_results = aStarSearch(current_grid_pos.gridX, current_grid_pos.gridY, PICKUP.grid_x, PICKUP.grid_y);
                        a_star_counter = 0;
                    }
                }

                else if (mission_active == true && picked_order == true)
                {
                    if (a_star_counter > 6) // Use the heavy a star less times so that the fps are at high levels and the programm runs smoother
                    {
                        initGrid(); // First need to initialize the grid
                        a_star_counter = 0;
                        a_star_results = aStarSearch(current_grid_pos.gridX, current_grid_pos.gridY, DROPOFF.grid_x, DROPOFF.grid_y);
                    }
                }
                a_star_counter++;
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
            update_npc_cars();
            // 6. Gas Station
            if (Gasoline_Refuel_Station.isvisible == false)
                Gasoline_Refuel_Station = refuel_station();
            check_for_refuel(&Gasoline_Refuel_Station, pos); // check if we are inside a station and perform a refuel if needed
            // Check if the npc has caught the player
            if (check_if_caught(pos, npc) == 1)
            {
                GameScreen.CurrentScreen = GAMEOVER;
                Initialize_Map(&map);
                CreateWalls(); // Now we create the rectangle of the player with parameters defined in headers
                gas = INITIAL_GASOLINE;
                pos = (Vector2){
                    MAN_RECTANGLE_WIDTH / 2.0f, MAN_RECTANGLE_HEIGHT / 2.0f}; // Remember pos is the center so for the top-left corner we need adjustments
                Player = (Rectangle){
                    pos.x - (MAN_RECTANGLE_WIDTH / 2.0f), pos.y - (MAN_RECTANGLE_HEIGHT / 2.0f), MAN_RECTANGLE_WIDTH, MAN_RECTANGLE_HEIGHT};
                mission_active = false;
                picked_order = false;
                sucessful_deliveries = 0;
                deduce_score_counter = 0;
                PICKUP = (grid_and_map_coords){(Vector2){-100, -100}, -1, -1}, DROPOFF = (grid_and_map_coords){(Vector2){-100, -100}, -1, -1};
                col = GREEN;
                should_draw_grid = false;
                a_star_counter = 0; // Counts how many times before A star is called
                npc.position.x = MAP_WIDTH - npc.WIDTH;
                npc.HEIGHT = MAN_RECTANGLE_HEIGHT;
                npc.WIDTH = MAN_RECTANGLE_WIDTH;
                npc.speed = 2;
                init_cars(); // Initialize all cars
                WaitTime(1.5);
            }
            break; // Breaks from the screen loop
        }

        // Draw section
        BeginDrawing();
        // This is the dimm mode to make the screen black.
        if (gas < INITIAL_GASOLINE / 2.6f && turn_on_dark_mode == false && turn_off_dark_mode == false)
            turn_on_dark_mode = true;

        else if (gas < INITIAL_GASOLINE / 4.3f && turn_on_dark_mode == true && turn_off_dark_mode == false)
        {
            turn_off_dark_mode = true;
            turn_on_dark_mode = false;
        }
        if (turn_on_dark_mode)
        {
            night_progress += 0.005f;
        }
        else if (turn_off_dark_mode)
        {
            night_progress -= 0.005f;
        }
        if (night_progress >= 1.0f) // Checks if we have surpassed the color limit
        {
            night_progress = 1.0f;
        }
        if (night_progress <= 0.0f) // Stops if we go below 0.
        {
            night_progress = 0.0f;
            turn_off_dark_mode = false;
        }
        backround_col = LerpColor(SKYBLUE, BLACK, night_progress); // Blend the colors
        ClearBackground(backround_col);

        DrawFPS(900, 10);
        switch (GameScreen.CurrentScreen)
        {
        case PREVIEW:
            char ch1[50] = {0};
            sprintf(ch1, "WELCOME PRESS ENTER TO LOAD LEVEL %d", pre_load_screen);
            DrawText(ch1, 500, 500, 50, WHITE);
            break;
        case GAMEOVER:
            DrawText("GAME OVER. Press any key to continue", 400, 500, 50, RED);
            break;
        default:
            // Start camera
            BeginMode3D(camera3d);
            DrawPlane((Vector3){
                          (float)WINDOW_WIDTH / 2.0f, -0.1, (float)WINDOW_HEIGHT / 2.0f},
                      (Vector2){3000, 3000}, DARKGRAY);
            // Here we implement the billboard logic which helps us make the outer barriers of the game
            Vector3 wallPos = {-20.0f, 0.0f, -80.0f};
            Vector3 lockUp = {0.0f, 1.0f, 0.0f};
            Vector2 size = {50.0f, 50.0f};
            Vector2 anchor = {0.5f, 0.0f};
            Rectangle sourceRec = {0.0f, 0.0f, (float)wallTexture.width, (float)wallTexture.height};
            // Draw command
            DrawBillboardPro(
                camera3d,
                wallTexture,
                sourceRec, // The image rectangle
                wallPos,   // Position {0,0,0}
                lockUp,    // Locked Axis (prevents falling over)
                size,
                anchor,
                0.0f,
                WHITE);

            print_refuel_station(Gasoline_Refuel_Station);
            DrawCubes(map);          // Draws map
            DrawModelEx(playerModel, // Draw the model you load from the playerModel
                        (Vector3){pos.x, MAN_3D_HEIGHT / 2.0f, pos.y},
                        (Vector3){0.0f, 1.0f, 0.0f},
                        angleRad * RAD2DEG,
                        (Vector3){1.0f, 1.0f, 1.0f},
                        col);

            if (mission_active == true)
            {
                draw_pickup_and_dropoff3D(PICKUP.REAL, DROPOFF.REAL);
                draw_astar_results3D(a_star_results);
            }
            draw_npc3D(npc);
            draw_cars();
            if (should_draw_grid == true)
                draw_grid();
            EndMode3D(); // End camera 3d

            // MINIMAP DRAW
            DrawRectangle(1500, 50, MINIMAP_WIDTH, MINIMAP_HEIGHT, BLACK);
            DrawRectangleLines(1500, 50, MINIMAP_WIDTH, MINIMAP_HEIGHT, WHITE); // Border

            // Draw where gas station is.
            if (Gasoline_Refuel_Station.isvisible == true)
            {
                char gascoordstext[30] = {0};
                sprintf(gascoordstext, "GAS IN X:%d Y:%d", Gasoline_Refuel_Station.grid_x, Gasoline_Refuel_Station.grid_y);
                DrawText(gascoordstext, 5, WINDOW_HEIGHT - 50, 20, WHITE);
            }

            // 2. Start Clipping (Only draw inside this box)
            BeginScissorMode(1500, 50, MINIMAP_WIDTH, MINIMAP_HEIGHT);

            BeginMode2D(minimap_cam);
            if (Gasoline_Refuel_Station.isvisible == true)
            {
                Color semicolor = ColorAlpha(WHITE, 0.92f);                                                          // Create a colour that is semi transparent
                DrawCircle((int)Gasoline_Refuel_Station.REAL.x, (int)Gasoline_Refuel_Station.REAL.y, 35, semicolor); // Draw the circle where the gas station is
            }
            DrawTextureRec(minimap_texture.texture,
                           (Rectangle){
                               0, 0, minimap_texture.texture.width, -minimap_texture.texture.height},
                           (Vector2){
                               0, 0},
                           WHITE);                                                       // Do not draw the rectangles but the model!
            DrawRectangle(Player.x, Player.y, Player.width * 5, Player.height * 5, col); // Draw player three times larger for better place visualizatiomn
            if (mission_active == true)
            {
                draw_pickup_and_dropoff(PICKUP.REAL, DROPOFF.REAL);
                draw_astar_results(a_star_results);
            }
            draw_npc(npc);
            EndMode2D();
            EndScissorMode();

            draw_fuel_bar();
            if (mission_active == true)
                draw_mission_score();                                                                           // Draw score that is going to be awarded if mission is active indeed
            Draw_and_update_score_window(sucessful_deliveries, HIGHSCORE1, HIGHSCORE2, HIGHSCORE3, GameScreen); // Draw score
            drawspeed();                                                                                        // Draws a speedometer.
            char ch[50] = {0};
            sprintf(ch, "GRID COORDS: %d %d", current_grid_pos.gridX, current_grid_pos.gridY);
            DrawText(ch, 1400, 25, 20, WHITE);
            break; // Breaks from the switch loop
        }
        EndDrawing();
    }
    UnloadModel(playerModel);     // Unload the model we built
    UnloadModel(GasStationModel); // Unload the gas station model
    UnloadModel(Bamboo_House);    // Unload the building's model
    CloseWindow();
    fclose(file); // Closes the file
    return 0;
}