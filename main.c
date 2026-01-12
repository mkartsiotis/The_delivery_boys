/*
 * Όνομα Παιχνιδιού: Ο Διανομέας (The Delivery Man)
 * Συγγραφείς: Καρτσιώτης Μιχαήλ, ΑΕΜ: 11892
 *             Κατσιμάνης Δημήτριος, ΑΕΜ:
 *
 * Περιγραφή: Ανάπτυξη λογισμικού παιχνιδιού διανομέα στη γλώσσα C με τη χρήση της βιβλιοθήκης raylib.h
 *            Κανόνες παιχνιδιού - Οδηγίες: -Μετά την έναρξη του παιχνιδιού πατώντας space εισέσχεσθε στην κεντρική οθόνη.
 *                                          -Από εκεί μπορείτε με τα βελάκια να επιλέξετε επίπεδο.
 *                                          -Έχουν δημιουργηθεί τρία επίπεδα αυξανόμενης δυσκολίας στα οποία εισέρχεσθε ανάλογα με τις επιδόσεις σας στα προηγούμενα επίπεδα όπως εμφανίζονται στην οθόνη.
 *                                          -Πατώντας Enter εισέρχεσθε στο επιλεγμένο επίπεδο.
 *                                          -Η κίνηση του διανομέα γίνεται μέσω των βελών.
 *                                          -Για την επιλογή αποστολής χρησιμοποιήστε το W η το S και για την επιλογή αποστολής πατήστε Εnter
 *                                          -Μετά την επιλογή αποστολής εμφανίζεται με κίτρινη γραμμή ή βέλτιστη διαδρομή προς το σημείο παραλαβής της παραγγελίας και μετά την παραλαβή η βέλτιστη διαδρομή προς το σημείο παράδοσης
 *                                          -Το σκορ που λαμβάνετε με την ολοκλήρωση κάθε αποστολής εμφανίζεται στο κεντρικό πάνω μέρος της οθόνης. Να σημειωθεί ότι κατά την σύγκρουσή σας με άλλα οχήματα και με την παρεύλευση χρόνου οι βαθμοί αφαιρούνται!
 *                                          -Κάτω δεξιά στην οθόνη σας εμφανίζεται μία μπάρα με τη διαθέσιμη ποσότητα καυσίμου στη δεξαμενή. Λίγο πριν το απόθεμα στη δεξαμενή εξαντληθεί εμφανίζεται στο χάρτη πρατήριο με τη μορφή δοχείου καυσίμου σε σημείο που επισημαίνεται στον μικρό χάρτη πάνω και αριστερά στην οθόνη με λευκό κύκλο.
 *                                          -Λαμβάνωντας το δοχείο καυσίμου που εμφανλίζεται παρατείνεται η διάρκεια ζωής σας.
 *                                          -Το παιχνίδι τερματίζεται είτε με την σύγκρουσή σας με το αστυνομικό όχημα είτε με την εξάντληση του αποθετηρίου καυσίμων.
 *                                          -Στόχος: Η συγκέντωση όσο το δυνατόν περισσότερων χρημάτων που ξεκλειδώνουν επίπεδα και προνόμια.
 *
 * Copyright (C) 2025-2026 Καρτσιώτης Μιχαήλ και Κατσιμάνης Δημήτριος
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#include "headers.h" //Including the header file

// Initialize some parameters
int WINDOW_WIDTH = 1900;
int WINDOW_HEIGHT = 1000;

int main(void)
{
    printf("WINDOW DIMENSIONS, %d, %d", WINDOW_WIDTH, WINDOW_HEIGHT);
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "The Delivery Man V0.2.0");
    InitAudioDevice();
    ToggleFullscreen();
    int monitor = GetCurrentMonitor();
    WINDOW_WIDTH = GetMonitorWidth(monitor);
    WINDOW_HEIGHT = GetMonitorHeight(monitor);
    HideCursor();
    SetTargetFPS(60);
    /*This is the section where we initialize all the variables*/
    // First Create an array of RECTANGLES
    // For this we need to define a global constant variable which can be static since is constant and inline to just manage memory efficiently
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
    Delivery_Location PICKUP[NUM_OF_ITEMS_ON_LIST], DROPOFF[NUM_OF_ITEMS_ON_LIST]; // Declare the array
    init_PICKUP_and_DROPOFF(PICKUP, DROPOFF);                                      // Initialize the array
    int selected_mission_index = -1;
    // Set the colour of the player just for debugging purposes
    Color col = GREEN;
    // For debugging we add a variable that controls the grid drawing
    bool should_draw_grid = false;
    // Variables used in A*
    best_possible_path a_star_results = {0};    // Stores the A*results in a same type variable
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
    // Camera logic and initialization
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
    ScreenStatus GameScreen = {PREVIEW0, true, 0}; // Be careful!Here we set the locked levels to 0.
    enum Screen pre_load_screen = LEVEL1;
    // Models for 3D rendering

    // Player Cube(it is a model so that we can turn the cube by printing the model turned)

    // Load the player's model
    Chaser = LoadModel("LaPolizia.glb");
    Model playerModel = LoadModel("motor2.glb");
    if (playerModel.meshCount == 0)
    {

        printf("ERROR: Motorbike failed to load! Check filename/path.\n");
    }
    else
    {
        printf("SUCCESS: Motorbike loaded with %d meshes.\n", playerModel.meshCount);
    }
    // Load the buildings' model
    Building = LoadModel("Commercial_Building.glb");
    if (Building.meshCount == 0)
    {
        printf("ERROR: Building failed to load! Check filename/path.\n");
    }

    else
    {
        printf("SUCCESS: Building loaded with %d meshes.\n", Building.meshCount);
    }
    // Gas Station model
    GasStationModel = LoadModel("Gas_tank.glb");
    // First npc model
    NPCmodel1 = LoadModel("Npc_car1.glb");
    if (NPCmodel1.meshCount == 0)
    {
        printf("ERROR: First npc model failed to load! Check filename/path.\n");
    }

    else
    {
        printf("SUCCESS: First npc model loaded with %d meshes.\n", NPCmodel1.meshCount);
    }
    // Second npc model
    NPCmodel2 = LoadModel("Npc_car2.glb");
    if (NPCmodel2.meshCount == 0)
    {
        printf("ERROR: Second npc model failed to load! Check filename/path.\n");
    }

    else
    {
        printf("SUCCESS: Second npc model loaded with %d meshes.\n", NPCmodel2.meshCount);
    }
    // Third npc model
    NPCmodel3 = LoadModel("Npc_car3.glb");
    if (NPCmodel3.meshCount == 0)
    {
        printf("ERROR: Third npc model failed to load! Check filename/path.\n");
    }

    else
    {
        printf("SUCCESS: Third npc model loaded with %d meshes.\n", NPCmodel3.meshCount);
    }
    RenderTexture2D minimap_texture = LoadRenderTexture(MAP_WIDTH, MAP_HEIGHT);
    Texture2D wallTexture = LoadTexture("cityskyline.png");
    Texture2D PREVIEWTEXTURE = LoadTexture("Previewimage.png");
    Texture2D PREVIEWTEXTURE1 = LoadTexture("LEVEL1.png");
    Texture2D PREVIEWTEXTURE2 = LoadTexture("LEVEL2.png");
    Texture2D PREVIEWTEXTURE3 = LoadTexture("LEVEL3.png");
    Texture2D PREVIEWTEXTURE4 = LoadTexture("LEVEL4.png");

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
    if (file == NULL) // If we are still unable to open a file just kill the program
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
    // Initialize the grid
    initGrid();
    // AUDIO
    // Setup the stream (Sample Rate, Bit Depth, Channels)
    AudioStream engineStream = LoadAudioStream(44100, 32, 1);

    // Tell raylib our sound
    SetAudioStreamCallback(engineStream, AudioInputCallback);

    // Start the noise
    PlayAudioStream(engineStream);
    // SRAND FOR ALL THE rANDOM FUNCS
    srand(time(NULL));
    // Main game loop
    while (!WindowShouldClose())
    {
        // Use a switch to tell which screen we are in
        switch (GameScreen.CurrentScreen)
        {
        case PREVIEW0:
            if (GetKeyPressed() != 0)
                GameScreen.CurrentScreen = PREVIEW;
            break;
        case PREVIEW:
            volume = 0; // Set volume to 0;
            // Game locked level logic! 1.Read the thersholds and set them!2. Let the player play iff the highscore has been completed!
            if (IsKeyPressed(KEY_ENTER))
            {
                if (pre_load_screen == LEVEL1)
                    GameScreen.CurrentScreen = pre_load_screen;
                else if (pre_load_screen == LEVEL2 && HIGHSCORE1 > 10) // IMPORTANT!SET HIGHSCORE THRESHOLDS
                    GameScreen.CurrentScreen = pre_load_screen;
                else if (pre_load_screen == LEVEL3 && HIGHSCORE2 > 5)
                    GameScreen.CurrentScreen = pre_load_screen;
                else if (pre_load_screen == LEVEL4)
                    GameScreen.CurrentScreen = pre_load_screen;
                set_game_parameters(&GameScreen, &npc);
            }
            else if ((IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_LEFT)) && pre_load_screen != LEVEL4)
            {
                pre_load_screen += 1;
            }
            else if ((IsKeyPressed(KEY_DOWN)|| IsKeyPressed(KEY_LEFT)) && pre_load_screen != LEVEL1)
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
                    deduce_score_for_mission(10, selected_mission_index); // Decrease the score if we ever crash}
                    has_crushed_with_npc = true;
                }
            }
            else
            {
                deduce_score_counter++;
                has_crushed_with_npc = false;
            }
            if (deduce_score_counter == 250)
            {
                deduce_score_counter = 0;
                deduce_score_for_mission(4, selected_mission_index);
            }

            // Turn the camera and set other parameters
            TurnCam(&camera3d, pos);
            // Continue with the game logic
            keep_in_boundaries(&pos);                         // Check if pos is in boundaries and keep him in
            Player.y = pos.y - (MAN_RECTANGLE_HEIGHT / 2.0f); // Update x and y coordinates of the square after the keep in boundaries function
            Player.x = pos.x - (MAN_RECTANGLE_WIDTH / 2.0f);
            current_grid_pos = RealToGrid(pos); // Calculate grid position of the player.

            // 2.SET PICKUP AND DROPOFF POSITIONS
            burn_fuel();
            burn_fuel();

            burn_fuel();

            // Burn the necessary fuel
            // Now we need to add the time limitation which we will also print on the window with the score
            // We are in the mission, so what we need is: A. We have the fuel
            //                                            B. terminate the mission if we reached the time limit.
            // For this we will use two functions the burn_fuel(void); and the draw_current_fuel(void). Check the headers for the definitions.

            if (gas <= 0) // If gas<=0 terminate mission due to reaching the gas limit
            {
                col = GREEN;                              // Reset colour
                mission_active = false;                   // Reset mission
                picked_order = false;                     // Reset var for picking orders
                init_PICKUP_and_DROPOFF(PICKUP, DROPOFF); // Re-initialize pickup and dropoff
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

            // Stores the A*results in a same type variable
            // In this section we will implement delivery handling techniques and NPC creation afterwards.
            // Get the players grid position

            // PICKUP AND DROPOFF SYSTEM
            check_and_apply_mission(PICKUP, DROPOFF); // Sets all the parameters for current mission
            if (mission_active == false)              // If we do not have a mission create one
            {
                // Check if all of our deliveries have been assigned
                for (int i = 0; i < NUM_OF_ITEMS_ON_LIST; i++) // For all of the items on the list
                {
                    if (PICKUP[i].is_set == false) // Set the point
                        PICKUP[i] = initialize_pickup_location(map);

                    if (DROPOFF[i].is_set == false)
                        DROPOFF[i] = initialize_dropoff_location(map, PICKUP[i].REAL);
                    set_score_for_current_mission(pos, PICKUP[i].REAL, DROPOFF[i].REAL, i); // Set the score to the initial value
                    // Now we need to be able to start the mission when being told to do so
                    if (PICKUP[i].is_selected == true && DROPOFF[i].is_selected == true)
                    { //  Start mission
                        mission_active = true;
                        selected_mission_index = i;
                        // Find shortest path and assign it to a_star_results
                        initGrid();
                        a_star_results = aStarSearch(current_grid_pos.gridX, current_grid_pos.gridY, PICKUP[i].grid_x, PICKUP[i].grid_y);
                        fseek(file, 0, 0);                                                 // Go to the beginnig of the file
                        fscanf(file, "-%d-%d-%d-", &HIGHSCORE1, &HIGHSCORE2, &HIGHSCORE3); // scans and assigns the values to the highscores.
                    }
                }
            }
            else if (mission_active == true) // If we have the mission check if we accomplished it.
            {
                col = WHITE; // Have we picked a order?If so have we completed delivery
                if (picked_order == false && pos.x - (PICKUP[selected_mission_index].REAL).x < MAN_RECTANGLE_WIDTH && pos.x - (PICKUP[selected_mission_index].REAL).x > -MAN_RECTANGLE_WIDTH && pos.y - (PICKUP[selected_mission_index].REAL).y < MAN_RECTANGLE_HEIGHT && pos.y - (PICKUP[selected_mission_index].REAL).y > -MAN_RECTANGLE_HEIGHT)
                {
                    picked_order = true;
                    PICKUP[selected_mission_index] = (Delivery_Location){
                        {(Vector2){-10000, -1000}, -1, -1}, false, false, false};
                }
                else if (pos.x - (DROPOFF[selected_mission_index].REAL).x < MAN_RECTANGLE_WIDTH && pos.x - (DROPOFF[selected_mission_index].REAL).x > -MAN_RECTANGLE_WIDTH && pos.y - (DROPOFF[selected_mission_index].REAL).y < MAN_RECTANGLE_HEIGHT && pos.y - (DROPOFF[selected_mission_index].REAL).y > -MAN_RECTANGLE_HEIGHT && picked_order == true)
                {
                    col = GREEN;
                    mission_active = false;
                    picked_order = false;
                    sucessful_deliveries += score_for_current_mission[selected_mission_index]; // Increase score by the amount of things left
                    score_for_current_mission[selected_mission_index] = 0;
                    // Check if we have surpassed the high score and then if so write the new score
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
                    DROPOFF[selected_mission_index] = (Delivery_Location){
                        {(Vector2){-100, -100}, -1, -1}, false, false, false};
                }
                // If we have a mission check if where we are and draw the fastest route accordingly
                if (mission_active == true && picked_order == false)
                {
                    if (a_star_counter > 6) // Use the heavy a star less times so that the fps are at high levels and the programm runs smoother
                    {
                        initGrid(); // First need to initialize the grid
                        a_star_results = aStarSearch(current_grid_pos.gridX, current_grid_pos.gridY, PICKUP[selected_mission_index].grid_x, PICKUP[selected_mission_index].grid_y);
                        a_star_counter = 0;
                    }
                }
                else if (mission_active == true && picked_order == true)
                {
                    if (a_star_counter > 6) // Use the heavy a star less times so that the fps are at high levels and the programm runs smoother
                    {
                        initGrid(); // First need to initialize the grid
                        a_star_counter = 0;
                        a_star_results = aStarSearch(current_grid_pos.gridX, current_grid_pos.gridY, DROPOFF[selected_mission_index].grid_x, DROPOFF[selected_mission_index].grid_y);
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
            // 7. AUDIO
            if (engineFrequency > 469.0)
                engineFrequency = 95 + (npc_smart_counter % 5 + rand() % 5) + (speed / 4.0f) * 500.f;
            else
                engineFrequency = 95 + (npc_smart_counter % 5) + (speed / 4.0f) * 250.f;
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
                init_PICKUP_and_DROPOFF(PICKUP, DROPOFF); // Re-initialize pickup and dropoff
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
        case PREVIEW0:
            DrawTextureEx(PREVIEWTEXTURE, (Vector2){0, 0}, 0.0f, 1.5f, WHITE);
            float time_1 = GetTime();
            if ((int)time_1 % 2 == 0)
            {
                int length = MeasureText(">PRESS ANY KEY TO CONTINUE<", 30);
                DrawText(">PRESS ANY KEY TO CONTINUE<", (WINDOW_WIDTH / 2.0f) - ((float)length / 2.0f), 5.0f * WINDOW_HEIGHT / 6.0f, 30, BLACK);
            }
            break;
        case PREVIEW:
            int length = MeasureText("LEVEL 1", 20);
            float scale1 = ((float)WINDOW_WIDTH / 3.0f) / (float)PREVIEWTEXTURE1.width;
            float scale2 = ((float)WINDOW_WIDTH / 3.0f) / (float)PREVIEWTEXTURE2.width;
            float scale3 = ((float)WINDOW_WIDTH / 3.0f) / (float)PREVIEWTEXTURE3.width;
            float scale4 = ((float)WINDOW_WIDTH / 3.0f) / (float)PREVIEWTEXTURE4.width;

            ClearBackground(LIGHTGRAY);
            DrawTextureEx(PREVIEWTEXTURE1, (Vector2){WINDOW_WIDTH / 9, WINDOW_HEIGHT / 9}, 0, scale1, WHITE);
            DrawText("LEVEL 1", WINDOW_WIDTH / 9 + (PREVIEWTEXTURE1.width * scale1 / 2.0f) - (length / 2.0f), WINDOW_HEIGHT / 10 + ((float)PREVIEWTEXTURE1.height * scale1) + 25.0f, 20, BLACK);
            DrawTextureEx(PREVIEWTEXTURE2, (Vector2){WINDOW_WIDTH / 9, (5 * WINDOW_HEIGHT / 9)}, 0, scale2, WHITE);
            DrawText("LEVEL 2", 5 * WINDOW_WIDTH / 9 + (PREVIEWTEXTURE1.width * scale1 / 2.0f) - (length / 2.0f), WINDOW_HEIGHT / 10 + ((float)PREVIEWTEXTURE1.height * scale1) + 25.0f, 20, BLACK);
            DrawTextureEx(PREVIEWTEXTURE3, (Vector2){5 * WINDOW_WIDTH / 9, (WINDOW_HEIGHT / 9)}, 0, scale3, WHITE);
            DrawText("LEVEL 3", WINDOW_WIDTH / 9 + (PREVIEWTEXTURE1.width * scale1 / 2.0f) - (length / 2.0f), 5 * WINDOW_HEIGHT / 10 + ((float)PREVIEWTEXTURE1.height * scale1) + 45.0f, 20, BLACK);
            DrawTextureEx(PREVIEWTEXTURE4, (Vector2){(5 * WINDOW_WIDTH / 9), (5 * WINDOW_HEIGHT / 9)}, 0, scale4, WHITE);
            DrawText("LEVEL 4", 5 * WINDOW_WIDTH / 9 + (PREVIEWTEXTURE1.width * scale1 / 2.0f) - (length / 2.0f), 5 * WINDOW_HEIGHT / 10 + ((float)PREVIEWTEXTURE1.height * scale1) + 45.0f, 20, BLACK);

            if (pre_load_screen == LEVEL1)
            {
                // Positioning logic for Level 1
                float posX = (WINDOW_WIDTH / 9.0f) - 2;
                float posY = (WINDOW_HEIGHT / 9.0f) - 2;
                Rectangle Rec = {posX, posY, (PREVIEWTEXTURE1.width * scale1) + 4, (PREVIEWTEXTURE1.height * scale1) + 4};
                DrawRectangleLinesEx(Rec, 6, ORANGE);
            }
            else if (pre_load_screen == LEVEL2)
            {
                // Use 5/9 for X, but check if you intended to change Y as well
                float posX = (5 * WINDOW_WIDTH / 9.0f) - 2;
                float posY = (WINDOW_HEIGHT / 9.0f) - 2;

                // Ensure PREVIEWTEXTURE2 and scale2 are correct
                float width = (PREVIEWTEXTURE2.width * scale2) + 4;
                float height = (PREVIEWTEXTURE2.height * scale2) + 33;

                Rectangle Rec = {posX, posY, width, height};
                DrawRectangleLinesEx(Rec, 6, ORANGE);
            }
            else if (pre_load_screen == LEVEL3)
            {
                // Use 5/9 for X, but check if you intended to change Y as well
                float posX = (WINDOW_WIDTH / 9.0f) - 2;
                float posY = (5 * WINDOW_HEIGHT / 9.0f) - 2;

                // Ensure PREVIEWTEXTURE2 and scale2 are correct
                float width = (PREVIEWTEXTURE2.width * scale2) + 4;
                float height = (PREVIEWTEXTURE2.height * scale2) + 5;

                Rectangle Rec = {posX, posY, width, height};
                DrawRectangleLinesEx(Rec, 6, ORANGE);
            }
            else if (pre_load_screen == LEVEL3)
            {
                Rectangle Rec = {(Vector2){(5 * WINDOW_WIDTH / 9) - 2, (5 * WINDOW_HEIGHT / 9) - 2}.x, (Vector2){(5 * WINDOW_WIDTH / 9 - 2), (5 * WINDOW_HEIGHT / 9) - 2}.y, (PREVIEWTEXTURE4.width * scale4) + 4, (PREVIEWTEXTURE4.height * scale4) + 4};
                DrawRectangleLinesEx(Rec, 6, ORANGE);
            }
            else if (pre_load_screen == LEVEL4)
            {
                // Use 5/9 for X, but check if you intended to change Y as well
                float posX = (5 * WINDOW_WIDTH / 9.0f) - 2;
                float posY = (5 * WINDOW_HEIGHT / 9.0f) - 2;

                // Ensure PREVIEWTEXTURE2 and scale2 are correct
                float width = (PREVIEWTEXTURE4.width * scale4) + 4;
                float height = (PREVIEWTEXTURE4.height * scale4) + 5;

                Rectangle Rec = {posX, posY, width, height};
                DrawRectangleLinesEx(Rec, 6, ORANGE);
            }
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
                        (Vector3){pos.x, MAN_3D_HEIGHT - 0.5, pos.y},
                        (Vector3){0.0f, 1.0f, 0.0f},
                        angleRad * RAD2DEG,
                        (Vector3){1.0f, 1.0f, 1.0f},
                        col);

            if (mission_active == true)
            {
                draw_pickup_and_dropoff3D(PICKUP[selected_mission_index].REAL, DROPOFF[selected_mission_index].REAL);
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
            if (mission_active == false)
                Draw_list_of_deliveries(PICKUP, DROPOFF);

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
            DrawRectangle(Player.x, Player.y, Player.width * 5, Player.height * 5, col); // Draw player three times larger for better place visualization
            if (mission_active == true)
            {
                draw_pickup_and_dropoff(PICKUP[selected_mission_index].REAL, DROPOFF[selected_mission_index].REAL);
                draw_astar_results(a_star_results);
            }
            else
            {
                int pre_selected_index = -1;
                for (int i = 0; i < NUM_OF_ITEMS_ON_LIST; i++)
                    if (PICKUP[i].is_pre_selected == 1 && DROPOFF[i].is_pre_selected == 1)
                        pre_selected_index = i;
                draw_pickup_and_dropoff(PICKUP[pre_selected_index].REAL, DROPOFF[pre_selected_index].REAL);
            }
            draw_npc(npc);
            EndMode2D();
            EndScissorMode();

            draw_fuel_bar();
            if (mission_active == true)
                draw_mission_score(selected_mission_index);                                                     // Draw score that is going to be awarded if mission is active indeed
            Draw_and_update_score_window(sucessful_deliveries, HIGHSCORE1, HIGHSCORE2, HIGHSCORE3, GameScreen); // Draw score
            drawspeed();                                                                                        // Draws a speedometer.
            char ch[50] = {0};
            sprintf(ch, "GRID COORDS: %d %d", current_grid_pos.gridX, current_grid_pos.gridY);
            DrawText(ch, 1400, 25, 20, WHITE);
            break; // Breaks from the switch loop
        }
        EndDrawing();
    }
    UnloadTexture(PREVIEWTEXTURE);  // Unload Texture
    UnloadTexture(PREVIEWTEXTURE1); // Unload Texture
    UnloadTexture(PREVIEWTEXTURE3); // Unload Texture
    UnloadTexture(PREVIEWTEXTURE2); // Unload Texture
    UnloadTexture(PREVIEWTEXTURE4); // Unload Texture

    UnloadModel(playerModel);        // Unload the model we built
    UnloadModel(GasStationModel);    // Unload the gas station model
    UnloadModel(Building);           // Unload the building's model
    UnloadAudioStream(engineStream); // Unload audio connection
    CloseWindow();
    fclose(file); // Closes the file
    return 0;
}