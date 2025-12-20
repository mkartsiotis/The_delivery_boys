#include "headers.h" //Including the header file

int main(void)
{
    InitWindow(GetScreenWidth(), GetScreenHeight(), "The Delivery Man V0.2.0");
    HideCursor(); 
    SetTargetFPS(60);
/*This is the section where we initialize all the variables*/
//First Create an array of RECTANGLES
//For this we need to define a global constant variable which can be static since is constant and inline to just manage memmory efficiently
//See the header file for more info
    Rectangle map[NUM_OF_RECTANGLES_X][NUM_OF_RECTANGLES_Y];
    Initialize_Map(&map);




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
        
        //Update Variables Section


        //Draw section
        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawRectangles(map);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}