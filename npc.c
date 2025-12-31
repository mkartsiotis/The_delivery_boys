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
npc_car cars_vertical[NUM_OF_RECTANGLES_X + 1][NUM_OF_NPC_CARS_ON_Y_ROAD] = {false, NPC_CAR_CEMETARY, NPC_CAR_CEMETARY, NPC_CAR_CEMETARY, SIZE_OF_CAR_X, SIZE_OF_CAR_Y, SIZE_OF_CAR_Z, PURPLE};
npc_car cars_horizontal[NUM_OF_RECTANGLES_Y + 1][NUM_OF_NPC_CARS_ON_X_ROAD] = {false, NPC_CAR_CEMETARY, NPC_CAR_CEMETARY, NPC_CAR_CEMETARY, SIZE_OF_CAR_X, SIZE_OF_CAR_Y, SIZE_OF_CAR_Z, PURPLE};
; // Create an array of cars for the X and Y axis respectively.

void updateNPC(NPC *chaser, Vector2 player_pos, Rectangle map[NUM_OF_RECTANGLES_Y][NUM_OF_RECTANGLES_X]) //(In npc.c)Checks conditions and recalculates path if needed.
{
    // SIMPLE_STRAIGHTFORWARD_APPROACH
    // 1.FIND_REALTIVE_POSITION
    // 2.CALCULATE REqUESTED MOVE
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
The approach: Create a conveyer belt system where the player-npc starts from point A and drives straight to point B untill it reaches that and disappears.
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
    // We divide the screen dimensions with the NUMBER_OF_RECTANGLES * 4 in each axis
    // Then for the nth block in the x axis(same logic for the y axis) starting from 0 untill NUMBER_OF_RECTANGLES - 1 we have:
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

    float tolerance_for_x_road = roadlength_x / (float)NUM_OF_NPC_CARS_ON_X_ROAD;
    float tolerance_for_y_road = roadlength_y / (float)NUM_OF_NPC_CARS_ON_Y_ROAD;

    // First check for the x axis(x roads)if there are any visible
    for (int i = 0; i < NUM_OF_RECTANGLES_Y + 1; i += 2) // for all even x roads
    {
        float min_pos_x = 10000;

        // First apply the move
        //  Now for all visible cars update their position.
        for (int j = 0; j < NUM_OF_NPC_CARS_ON_X_ROAD; j++) // for all cars on some of those roads
            if (cars_horizontal[i][j].is_visible == true)
            {
                // Check if we meet the threshold.
                if (cars_horizontal[i][j].pos.x < cars_horizontal[i][j].end_pos.x)
                {
                    cars_horizontal[i][j].pos.x += 2.0f;
                }
                else
                {
                    cars_horizontal[i][j].pos = NPC_CAR_CEMETARY;
                    cars_horizontal[i][j].is_visible = false;
                    cars_horizontal[i][j].start_pos = NPC_CAR_CEMETARY;
                    cars_horizontal[i][j].end_pos = NPC_CAR_CEMETARY;
                    cars_horizontal[i][j].col = choseRandomColour();
                    printf("LOST CAR\n");
                }
            }
        // Then check the minimum distance to add a car if needed
        for (int j = 0; j < NUM_OF_NPC_CARS_ON_X_ROAD; j++) // for all cars on those roads
            if (cars_horizontal[i][j].is_visible == true)
            {
                if (cars_horizontal[i][j].pos.x < min_pos_x)
                {
                    min_pos_x = cars_horizontal[i][j].pos.x;
                }
            }
        // Now if the minimum distance is larger than the thershold or if no car is in the map initialize a new car.
        if (min_pos_x < tolerance_for_x_road && min_pos_x != -1)
            ;
        else
        {
            // Make visible a new car.
            int invisible_car_num = -1;
            for (int j = 0; j < NUM_OF_NPC_CARS_ON_X_ROAD; j++) // for all cars on those roads
                if (cars_horizontal[i][j].is_visible == false)
                {
                    invisible_car_num = j;
                    break;
                }
            if (invisible_car_num == -1)
                continue;
            // Now put the car in the road start. Make it visible
            cars_horizontal[i][invisible_car_num].pos = (Vector2){0, stepY * 4 * i};
            cars_horizontal[i][invisible_car_num].is_visible = true;
            cars_horizontal[i][invisible_car_num].start_pos = (Vector2){0, stepY * 4 * i};
            cars_horizontal[i][invisible_car_num].end_pos = (Vector2){roadlength_x, stepY * 4 * i};
            cars_horizontal[i][invisible_car_num].col = choseRandomColour();
            printf("MADE NEW CAR VISIBLE! IN ROAD %d \n", i);
        }
    }

    // Do the same for odd roads.
    for (int i = 1; i < NUM_OF_RECTANGLES_Y + 1; i += 2) // for all even x roads
    {
        float max_pos_x = -10000;

        // First apply the move
        //  Now for all visible cars update their position.
        for (int j = 0; j < NUM_OF_NPC_CARS_ON_X_ROAD; j++) // for all cars on some of those roads
            if (cars_horizontal[i][j].is_visible == true)
            {
                // Check if we meet the threshold.
                if (cars_horizontal[i][j].pos.x > cars_horizontal[i][j].end_pos.x)
                {
                    cars_horizontal[i][j].pos.x -= 2.0f;
                }
                else
                {
                    cars_horizontal[i][j].pos = NPC_CAR_CEMETARY;
                    cars_horizontal[i][j].is_visible = false;
                    cars_horizontal[i][j].start_pos = NPC_CAR_CEMETARY;
                    cars_horizontal[i][j].end_pos = NPC_CAR_CEMETARY;
                    cars_horizontal[i][j].col = choseRandomColour();
                    printf("LOST CAR\n");
                }
            }
        // Then check the minimum distance to add a car if needed
        for (int j = 0; j < NUM_OF_NPC_CARS_ON_X_ROAD; j++) // for all cars on those roads
            if (cars_horizontal[i][j].is_visible == true)
            {
                if (cars_horizontal[i][j].pos.x > max_pos_x) // FIND THE CAR WITH THE MAXIMUM POSITION
                {
                    max_pos_x = cars_horizontal[i][j].pos.x;
                }
            }
        // Now if the minimum distance is larger than the thershold or if no car is in the map initialize a new car.
        if (MAP_WIDTH - max_pos_x < tolerance_for_x_road && max_pos_x != -10000)
            ;
        else
        {
            // Make visible a new car.
            int invisible_car_num = -1;
            for (int j = 0; j < NUM_OF_NPC_CARS_ON_X_ROAD; j++) // for all cars on those roads
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
            cars_horizontal[i][invisible_car_num].col = choseRandomColour();
            printf("MADE NEW CAR VISIBLE! IN ROAD %d \n", i);
        }
    }
    // Do the same for the Y axis.
    //  First check for the x axis(x roads)if there are any visible
    for (int i = 0; i < NUM_OF_RECTANGLES_X + 1; i += 2) // for all y roads
    {
        float min_pos_y = 10000;

        // First apply the move
        //  Now for all visible cars update their position.
        for (int j = 0; j < NUM_OF_NPC_CARS_ON_Y_ROAD; j++) // for all cars on those roads
            if (cars_vertical[i][j].is_visible == true)
            {
                // Check if we meet the threshold.
                if (cars_vertical[i][j].pos.y < cars_vertical[i][j].end_pos.y)
                {
                    cars_vertical[i][j].pos.y += 2.0f;
                }
                else
                {
                    cars_vertical[i][j].pos = NPC_CAR_CEMETARY;
                    cars_vertical[i][j].is_visible = false;
                    cars_vertical[i][j].start_pos = NPC_CAR_CEMETARY;
                    cars_vertical[i][j].end_pos = NPC_CAR_CEMETARY;
                    cars_vertical[i][j].col = choseRandomColour();
                    printf("LOST CAR\n");
                }
            }
        // Then check the minimum distance to add a car if needed
        for (int j = 0; j < NUM_OF_NPC_CARS_ON_Y_ROAD; j++) // for all cars on those roads
            if (cars_vertical[i][j].is_visible == true)
            {
                if (cars_vertical[i][j].pos.y < min_pos_y)
                {
                    min_pos_y = cars_vertical[i][j].pos.y;
                }
            }
        // Now if the minimum distance is larger than the thershold or if no car is in the map initialize a new car.
        if (min_pos_y < tolerance_for_y_road && min_pos_y != -1)
            ;
        else
        {
            // Make visible a new car.
            int invisible_car_num = -1;
            for (int j = 0; j < NUM_OF_NPC_CARS_ON_Y_ROAD; j++) // for all cars on those roads
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
            cars_vertical[i][invisible_car_num].col = choseRandomColour();
            printf("MADE NEW CAR VISIBLE! IN ROAD %d \n", i);
        }
    }
    // Do the same for odd roads in the Y axis.
    for (int i = 1; i < NUM_OF_RECTANGLES_X + 1; i += 2) // for all even x roads
    {
        float max_pos_y = -10000;

        // First apply the move
        //  Now for all visible cars update their position.
        for (int j = 0; j < NUM_OF_NPC_CARS_ON_Y_ROAD; j++) // for all cars on some of those roads
            if (cars_vertical[i][j].is_visible == true)
            {
                // Check if we meet the threshold.
                if (cars_vertical[i][j].pos.y > cars_vertical[i][j].end_pos.y)
                {
                    cars_vertical[i][j].pos.y -= 2.0f;
                }
                else
                {
                    cars_vertical[i][j].pos = NPC_CAR_CEMETARY;
                    cars_vertical[i][j].is_visible = false;
                    cars_vertical[i][j].start_pos = NPC_CAR_CEMETARY;
                    cars_vertical[i][j].end_pos = NPC_CAR_CEMETARY;
                    cars_vertical[i][j].col = choseRandomColour();
                    printf("LOST CAR\n");
                }
            }
        // Then check the minimum distance to add a car if needed
        for (int j = 0; j < NUM_OF_NPC_CARS_ON_X_ROAD; j++) // for all cars on those roads
            if (cars_vertical[i][j].is_visible == true)
            {
                if (cars_vertical[i][j].pos.y > max_pos_y) // FIND THE CAR WITH THE MAXIMUM POSITION
                {
                    max_pos_y = cars_vertical[i][j].pos.y;
                }
            }
        // Now if the minimum distance is larger than the thershold or if no car is in the map initialize a new car.
        if (MAP_HEIGHT - max_pos_y < tolerance_for_y_road && max_pos_y != -10000)
            ;
        else
        {
            // Make visible a new car.
            int invisible_car_num = -1;
            for (int j = 0; j < NUM_OF_NPC_CARS_ON_Y_ROAD; j++) // for all cars on those roads
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
            cars_vertical[i][invisible_car_num].col = choseRandomColour();
            printf("MADE NEW CAR VISIBLE! IN ROAD %d \n", i);
        }
    }
}

void init_cars(void)
{
    for (int i = 0; i < NUM_OF_RECTANGLES_Y + 1; i++)       // for all x roads
        for (int j = 0; j < NUM_OF_NPC_CARS_ON_X_ROAD; j++) // for all cars on those roads
            cars_horizontal[i][j] = (npc_car){false, NPC_CAR_CEMETARY, NPC_CAR_CEMETARY, NPC_CAR_CEMETARY, SIZE_OF_CAR_X, SIZE_OF_CAR_Y, SIZE_OF_CAR_Z, PURPLE};
    for (int i = 0; i < NUM_OF_RECTANGLES_X + 1; i++)       // for all y roads
        for (int j = 0; j < NUM_OF_NPC_CARS_ON_Y_ROAD; j++) // for all cars on those roads
            cars_vertical[i][j] = (npc_car){false, NPC_CAR_CEMETARY, NPC_CAR_CEMETARY, NPC_CAR_CEMETARY, SIZE_OF_CAR_X, SIZE_OF_CAR_Y, SIZE_OF_CAR_Z, PURPLE};
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