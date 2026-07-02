/*
 * Όνομα Παιχνιδιού: Ο Διανομέας (The Delivery Man)
 * Συγγραφείς: Καρτσιώτης Μιχαήλ, ΑΕΜ: 11892
 *             Κατσιμάνης Δημήτριος, ΑΕΜ: 11895
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
 *                                          -Πατώντας space ενεργοποιείται το nitro.
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
    InitGameAudio(0);
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
    Color col = WHITE;
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
    // FRACTALS - PERLY NOISE(NOT MUCH DEVELOPMENT BY US PRIMARILY FROM RAY EXAMPLES)
    Image noiseImage = GenImagePerlinNoise(100, 100, 50, 50, 4.0f); // Create noise image
    ImageFormat(&noiseImage, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);

    Color *noisePixels = LoadImageColors(noiseImage);
    int width = noiseImage.width;
    int height = noiseImage.height;
    float centerX = width / 2.0f;
    float centerY = height / 2.0f;
    float maxDist = width / 2.0f; // The radius of our island

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            // Calculate distance from center
            float dx = x - centerX;
            float dy = y - centerY;
            float distanceFromCenter = sqrtf(dx * dx + dy * dy);
            float distNormal = distanceFromCenter / maxDist;
            float gradient = 1.0f;
            if (distNormal > 0.7f)
            {
                // Linearly fade from 1.0 down to 0.0 at the edge
                gradient = 1.0f - ((distNormal - 0.7f) / 0.3f);
            }
            if (gradient < 0.0f)
                gradient = 0.0f;

            int index = (y * width) + x;

            // Current height from Perlin Noise
            int currentHeight = noisePixels[index].r;
            unsigned char newHeight = (unsigned char)(currentHeight * gradient);

            // Apply to all channels (Grayscale)
            noisePixels[index].r = newHeight;
            noisePixels[index].g = newHeight;
            noisePixels[index].b = newHeight;
        }
    }

    UnloadImage(noiseImage); // Clear old data
    noiseImage = GenImageColor(width, height, BLANK);
    UnloadImage(noiseImage);
    noiseImage.data = noisePixels;
    noiseImage.width = width;
    noiseImage.height = height;
    noiseImage.format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;
    noiseImage.mipmaps = 1;
    Mesh mesh = GenMeshHeightmap(noiseImage, (Vector3){100, 20, 100}); // Create mesh from image
    Model Fractals = LoadModelFromMesh(mesh);
    Image paintImage = ImageCopy(noiseImage);
    Texture2D fractaltexture = LoadTextureFromImage(paintImage);
    Color *pixels = LoadImageColors(paintImage);
    int pixelCount = paintImage.width * paintImage.height;
    for (int i = 0; i < pixelCount; i++) // COLOR LOAD
    {
        unsigned char height = pixels[i].r;

        if (height < 85)
        {
            pixels[i] = (Color){76, 62, 40, 255};
        }
        else if (height < 145)
        {
            pixels[i] = (Color){50, 205, 50, 255};
        }
        else if (height < 195)
        {
            pixels[i] = (Color){139, 69, 19, 255};
        }
        else
        {
            pixels[i] = WHITE;
        }
    }
    UpdateTexture(fractaltexture, pixels);

    // Upload the modified pixels back to the GPU texture
    UpdateTexture(fractaltexture, pixels);
    Fractals.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = fractaltexture; // colour the model

    UnloadImageColors(pixels); // Free the array created by LoadImageColors
    UnloadImage(noiseImage);   // Free the noise image
    UnloadImage(paintImage);   // Free the paint image
    // Player Cube(it is a model so that we can turn the cube by printing the model turned)
    // Load the player's model
    int model_counter = 0;
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
    Texture2D PREVIEWTEXTURE = LoadTexture("Previewimage.png");
    Texture2D PREVIEWTEXTURE1 = LoadTexture("LEVEL1.png");
    Texture2D PREVIEWTEXTURE2 = LoadTexture("LEVEL2.png");
    Texture2D PREVIEWTEXTURE3 = LoadTexture("LEVEL3.png");
    Texture2D PREVIEWTEXTURE4 = LoadTexture("LEVEL4.png");
    Texture2D INSTRUCTS = LoadTexture("PREVIEWSCREENINSTUCTIONS.png");
    // Start drawing onto this canvas (instead of the screen)
    BeginTextureMode(minimap_texture);
    ClearBackground(BLANK); // Make background transparent
    DrawRectangles(map);    // Draw the static red blocks ONCE
    EndTextureMode();
    // User logs. File format: -%HIGHSCORECOINS-
    FILE *file = fopen("userlogs.txt", "r+"); // Check if file is found and if so find it. If the file already exists it will open. If it does not exist a new file is automatically created.
    if (file == NULL)                         // If file is not found create one
    {
        file = fopen("userlogs.txt", "w+"); // Create a new file that can be read and written
        fprintf(file, "-%06d-", 0);
        fflush(file);
        fclose(file);
        file = fopen("userlogs.txt", "r+");
    }
    if (file == NULL) // If we are still unable to open a file just kill the program
        exit(EXIT_FAILURE);
    // Locked levels and previous highscores!
    int HIGHSCORE = 0;
    fscanf(file, "-%d-", &HIGHSCORE); // scans and assigns the values to the highscores.
    // Gas Refueling
    Gas_Station Gasoline_Refuel_Station = {0}; // This is a type gasstation and is used to set the parameters of the gas station that will be printed if neccessary.
    // Dark and vibrant modes
    bool turn_off_dark_mode = false, turn_on_dark_mode = false;
    Color backround_col = SKYBLUE;
    float night_progress = 0.0f; // Tells how much are we in the dark mode
    // Initialize the grid
    initGrid();
    // SRAND FOR ALL THE rANDOM FUNCS
    srand(time(NULL));
    // NITRO
    bool is_nitro_active = false;
    bool is_nitro_ready = false;
    int initialtime = 0;
    // Main game loop
    while (!WindowShouldClose())
    {
        // Insert the background music
        UpdateGameAudio();
        // Use a switch to tell which screen we are in
        switch (GameScreen.CurrentScreen)
        {
        case INTRTUCTIONS:

            if (GetKeyPressed() != 0)
                GameScreen.CurrentScreen = PREVIEW;
            break;
        case PREVIEW0:
            if (GetKeyPressed() != 0)
                GameScreen.CurrentScreen = INTRTUCTIONS;
            break;
        case PREVIEW:
            //  Game locked level logic! 1.Read the thersholds and set them!2. Let the player play iff the highscore has been completed!
            if (IsKeyPressed(KEY_ENTER))
            {
                if (pre_load_screen == LEVEL1)
                    GameScreen.CurrentScreen = pre_load_screen;
                else if (pre_load_screen == LEVEL2 && HIGHSCORE > 600) // IMPORTANT!SET HIGHSCORE THRESHOLDS
                    GameScreen.CurrentScreen = pre_load_screen;
                else if (pre_load_screen == LEVEL3 && HIGHSCORE > 1200)
                    GameScreen.CurrentScreen = pre_load_screen;
                else if (pre_load_screen == LEVEL4 && HIGHSCORE > 2000)
                    GameScreen.CurrentScreen = pre_load_screen;
                set_game_parameters(&GameScreen, &npc);
            }
            else if ((IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_RIGHT)) && pre_load_screen != LEVEL4)
            {
                pre_load_screen += 1;
            }
            else if ((IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_LEFT)) && pre_load_screen != LEVEL1)
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

            // Update Variables Section(type N. TASK)
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
                    deduce_score_for_mission(10, selected_mission_index); // Decrease the score if we ever crash
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
            if (GameScreen.CurrentScreen == LEVEL4)
            {
                burn_fuel();
                burn_fuel();
                burn_fuel();
            }
            else if (GameScreen.CurrentScreen == LEVEL4)
                burn_fuel();
            else
                burn_fuel();
            // Burn the necessary fuel
            // Now we need to add the time limitation which we will also print on the window with the score
            // We are in the mission, so what we need is: A. We have the fuel
            //                                            B. terminate the mission if we reached the time limit.
            // For this we will use two functions the burn_fuel(void); and the draw_current_fuel(void). Check the headers for the definitions.

            if (gas <= 0) // If gas<=0 terminate mission due to reaching the gas limit
            {
                GameScreen.CurrentScreen = GAMEOVER;
                // Move to the start of the second integer
                //  We use fseek(file, 4, SEEK_SET) assuming -X--Y- where X is 1 digit
                // We need to find the position!
                fseek(file, 0, 0);
                // Write the new value
                int newscore = HIGHSCORE + sucessful_deliveries;
                fprintf(file, "-%06d-", newscore);
                fflush(file);
                fseek(file, 0, 0);
                fscanf(file, "-%06d-", &HIGHSCORE);
                initialtime = 0;
                speedMAX = 3;
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
                SwitchTrack(0);
                init_PICKUP_and_DROPOFF(PICKUP, DROPOFF); // Re-initialize pickup and dropoff
                col = WHITE;
                should_draw_grid = false;
                is_nitro_ready = false;
                is_nitro_active = false;
                a_star_counter = 0; // Counts how many times before A star is called
                model_counter = 0;
                npc.position.x = MAP_WIDTH - npc.WIDTH;
                npc.HEIGHT = MAN_RECTANGLE_HEIGHT;
                npc.WIDTH = MAN_RECTANGLE_WIDTH;
                npc.speed = 2;
                Gasoline_Refuel_Station.isvisible = false;
                Gasoline_Refuel_Station.grid_x = 0;
                Gasoline_Refuel_Station.grid_y = 0;
                Gasoline_Refuel_Station.REAL = (Vector2){0.0f, 0.0f};

                init_cars(); // Initialize all cars
                WaitTime(1.5);
                break;
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
                        fseek(file, 0, 0);                // Go to the beginnig of the file
                        fscanf(file, "-%d-", &HIGHSCORE); // scans and assigns the values to the highscores.
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
                    col = WHITE;
                    mission_active = false;
                    picked_order = false;
                    sucessful_deliveries += score_for_current_mission[selected_mission_index]; // Increase score by the amount of things left
                    score_for_current_mission[selected_mission_index] = 0;
                    // Check if we have surpassed the high score and then if so write the new score
                    fseek(file, 0, 0);
                    fscanf(file, "-%d-", &HIGHSCORE); // scans and assigns the values to the highscores.
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

            if (model_counter % 4 == 0)
                update_npc_cars();
            if (model_counter > 10)
                model_counter = 0;
            // 6. Gas Station
            if (Gasoline_Refuel_Station.isvisible == false)
                Gasoline_Refuel_Station = refuel_station();
            check_for_refuel(&Gasoline_Refuel_Station, pos); // check if we are inside a station and perform a refuel if needed
            // 7. AUDIO
            // if (engineFrequency > 469.0)
            //    engineFrequency = 95 + (npc_smart_counter % 5 + rand() % 5) + (speed / 4.0f) * 500.f;
            // else
            //    engineFrequency = 95 + (npc_smart_counter % 5) + (speed / 4.0f) * 250.f;
            // Check if the npc has caught the player
            // 8. NITRO
            if (initialtime < 902)
                initialtime++;
            if (initialtime > 900 && is_nitro_active == false && is_nitro_ready == false)
            {
                is_nitro_ready = true;
            }
            if (is_nitro_ready == true && is_nitro_active == false && IsKeyPressed(KEY_SPACE))
            {
                is_nitro_ready = false;
                is_nitro_active = true;
                speedMAX = 4;
                SwitchTrack(1);
                initialtime = 0;
            }
            if (is_nitro_ready == false && is_nitro_active == true && initialtime > (14 * 60))
            {
                is_nitro_ready = false;
                is_nitro_active = false;
                speedMAX = 3;
                SwitchTrack(0);
                initialtime = 0;
            }
            if (check_if_caught(pos, npc) == 1)
            {
                GameScreen.CurrentScreen = GAMEOVER;

                // Move to the start of the second integer
                //  We use fseek(file, 4, SEEK_SET) assuming -X--Y- where X is 1 digit
                // We need to find the position!
                fseek(file, 0, 0);
                // Write the new value
                int newscore = HIGHSCORE + sucessful_deliveries;
                fprintf(file, "-%06d-", newscore);
                fflush(file);
                fseek(file, 0, 0);
                fscanf(file, "-%06d-", &HIGHSCORE);
                // Set to 0 initial time
                initialtime = 0;
                speedMAX = 3;
                SwitchTrack(0);
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
                col = WHITE;
                should_draw_grid = false;
                is_nitro_ready = false;
                is_nitro_active = false;
                a_star_counter = 0; // Counts how many times before A star is called
                model_counter = 0;
                npc.position.x = MAP_WIDTH - npc.WIDTH;
                npc.HEIGHT = MAN_RECTANGLE_HEIGHT;
                npc.WIDTH = MAN_RECTANGLE_WIDTH;
                Gasoline_Refuel_Station.isvisible = false;
                Gasoline_Refuel_Station.grid_x = 0;
                Gasoline_Refuel_Station.grid_y = 0;
                Gasoline_Refuel_Station.REAL = (Vector2){0.0f, 0.0f};
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
        case INTRTUCTIONS:
            DrawTextureEx(INSTRUCTS, (Vector2){0.0f, 0.0f}, 0.0f, 1.5f, WHITE);
            break;
        case PREVIEW0:
            DrawText("MICHAEL KARTSIOTIS & KATSIMANIS DIMITRIS", ((int)WINDOW_WIDTH - 530), WINDOW_HEIGHT - 40, 20, WHITE);
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

            if (HIGHSCORE < 600)
                DrawLine((WINDOW_WIDTH / 9.0f) - 2, (WINDOW_HEIGHT / 9.0f) - 2, (WINDOW_WIDTH / 9.0f) - 2 + (PREVIEWTEXTURE1.width * scale1), (WINDOW_HEIGHT / 9.0f) - 2 + (PREVIEWTEXTURE1.height * scale1), RED);
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
                float posX = (5 * WINDOW_WIDTH / 9.0f) - 2;
                float posY = (WINDOW_HEIGHT / 9.0f) - 2;

                float width = (PREVIEWTEXTURE2.width * scale2) + 4;
                float height = (PREVIEWTEXTURE2.height * scale2) + 33;

                Rectangle Rec = {posX, posY, width, height};
                DrawRectangleLinesEx(Rec, 6, ORANGE);
            }
            else if (pre_load_screen == LEVEL3)
            {
                float posX = (WINDOW_WIDTH / 9.0f) - 2;
                float posY = (5 * WINDOW_HEIGHT / 9.0f) - 2;

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
                float posX = (5 * WINDOW_WIDTH / 9.0f) - 2;
                float posY = (5 * WINDOW_HEIGHT / 9.0f) - 2;

                float width = (PREVIEWTEXTURE4.width * scale4) + 4;
                float height = (PREVIEWTEXTURE4.height * scale4) + 5;

                Rectangle Rec = {posX, posY, width, height};
                DrawRectangleLinesEx(Rec, 6, ORANGE);
            }
            char text_1[20] = {0};
            sprintf(text_1, "COINS:%d", HIGHSCORE);
            DrawText(text_1, 50, 50, 30, YELLOW);
            break;
        case GAMEOVER:
            ClearBackground(BLACK);
            int text_length = MeasureText("GAME OVER. Press any key to continue", 50);
            DrawText("GAME OVER. Press any key to continue", (WINDOW_WIDTH / 2) - (text_length / 2), WINDOW_HEIGHT / 2, 50, RED);
            break;
        default:
            // Start camera
            BeginMode3D(camera3d);
            DrawPlane((Vector3){
                          (float)WINDOW_WIDTH / 2.0f, -0.1, (float)WINDOW_HEIGHT / 2.0f},
                      (Vector2){3000, 2000}, (Color){50, 50, 50, 255});
            print_refuel_station(Gasoline_Refuel_Station);
            if (model_counter == 0)
                DrawCubes(map, pos); // Draws map
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
            if (model_counter == 0)
                draw_cars((Vector3){pos.x, 0.0f, pos.y});
            if (should_draw_grid == true)
                draw_grid();
            // FRACTALS
            if (pos.x < 40)
                DrawModelEx(Fractals, (Vector3){-350.0f, -22.0f, (float)(WINDOW_HEIGHT / 2.0f) + 650}, (Vector3){0.0f, 1.0f, 0.0f}, 90.0f, (Vector3){13.0f, 6.0f, 0.6f}, WHITE);
            // FRACTALS
            else if (pos.x > WINDOW_WIDTH - 40)
                DrawModelEx(Fractals, (Vector3){((float)WINDOW_WIDTH + 350.0f), -22.0f, (float)(WINDOW_HEIGHT / 2.0f) + 650}, (Vector3){0.0f, 1.0f, 0.0f}, 90.0f, (Vector3){13.0f, 6.0f, 0.6f}, WHITE);
            // FRACTALS
            if (pos.y < 35)
                DrawModelEx(Fractals, (Vector3){(float)(WINDOW_HEIGHT / 2.0f) - 500, -22.0f, -350.0f}, (Vector3){0.0f, 1.0f, 0.0f}, 0.0f, (Vector3){19.0f, 6.0f, 0.6f}, WHITE);
            // FRACTALS
            else if (pos.y > WINDOW_HEIGHT - 35)
                DrawModelEx(Fractals, (Vector3){(float)(WINDOW_HEIGHT / 2.0f) - 500, -22.0f, (float)WINDOW_HEIGHT + 350.0f}, (Vector3){0.0f, 1.0f, 0.0f}, 0.0f, (Vector3){19.0f, 6.0f, 0.6f}, WHITE);
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
            float time = GetTime();
            if (is_nitro_active == true && initialtime < 900)
            {
                int length = MeasureText("NITRO BOOST", 50);
                int posXboostText = (WINDOW_WIDTH - length) / 2;
                int posYboostText = WINDOW_HEIGHT - 150;
                DrawText("NITRO BOOST", posXboostText, posYboostText, 50, (Color){(time / 255.0f) + 5, (125.0f * time) + 5, (150.0f * time) + 5, (float)(255.0f)});
            }
            else
                draw_nitro_bar(initialtime);
            // MINIMAP DRAW
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
            DrawText("MICHAEL KARTSIOTIS & KATSIMANIS DIMITRIS", ((int)WINDOW_WIDTH - 530), WINDOW_HEIGHT - 40, 20, WHITE);
            draw_fuel_bar();
            if (mission_active == true)
                draw_mission_score(selected_mission_index);                            // Draw score that is going to be awarded if mission is active indeed
            Draw_and_update_score_window(sucessful_deliveries, HIGHSCORE, GameScreen); // Draw score
            drawspeed();                                                               // Draws a speedometer.
            char ch[50] = {0};
            sprintf(ch, "GRID COORDS: %d %d", current_grid_pos.gridX, current_grid_pos.gridY);
            DrawText(ch, 1700, 25, 20, WHITE);
            break; // Breaks from the switch loop
        }
        EndDrawing();
    }
    UnloadTexture(PREVIEWTEXTURE);  // Unload Texture
    UnloadTexture(PREVIEWTEXTURE1); // Unload Texture
    UnloadTexture(PREVIEWTEXTURE3); // Unload Texture
    UnloadTexture(PREVIEWTEXTURE2); // Unload Texture
    UnloadTexture(PREVIEWTEXTURE4); // Unload Texture

    UnloadModel(playerModel);     // Unload the model we built
    UnloadModel(GasStationModel); // Unload the gas station model
    UnloadModel(Building);        // Unload the building's model
    UnloadModel(NPCmodel1);
    UnloadModel(NPCmodel3);
    UnloadModel(NPCmodel2);
    UnloadTexture(INSTRUCTS);
    UnloadTexture(fractaltexture);
    UnloadModel(Fractals);
    CloseAudioDevice();
    CloseWindow();
    fclose(file); // Closes the file
    return 0;
}
