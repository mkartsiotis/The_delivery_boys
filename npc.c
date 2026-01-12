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
/*It is now the time to think how NPC's work.
1.We need to calculate every X times with A* the path that the NPC will follow to reach the player
2.We need to store that in the NPC type(with typedef) along with some other paramaters vital for its movement and specifically:
->NPC speed
->NPC a variable that checks if we have reached the point
->NPC MAX_REPS that checks how many times we have run the algorithm before recalculating the path to the player.
How we are going to implement all this thinking:
1.We need the least number of function calls and the most efficient way to do it. SO what I am thinking is:
    a)In the main we create an NPC type.(outside window should close)
    b)We define initial NPC conditions
    c)We call a function that creates a path to the player and stores it to the NPC.(likely with pointer parameters to the NPC)
    d)We create an update_NPC function that checks all the conditions (in which node we are if the time limit has been reached etc).
        That same function also commands the follow function.(its the brain of the npc)
    e)We call a function that makes the NPC follow the path.(legs of the NPC)
3.Create a draw function
4. After verification that all the previous work as expected then we will add collision logic.
*/
// first we need to create the function in the header file
// We did that. Now lets make the functions.
// Its now time to safely say that we are going to ditch that logic.
#include "headers.h"
// Initialize external variables
int npc_smart_counter = 0;    // This is a counter variable that tracks how many times before the NPC changes its target to match the players position. It is the NPC's update time.
Vector2 target_npc_pos = {0}; // This is the old pos used in the update NPC and is the NPC target pos
npc_car cars_vertical[NUM_OF_RECTANGLES_X + 1][NUM_OF_NPC_CARS_ON_Y_ROAD] = {false, NPC_CAR_CEMETARY, NPC_CAR_CEMETARY, NPC_CAR_CEMETARY, SIZE_OF_CAR_X, SIZE_OF_CAR_Y, SIZE_OF_CAR_Z, 0.0f, PURPLE};
npc_car cars_horizontal[NUM_OF_RECTANGLES_Y + 1][NUM_OF_NPC_CARS_ON_X_ROAD] = {false, NPC_CAR_CEMETARY, NPC_CAR_CEMETARY, NPC_CAR_CEMETARY, SIZE_OF_CAR_X, SIZE_OF_CAR_Y, SIZE_OF_CAR_Z, 0.0f, PURPLE}; // Create an array of cars for the X and Y axis respectively.
Traffic_state Traffic_Cop;                                                                                                                                                                              //(Initialized in npc.c)This just creates a vertica and a horizontal go to make the cars behave according to the law.
int NUM_OF_NPC_CARS_ON_X_ROAD_ON_CURRENT_LEVEL = 0;                                                                                                                                                     //(Initialized in npc.c)This is a variable that is used to determine the most ammount of cars that appear on each level
int NUM_OF_NPC_CARS_ON_Y_ROAD_ON_CURRENT_LEVEL = 0;                                                                                                                                                     //(Initialized in npc.c)This is a variable that is used to determine the most ammount of cars that appear on each level
float chaser_angle = 0;

void updateNPC(NPC *chaser, Vector2 player_pos, Rectangle map[NUM_OF_RECTANGLES_Y][NUM_OF_RECTANGLES_X]) //(In npc.c)Checks conditions and recalculates path if needed.
{
    // SIMPLE_STRAIGHTFORWARD_APPROACH
    // 1.FIND_REALTIVE_POSITION
    // 2.CALCULATE REQUESTED MOVE
    // 3.CHECK IF LEGAL
    // 4.PERFORM IT!!!
    float dx = 0, dy = 0;
    if (npc_smart_counter == (int)NPC_SMART_DELAY)
    {
        target_npc_pos = player_pos;
        npc_smart_counter = 0;
    }
    else
        npc_smart_counter++;
    Rectangle RequestedPos = {chaser->position.x - chaser->WIDTH / 2.0f, chaser->position.y - chaser->HEIGHT / 2.0f, chaser->WIDTH, chaser->HEIGHT};
    if (chaser->position.x == target_npc_pos.x && chaser->position.y == target_npc_pos.y)
    {
        npc_smart_counter = (int)NPC_SMART_DELAY;
        return;
    }
    if (fabs(chaser->position.x - target_npc_pos.x) < chaser->speed)
        ;
    else if (chaser->position.x > target_npc_pos.x)
        dx = -chaser->speed;
    else if (chaser->position.x < target_npc_pos.x)
        dx = chaser->speed;
    RequestedPos.x += dx;
    // Now we need to perform checkouts and reverse the position if needed
    if (check_for_collisions(RequestedPos, map) == 1)
        RequestedPos.x -= dx;
    chaser->position.x = RequestedPos.x + chaser->WIDTH / 2.0f;
    if (fabs(chaser->position.y - target_npc_pos.y) < chaser->speed)
        ;
    else if (chaser->position.y > target_npc_pos.y)
        dy = -chaser->speed;
    else if (chaser->position.y < target_npc_pos.y)
        dy = chaser->speed;
    RequestedPos.y += dy;
    chaser_angle = (atan2f(-dy, dx) * RAD2DEG) + 180;
    // Now we need to perform checkouts and reverse the position if needed
    if (check_for_collisions(RequestedPos, map) == 1)
        RequestedPos.y -= dy;
    chaser->position.y = RequestedPos.y + chaser->HEIGHT / 2.0f;
}
int check_if_caught(Vector2 playerpos, NPC npc) // Checks if they come in contact
{
    Rectangle Player = {playerpos.x - (MAN_RECTANGLE_WIDTH / 2.0f), playerpos.y - (MAN_RECTANGLE_HEIGHT / 2.0f), MAN_RECTANGLE_WIDTH, MAN_RECTANGLE_HEIGHT};
    Rectangle NPCPlayer = {npc.position.x - (npc.WIDTH / 2.0f), npc.position.y - (npc.WIDTH / 2.0f), npc.WIDTH, npc.HEIGHT};
    return CheckCollisionRecs(Player, NPCPlayer);
}
// For the NPC random moving vehicles:
// Lets first build a working prototype and then we'll refine it.
// Objectives - steps- logic:
/*
The approach: Create a conveyer belt system where the player-npc starts from point A and drives straight to point B until it reaches that and disappears.
To do this we need:
1. An array of special type(typedefined in the header file) where all the precious data for the npc will be stored and namely:
    -Current state(active or passive)
    -Current coordinates
    -Start position and end position coordiantes
    -Size for drawing. SizeX, sizeY, sizeZ.
    -Colour for drawing.
2. After that we need to assign a number of npcs to a number of roads.
    We assign N number of cars to each road(we can have differnt values for vertical and horizontal roads).
3. We move those cars at every frame and then when we reach the end the car state should be disappeard.
4. How will the car be visible?Easy!Check the visible car with the smallest coordinate according to the road's direction.
    Then calculate the threshold for the origin and if it is larger than a number LENGTH then make the disappered car appear and initialize it.
5. Do that for all of the roads and check if the system works.
6. Add colisiion logic and set the speed of the player to 0 if a car is hit.
7. *Fail - Iterate - Improove - Succeed*
Lets start... Time for all of this is 1hr.
What functions we are going to use:
We do not need functions for initialization for the first time since we will use an extern array and all parameters will be saved.
We need a function that for a specific road:
-Decides if we need a new car
-If we do it makes it visible and initializes it
We need as well a function that:
-Updates the cars' position according to the roads' state(initial vesrions will have all the cars moving away from the top-left of the screen(positive increase). Then we will implement road logic).
And finally one that just prints the cars according to their parameters.
Note that if we keep X and Y seperated(which I think is purposeful) we might need some additional functions(seperate) for the X and Y axis.
*/
void update_npc_cars(void)
{
    // First we need to disembark on a journey to check the logic of block creation:
    // How we initialize BLOCKS:
    // We divide the screen dimensions with the number_OF_RECTANGLES * 4 in each axis
    // Then for the nth block in the x axis(same logic for the y axis) starting from 0 until number_OF_RECTANGLES - 1 we have:
    /*float stepX = (float)MAP_WIDTH / (NUM_OF_RECTANGLES_X * 4.0f);
    float x_value = (1 + 4 * n) * stepX;
    (*map)[i][n] = (Rectangle){x_value, y_value, SIZE_OF_RECTANGLES_X, SIZE_OF_RECTANGLES_Y};
    Remeber those are the top left coordintes.
    But we defined: SIZE_OF_RECTANGLES_X (MAP_WIDTH / (2.0f * NUM_OF_RECTANGLES_X))
    So the road size should be: 4 * stepX - SIZE_OF_RECTANGLES_X = ((float)MAP_WIDTH / (float)(NUM_OF_RECTANGLES_X)) - (float)SIZE_OF_RECTANGLES_X;
    Same for the Y axis.
    */
    float stepX = (float)MAP_WIDTH / (4.0f * (float)NUM_OF_RECTANGLES_X);
    float stepY = (float)MAP_HEIGHT / (4.0f * (float)NUM_OF_RECTANGLES_Y);

    float roadlength_x = MAP_WIDTH;
    float roadlength_y = MAP_HEIGHT;

    float tolerance_for_x_road = roadlength_x / (float)NUM_OF_NPC_CARS_ON_X_ROAD_ON_CURRENT_LEVEL;
    float tolerance_for_y_road = roadlength_y / (float)NUM_OF_NPC_CARS_ON_Y_ROAD_ON_CURRENT_LEVEL;

    adress_traffic(); // helps with traffic management. (see below)

    // First check for the x axis(x roads)if there are any visible
    for (int i = 0; i < NUM_OF_RECTANGLES_Y + 1; i += 2) // for all even x roads
    {
        float min_pos_x = 10000;

        // First apply the move
        //  Now for all visible cars update their position.
        for (int j = 0; j < NUM_OF_NPC_CARS_ON_X_ROAD_ON_CURRENT_LEVEL; j++) // for all cars on some of those roads
            if (cars_horizontal[i][j].is_visible == true)
            {
                // Check if we meet the threshold.
                if (cars_horizontal[i][j].pos.x < cars_horizontal[i][j].end_pos.x)
                {
                    adjust_speedx(i, j);                                        // Adjust the speed of the following car to ensure no car passes through another.
                    cars_horizontal[i][j].pos.x += cars_horizontal[i][j].speed; // If we can move, move!!!
                }
                else
                {
                    cars_horizontal[i][j].pos = NPC_CAR_CEMETARY;
                    cars_horizontal[i][j].is_visible = false;
                    cars_horizontal[i][j].start_pos = NPC_CAR_CEMETARY;
                    cars_horizontal[i][j].end_pos = NPC_CAR_CEMETARY;
                    cars_horizontal[i][j].col = choseRandomColour();
                }
            }
        // Then check the minimum distance to add a car if needed
        for (int j = 0; j < NUM_OF_NPC_CARS_ON_X_ROAD_ON_CURRENT_LEVEL; j++) // for all cars on those roads
            if (cars_horizontal[i][j].is_visible == true)
            {
                if (cars_horizontal[i][j].pos.x < min_pos_x)
                {
                    min_pos_x = cars_horizontal[i][j].pos.x;
                }
            }
        // Now if the minimum distance is larger than the threshold or if no car is in the map initialize a new car.
        if (min_pos_x < tolerance_for_x_road && min_pos_x != -1)
            ;
        else
        {
            // Make visible a new car.
            int invisible_car_num = -1;
            for (int j = 0; j < NUM_OF_NPC_CARS_ON_X_ROAD_ON_CURRENT_LEVEL; j++) // for all cars on those roads
                if (cars_horizontal[i][j].is_visible == false)
                {
                    invisible_car_num = j;
                    break;
                }
            if (invisible_car_num == -1)
                continue;
            // Now we put the car in the road start and make it visible.
            cars_horizontal[i][invisible_car_num].pos = (Vector2){0, stepY * 4 * i};
            cars_horizontal[i][invisible_car_num].is_visible = true;
            cars_horizontal[i][invisible_car_num].start_pos = (Vector2){0, stepY * 4 * i};
            cars_horizontal[i][invisible_car_num].end_pos = (Vector2){roadlength_x, stepY * 4 * i};
            cars_horizontal[i][invisible_car_num].speed = (rand() % 25 + 10.0f) / 10.0f; // Formula for speed calculation.
            cars_horizontal[i][invisible_car_num].col = choseRandomColour();
        }
    }

    // Do the same for odd roads.
    for (int i = 1; i < NUM_OF_RECTANGLES_Y + 1; i += 2) // for all even x roads
    {
        float max_pos_x = -10000;

        // First apply the move
        //  Now for all visible cars update their position.
        for (int j = 0; j < NUM_OF_NPC_CARS_ON_X_ROAD_ON_CURRENT_LEVEL; j++) // for all cars on some of those roads
            if (cars_horizontal[i][j].is_visible == true)
            {
                // Check if we meet the threshold.
                if (cars_horizontal[i][j].pos.x > cars_horizontal[i][j].end_pos.x)
                {
                    adjust_speedx(i, j);
                    cars_horizontal[i][j].pos.x -= cars_horizontal[i][j].speed;
                }
                else
                {
                    cars_horizontal[i][j].pos = NPC_CAR_CEMETARY;
                    cars_horizontal[i][j].is_visible = false;
                    cars_horizontal[i][j].start_pos = NPC_CAR_CEMETARY;
                    cars_horizontal[i][j].end_pos = NPC_CAR_CEMETARY;
                    cars_horizontal[i][j].col = choseRandomColour();
                }
            }
        // Then check the minimum distance to add a car if needed
        for (int j = 0; j < NUM_OF_NPC_CARS_ON_X_ROAD_ON_CURRENT_LEVEL; j++) // for all cars on those roads
            if (cars_horizontal[i][j].is_visible == true)
            {
                if (cars_horizontal[i][j].pos.x > max_pos_x) // FIND THE CAR WITH THE MAXIMUM POSITION
                {
                    max_pos_x = cars_horizontal[i][j].pos.x;
                }
            }
        // Now if the minimum distance is larger than the threshold or if no car is in the map initialize a new car.
        if (MAP_WIDTH - max_pos_x < tolerance_for_x_road && max_pos_x != -10000)
            ;
        else
        {
            // Make visible a new car.
            int invisible_car_num = -1;
            for (int j = 0; j < NUM_OF_NPC_CARS_ON_X_ROAD_ON_CURRENT_LEVEL; j++) // for all cars on those roads
                if (cars_horizontal[i][j].is_visible == false)
                {
                    invisible_car_num = j;
                    break;
                }
            if (invisible_car_num == -1)
                continue;
            // Now put the car in the road start. Make it visible
            cars_horizontal[i][invisible_car_num].pos = (Vector2){roadlength_x, stepY * 4 * i};
            cars_horizontal[i][invisible_car_num].is_visible = true;
            cars_horizontal[i][invisible_car_num].start_pos = (Vector2){roadlength_x, stepY * 4 * i};
            cars_horizontal[i][invisible_car_num].end_pos = (Vector2){0, stepY * 4 * i};
            cars_horizontal[i][invisible_car_num].speed = (rand() % 25 + 10.0f) / 10.0f; // Formula for speed calculation.
            cars_horizontal[i][invisible_car_num].col = choseRandomColour();
        }
    }
    // Do the same for the Y axis.
    //  First check for the x axis(x roads)if there are any visible
    for (int i = 0; i < NUM_OF_RECTANGLES_X + 1; i += 2) // for all y roads
    {
        float min_pos_y = 10000;

        // First apply the move
        //  Now for all visible cars update their position.
        for (int j = 0; j < NUM_OF_NPC_CARS_ON_Y_ROAD_ON_CURRENT_LEVEL; j++) // for all cars on those roads
            if (cars_vertical[i][j].is_visible == true)
            {
                // Check if we meet the threshold.
                if (cars_vertical[i][j].pos.y < cars_vertical[i][j].end_pos.y)
                {
                    adjust_speedy(i, j); // Adjust the car speed
                    cars_vertical[i][j].pos.y += cars_vertical[i][j].speed;
                }
                else
                {
                    cars_vertical[i][j].pos = NPC_CAR_CEMETARY;
                    cars_vertical[i][j].is_visible = false;
                    cars_vertical[i][j].start_pos = NPC_CAR_CEMETARY;
                    cars_vertical[i][j].end_pos = NPC_CAR_CEMETARY;
                    cars_vertical[i][j].col = choseRandomColour();
                }
            }
        // Then check the minimum distance to add a car if needed
        for (int j = 0; j < NUM_OF_NPC_CARS_ON_Y_ROAD_ON_CURRENT_LEVEL; j++) // for all cars on those roads
            if (cars_vertical[i][j].is_visible == true)
            {
                if (cars_vertical[i][j].pos.y < min_pos_y)
                {
                    min_pos_y = cars_vertical[i][j].pos.y;
                }
            }
        // Now if the minimum distance is larger than the threshold or if no car is in the map initialize a new car.
        if (min_pos_y < tolerance_for_y_road && min_pos_y != -1)
            ;
        else
        {
            // Make visible a new car.
            int invisible_car_num = -1;
            for (int j = 0; j < NUM_OF_NPC_CARS_ON_Y_ROAD_ON_CURRENT_LEVEL; j++) // for all cars on those roads
                if (cars_vertical[i][j].is_visible == false)
                {
                    invisible_car_num = j;
                    break;
                }
            if (invisible_car_num == -1)
                continue;
            // Now put the car in the road start. Make it visible
            cars_vertical[i][invisible_car_num].pos = (Vector2){stepX * 4 * i, 0};
            cars_vertical[i][invisible_car_num].is_visible = true;
            cars_vertical[i][invisible_car_num].start_pos = (Vector2){stepX * 4 * i, 0};
            cars_vertical[i][invisible_car_num].end_pos = (Vector2){stepX * 4 * i, roadlength_y};
            cars_vertical[i][invisible_car_num].speed = (rand() % 25 + 10.0f) / 10.0f; // Formula for speed calculation.
            cars_vertical[i][invisible_car_num].col = choseRandomColour();
        }
    }
    // Do the same for odd roads in the Y axis.
    for (int i = 1; i < NUM_OF_RECTANGLES_X + 1; i += 2) // for all even x roads
    {
        float max_pos_y = -10000;

        // First apply the move
        //  Now for all visible cars update their position.
        for (int j = 0; j < NUM_OF_NPC_CARS_ON_Y_ROAD_ON_CURRENT_LEVEL; j++) // for all cars on some of those roads
            if (cars_vertical[i][j].is_visible == true)
            {
                // Check if we meet the threshold.
                if (cars_vertical[i][j].pos.y > cars_vertical[i][j].end_pos.y)
                {
                    cars_vertical[i][j].pos.y -= cars_vertical[i][j].speed;
                    adjust_speedy(i, j); // Adjust the car speed
                }
                else
                {
                    cars_vertical[i][j].pos = NPC_CAR_CEMETARY;
                    cars_vertical[i][j].is_visible = false;
                    cars_vertical[i][j].start_pos = NPC_CAR_CEMETARY;
                    cars_vertical[i][j].end_pos = NPC_CAR_CEMETARY;
                    cars_vertical[i][j].col = choseRandomColour();
                }
            }
        // Then check the minimum distance to add a car if needed
        for (int j = 0; j < NUM_OF_NPC_CARS_ON_X_ROAD_ON_CURRENT_LEVEL; j++) // for all cars on those roads
            if (cars_vertical[i][j].is_visible == true)
            {
                if (cars_vertical[i][j].pos.y > max_pos_y) // FIND THE CAR WITH THE MAXIMUM POSITION
                {
                    max_pos_y = cars_vertical[i][j].pos.y;
                }
            }
        // Now if the minimum distance is larger than the threshold or if no car is in the map initialize a new car.
        if (MAP_HEIGHT - max_pos_y < tolerance_for_y_road && max_pos_y != -10000)
            ;
        else
        {
            // Make visible a new car.
            int invisible_car_num = -1;
            for (int j = 0; j < NUM_OF_NPC_CARS_ON_Y_ROAD_ON_CURRENT_LEVEL; j++) // for all cars on those roads
                if (cars_vertical[i][j].is_visible == false)
                {
                    invisible_car_num = j;
                    break;
                }
            if (invisible_car_num == -1)
                continue;
            // Now put the car in the road start. Make it visible
            cars_vertical[i][invisible_car_num].pos = (Vector2){stepX * 4 * i, roadlength_y};
            cars_vertical[i][invisible_car_num].is_visible = true;
            cars_vertical[i][invisible_car_num].start_pos = (Vector2){stepX * 4 * i, roadlength_y};
            cars_vertical[i][invisible_car_num].end_pos = (Vector2){stepX * 4 * i, 0};
            cars_vertical[i][invisible_car_num].speed = (rand() % 25 + 10.0f) / 10.0f; // Formula for speed calculation.
            cars_vertical[i][invisible_car_num].col = choseRandomColour();
        }
    }
}

void init_cars(void)
{
    for (int i = 0; i < NUM_OF_RECTANGLES_Y + 1; i++)       // for all x roads
        for (int j = 0; j < NUM_OF_NPC_CARS_ON_X_ROAD; j++) // for all cars on those roads
            cars_horizontal[i][j] = (npc_car){false, NPC_CAR_CEMETARY, NPC_CAR_CEMETARY, NPC_CAR_CEMETARY, SIZE_OF_CAR_X, SIZE_OF_CAR_Y, SIZE_OF_CAR_Z, 0.0f, PURPLE};
    for (int i = 0; i < NUM_OF_RECTANGLES_X + 1; i++)       // for all y roads
        for (int j = 0; j < NUM_OF_NPC_CARS_ON_Y_ROAD; j++) // for all cars on those roads
            cars_vertical[i][j] = (npc_car){false, NPC_CAR_CEMETARY, NPC_CAR_CEMETARY, NPC_CAR_CEMETARY, SIZE_OF_CAR_X, SIZE_OF_CAR_Y, SIZE_OF_CAR_Z, 0.0f, PURPLE};
}

int check_for_car_crashes(Rectangle Player) // Checks for collisions with the npc cars.
{
    bool has_crushed = false;
    for (int i = 0; i < NUM_OF_RECTANGLES_Y + 1; i++)       // for all x roads
        for (int j = 0; j < NUM_OF_NPC_CARS_ON_X_ROAD; j++) // for all cars on those roads
        {
            if (cars_horizontal[i][j].is_visible == true)
            {
                Rectangle npc_rec = {cars_horizontal[i][j].pos.x - (cars_horizontal[i][j].sizeX / 2.0f), cars_horizontal[i][j].pos.y - (cars_horizontal[i][j].sizeY / 2.0f), cars_horizontal[i][j].sizeX, cars_horizontal[i][j].sizeY};
                if (CheckCollisionRecs(Player, npc_rec) == 1)
                {
                    has_crushed = true;
                    break;
                }
            }
        }
    for (int i = 0; i < NUM_OF_RECTANGLES_X + 1; i++)       // for all y roads
        for (int j = 0; j < NUM_OF_NPC_CARS_ON_Y_ROAD; j++) // for all cars on those roads
        {
            if (cars_vertical[i][j].is_visible == true)
            {
                Rectangle npc_rec = {cars_vertical[i][j].pos.x - (cars_vertical[i][j].sizeX / 2.0f), cars_vertical[i][j].pos.y - (cars_vertical[i][j].sizeY / 2.0f), cars_vertical[i][j].sizeX, cars_vertical[i][j].sizeY};
                if (CheckCollisionRecs(Player, npc_rec) == 1)
                {
                    has_crushed = true;
                    break;
                }
            }
        }
    if (has_crushed == true)
        return 1;
    else
        return 0;
}

Color choseRandomColour(void) // Returns a random colour from rand and a decoding method.
{
    Color return_col = (Color){rand() % 256, rand() % 256, rand() % 256, 255};
    return return_col;
}

/*
Adjust speed logic:
*
1. Give random values to car speeds
2. With this function find the closest car in front.
3. For that closest car check if its distance from our car is less than a threshold
4. If that holds true then set the speed of the following car to the speed of the car ahead
AND do that only for visible cars of course!!!*/
void adjust_speedx(int i, int j) //(In npc.c)This function sets the speed of the following cars to the speed of the the slower mooving first car.
{
    // Only run this for visible cars
    if (!cars_horizontal[i][j].is_visible)
        return;

    // For Even i
    if (i % 2 == 0)
    {
        float min_dist = 10000.0f; // distance from the front of the car
        int car_ahead_index = -1;

        // Check all the cars in our line
        for (int k = 0; k < NUM_OF_NPC_CARS_ON_X_ROAD_ON_CURRENT_LEVEL; k++)
        {
            // Skip the current or invisible cars
            if (k == j || cars_horizontal[i][k].is_visible == false)
                continue;

            float distance = cars_horizontal[i][k].pos.x - cars_horizontal[i][j].pos.x;
            if (distance > 0) // For all the cars ahead...
            {
                // Find the minimum value
                if (distance < min_dist)
                {
                    min_dist = distance;
                    car_ahead_index = k;
                }
            }
        }

        // We have now just found the car directly in front. Now we check if that car is close enough to trigger the braking.(setting the speed of the current car equal to the car in front)
        float safe_gap = 6.0f;
        if (car_ahead_index != -1 && min_dist < (cars_horizontal[i][j].sizeX + safe_gap))
        {
            // Match the speed of the car ahead
            if (cars_horizontal[i][j].speed > cars_horizontal[i][car_ahead_index].speed)
            {
                cars_horizontal[i][j].speed = cars_horizontal[i][car_ahead_index].speed;
            }
        }
    }

    // For odd i
    if (i % 2 == 1)
    {
        float min_dist = 10000.0f; // distance from the front of the car
        int car_ahead_index = -1;

        // Check all the cars in our line
        for (int k = 0; k < NUM_OF_NPC_CARS_ON_X_ROAD_ON_CURRENT_LEVEL; k++)
        {
            // Skip the current or invisible cars
            if (k == j || cars_horizontal[i][k].is_visible == false)
                continue;

            float distance = cars_horizontal[i][j].pos.x - cars_horizontal[i][k].pos.x;
            if (distance > 0) // For all the cars ahead...
            {
                // Find the minimum value
                if (distance < min_dist)
                {
                    min_dist = distance;
                    car_ahead_index = k;
                }
            }
        }

        // We have now just found the car directly in front. Now we check if that car is close enough to trigger the braking.(setting the speed of the current car equal to the car in front)
        float safe_gap = 6.0f;
        if (car_ahead_index != -1 && min_dist < (cars_horizontal[i][j].sizeX + safe_gap))
        {
            // Match the speed of the car ahead
            if (cars_horizontal[i][j].speed > cars_horizontal[i][car_ahead_index].speed)
            {
                cars_horizontal[i][j].speed = cars_horizontal[i][car_ahead_index].speed;
            }
        }
    }
}

void adjust_speedy(int i, int j) //(In npc.c)This function sets the speed of the following cars to the speed of the the slower mooving first car.
{
    // Only run this for visible cars
    if (!cars_vertical[i][j].is_visible)
        return;

    // For Even i
    if (i % 2 == 0)
    {
        float min_dist = 10000.0f; // distance from the front of the car
        int car_ahead_index = -1;

        // Check all the cars in our line
        for (int k = 0; k < NUM_OF_NPC_CARS_ON_Y_ROAD_ON_CURRENT_LEVEL; k++)
        {
            // Skip the current or invisible cars
            if (k == j || cars_vertical[i][k].is_visible == false)
                continue;

            float distance = cars_vertical[i][k].pos.y - cars_vertical[i][j].pos.y;
            if (distance > 0) // For all the cars ahead...
            {
                // Find the minimum value
                if (distance < min_dist)
                {
                    min_dist = distance;
                    car_ahead_index = k;
                }
            }
        }

        // We have now just found the car directly in front. Now we check if that car is close enough to trigger the braking.(setting the speed of the current car equal to the car in front)
        float safe_gap = 6.0f;
        if (car_ahead_index != -1 && min_dist < (cars_vertical[i][j].sizeY + safe_gap))
        {
            // Match the speed of the car ahead
            if (cars_vertical[i][j].speed > cars_vertical[i][car_ahead_index].speed)
            {
                cars_vertical[i][j].speed = cars_vertical[i][car_ahead_index].speed;
            }
        }
    }

    // For odd i
    if (i % 2 == 1)
    {
        float min_dist = 10000.0f; // distance from the front of the car
        int car_ahead_index = -1;

        // Check all the cars in our line
        for (int k = 0; k < NUM_OF_NPC_CARS_ON_Y_ROAD_ON_CURRENT_LEVEL; k++)
        {
            // Skip the current or invisible cars
            if (k == j || cars_vertical[i][k].is_visible == false)
                continue;

            float distance = cars_vertical[i][j].pos.y - cars_vertical[i][k].pos.y;
            if (distance > 0) // For all the cars ahead...
            {
                // Find the minimum value
                if (distance < min_dist)
                {
                    min_dist = distance;
                    car_ahead_index = k;
                }
            }
        }

        // We have now just found the car directly in front. Now we check if that car is close enough to trigger the braking.(setting the speed of the current car equal to the car in front)
        float safe_gap = 6.0f;
        if (car_ahead_index != -1 && min_dist < (cars_vertical[i][j].sizeX + safe_gap))
        {
            // Match the speed of the car ahead
            if (cars_vertical[i][j].speed > cars_vertical[i][car_ahead_index].speed)
            {
                cars_vertical[i][j].speed = cars_vertical[i][car_ahead_index].speed;
            }
        }
    }
}

void adress_traffic(void) //(in npc.c)This function creates a vertical and a horizontal go for the cars.
{
    /*This function creates a vertical and a horizontal go for the cars.
    Main Logic:
    1.Create a vertical and a horizontal go.(see header files)
    2.Set that variable according to time.....We will do that by implementing a static variable to the function nad thus track the number of times the function is called.
    3.We need to track if an npc is close to a intersection. We will do that by implementing the grid coordinate system we already use.
    4.If a car is close to an intersection if the variable indicates a red light stop. Else...If the previous distance was stop go!!!!
    Wish me luck....*/
    static int timer_counter = 0;
    timer_counter++;

    // Track how close are the horizontal cars to intersections
    if (Traffic_Cop == Vertical_GO) // If light for horizontal cars is 0
    {
        // For all the stationary vertical cars give them the green light!!!
        for (int i = 0; i < NUM_OF_RECTANGLES_X + 1; i++)                        // for all y roads
            for (int j = 0; j < NUM_OF_NPC_CARS_ON_Y_ROAD_ON_CURRENT_LEVEL; j++) // for all cars on some of those roads
            {
                if (cars_vertical[i][j].is_visible == false) // If the cars are invisible go to the end of the loop and ignore all the changes
                    continue;
                // If their speed is 0 give them speed!!!
                if (cars_vertical[i][j].speed == 0)
                {
                    cars_vertical[i][j].speed = (rand() % 25 + 10.0f) / 10.0f;
                }
            }

        // For even roads
        for (int i = 0; i < NUM_OF_RECTANGLES_Y + 1; i += 2)                     // for all even x roads
            for (int j = 0; j < NUM_OF_NPC_CARS_ON_X_ROAD_ON_CURRENT_LEVEL; j++) // for all cars on some of those roads
            {
                if (cars_horizontal[i][j].is_visible == false) // If the cars are invisible go to the end of the loop and ignore all the changes
                    continue;
                // Now we need to remind to ourselves how we have set the parameters of the game and its core logic.
                float cellXsize = WINDOW_WIDTH / ((float)NUM_OF_RECTANGLES_X * 4); // Remember the by 4 architecture(see docs).

                float distance = 0, min_distance = 10000.0f;
                // We now need to find the minimum distance
                for (int k = 0; k < NUM_OF_RECTANGLES_X; k++) // For all the intersections
                {
                    distance = (3 + 4 * k) * cellXsize - cars_horizontal[i][j].pos.x;
                    if (distance >= 0 && distance < min_distance)
                        min_distance = distance;
                }
                if (min_distance < cars_horizontal[i][j].speed + 1) // If we are close enough to an intersection and we are before of it.
                {
                    cars_horizontal[i][j].speed = 0; // Stop the car
                }
            }
        // For odd roads
        for (int i = 1; i < NUM_OF_RECTANGLES_Y + 1; i += 2)                     // for all odd x roads
            for (int j = 0; j < NUM_OF_NPC_CARS_ON_X_ROAD_ON_CURRENT_LEVEL; j++) // for all cars on some of those roads
            {
                if (cars_horizontal[i][j].is_visible == false) // If the cars are invisible go to the end of the loop and ignore all the changes
                    continue;
                // Now we need to remind to ourselves how we have set the parameters of the game and its core logic.
                float cellXsize = WINDOW_WIDTH / ((float)NUM_OF_RECTANGLES_X * 4); // Remember the by 4 architecture(see docs).

                float distance = 0, min_distance = 10000.0f;
                // We now need to find the minimum distance
                for (int k = 0; k < NUM_OF_RECTANGLES_X; k++) // For all the intersections
                {
                    distance = cars_horizontal[i][j].pos.x - (5 + 4 * k) * cellXsize; // For the odd roads stop points are different.
                    if (distance >= 0 && distance < min_distance)
                        min_distance = distance;
                }
                if (min_distance < cars_horizontal[i][j].speed + 1) // If we are close enough to an intersection and we are before of it.
                {
                    cars_horizontal[i][j].speed = 0; // Stop the car
                }
            }
    }

    if (Traffic_Cop == Horizontal_GO) // If green is for horizontal...
    {
        // If it is green for the horizontal cars let them go!!!
        for (int i = 0; i < NUM_OF_RECTANGLES_Y + 1; i++)                        // for all x roads
            for (int j = 0; j < NUM_OF_NPC_CARS_ON_X_ROAD_ON_CURRENT_LEVEL; j++) // for all cars on some of those roads
            {
                if (cars_horizontal[i][j].is_visible == false) // If the cars are invisible go to the end of the loop and ignore all the changes
                    continue;
                // If their speed is 0 give them speed!!!
                if (cars_horizontal[i][j].speed == 0)
                {
                    cars_horizontal[i][j].speed = (rand() % 25 + 10.0f) / 10.0f;
                }
            }

        // Traffic light logic
        //  For even roads
        for (int i = 0; i < NUM_OF_RECTANGLES_X + 1; i += 2)                     // for all even x roads
            for (int j = 0; j < NUM_OF_NPC_CARS_ON_Y_ROAD_ON_CURRENT_LEVEL; j++) // for all cars on some of those roads
            {
                if (cars_vertical[i][j].is_visible == false) // If the cars are invisible go to the end of the loop and ignore all the changes
                    continue;
                // Now we need to remind to ourselves how we have set the parameters of the game and its core logic.
                float cellYsize = WINDOW_HEIGHT / ((float)NUM_OF_RECTANGLES_Y * 4); // Remember the by 4 architecture(see docs).

                float distance = 0, min_distance = 10000.0f;
                // We now need to find the minimum distance
                for (int k = 0; k < NUM_OF_RECTANGLES_Y; k++) // For all the intersections
                {
                    distance = (3 + 4 * k) * cellYsize - cars_vertical[i][j].pos.y;
                    if (distance >= 0 && distance < min_distance)
                        min_distance = distance;
                }
                if (min_distance < cars_vertical[i][j].speed + 1) // If we are close enough to an intersection and we are before of it.
                {
                    cars_vertical[i][j].speed = 0; // Stop the car
                }
            }
        // For odd roads
        for (int i = 1; i < NUM_OF_RECTANGLES_X + 1; i += 2)                     // for all odd x roads
            for (int j = 0; j < NUM_OF_NPC_CARS_ON_Y_ROAD_ON_CURRENT_LEVEL; j++) // for all cars on some of those roads
            {
                if (cars_vertical[i][j].is_visible == false) // If the cars are invisible go to the end of the loop and ignore all the changes
                    continue;
                // Now we need to remind to ourselves how we have set the parameters of the game and its core logic.
                float cellYsize = WINDOW_HEIGHT / ((float)NUM_OF_RECTANGLES_Y * 4); // Remember the by 4 architecture(see docs).

                float distance = 0, min_distance = 10000.0f;
                // We now need to find the minimum distance
                for (int k = 0; k < NUM_OF_RECTANGLES_Y; k++) // For all the intersections
                {
                    distance = cars_vertical[i][j].pos.y - (5 + 4 * k) * cellYsize; // For the odd roads stop points are different.
                    if (distance >= 0 && distance < min_distance)
                        min_distance = distance;
                }
                if (min_distance < cars_vertical[i][j].speed + 1) // If we are close enough to an intersection and we are before of it.
                {
                    cars_vertical[i][j].speed = 0; // Stop the car
                }
            }
    }

    if (timer_counter > 180) // Every 3 seconds
    {
        timer_counter = 0;
        if (Traffic_Cop == Vertical_GO)
            Traffic_Cop = Horizontal_GO;
        else
            Traffic_Cop = Vertical_GO;
    }
}