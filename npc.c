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
int npc_smart_counter = 0; // This is a counter variable that tracks how many times before the NPC changes its target to match the players position. It is the NPC's update time.
Vector2 target_npc_pos = {0};//This is the old pos used in the update NPC and is the NPC target pos

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