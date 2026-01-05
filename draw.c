#include "headers.h"

void DrawRectangles(Rectangle map[NUM_OF_RECTANGLES_Y][NUM_OF_RECTANGLES_X])
{
    for (int i = 0; i < NUM_OF_RECTANGLES_Y; i++)
        for (int j = 0; j < NUM_OF_RECTANGLES_X; j++)
            DrawRectangle(map[i][j].x, map[i][j].y, SIZE_OF_RECTANGLES_X, SIZE_OF_RECTANGLES_Y, RED);
}

// In draw.c
void DrawCubes(Rectangle map[NUM_OF_RECTANGLES_Y][NUM_OF_RECTANGLES_X])
{
    for (int i = 0; i < NUM_OF_RECTANGLES_Y; i++)
        for (int j = 0; j < NUM_OF_RECTANGLES_X; j++)
        {
            float width = SIZE_OF_RECTANGLES_X;
            float length = SIZE_OF_RECTANGLES_Y; // The "height" in 2D is length in 3D

            // OFFSET MATH: Move the center to the middle of the rectangle
            float centerX = map[i][j].x + SIZE_OF_RECTANGLES_X / 2.0f;
            float centerZ = map[i][j].y + SIZE_OF_RECTANGLES_Y / 2.0f;
            DrawCube((Vector3){centerX, ((float)SIZE_OF_RECTANGLES_3DHEIGHT / 2.0f), centerZ}, width, SIZE_OF_RECTANGLES_3DHEIGHT, length, RED);
            DrawCubeWires((Vector3){centerX, ((float)SIZE_OF_RECTANGLES_3DHEIGHT / 2.0f), centerZ}, width, SIZE_OF_RECTANGLES_3DHEIGHT, length, BLACK);
        }
}
void draw_pickup_and_dropoff(Vector2 PICKUP, Vector2 DROPOFF)
{
    DrawCircle(PICKUP.x, PICKUP.y, 4, YELLOW);
    DrawCircle(DROPOFF.x, DROPOFF.y, 4, GREEN);
}
void draw_pickup_and_dropoff3D(Vector2 PICKUP, Vector2 DROPOFF)
{
    DrawCylinder((Vector3){PICKUP.x, 0.0f, PICKUP.y}, 3, 6, 3.0f, 30, YELLOW);
    DrawCylinder((Vector3){DROPOFF.x, 0.0f, DROPOFF.y}, 3, 6, 3.0f, 30, GREEN);
}

void Draw_and_update_score_window(int sucessful_deliveries, FILE *file, ScreenStatus GameScreen)
{
    Color Mycolour = Fade(BLUE, 0.2f);
    DrawRectangle(0, 0, WINDOW_WIDTH, 50, Mycolour);
    char score[15] = {0};
    sprintf(score, "SCORE IS : %d", sucessful_deliveries);
    DrawText(score, 25, 25, 20, WHITE);
    // Find HIGH SCORE and READ IT
    int HIGH_SCORE1 = 0, HIGH_SCORE2 = 0, HIGH_SCORE3 = 0;
    fseek(file, 0, 0);
    fscanf(file, "-%d-%d-%d-", &HIGH_SCORE1, &HIGH_SCORE2, &HIGH_SCORE3);
    char high_score_text[20] = {0};
    if (GameScreen.CurrentScreen == LEVEL1)
        sprintf(high_score_text, "HIGHSCORE: %d", HIGH_SCORE1); // Print the highscore relevant to the level
    else if (GameScreen.CurrentScreen == LEVEL2)
        sprintf(high_score_text, "HIGHSCORE: %d", HIGH_SCORE2);
    else if (GameScreen.CurrentScreen == LEVEL3)
        sprintf(high_score_text, "HIGHSCORE: %d", HIGH_SCORE3);

    DrawText(high_score_text, 300, 25, 20, WHITE);
}
void draw_fuel_bar(void) //(In draw.c)Draws the remaining fuel in the depoisit.
{
    DrawRectangleLines(1800, (float)WINDOW_HEIGHT * 2.0f / 3.0f, 20, 100, WHITE);
    Color tankcol;
    if(gas > 80)
        tankcol = GREEN;
    else 
        tankcol = RED;
    DrawRectangle(1800, ((float)WINDOW_HEIGHT * 2.0f / 3.0f + 100 - (gas / (float)INITIAL_GASOLINE * 100)), 20, (gas / (float)INITIAL_GASOLINE * 100), tankcol);
}
void draw_grid(void) //(In draw.c)Draws the grid of the big map in world-map coordinates. Note that this function does not draw the lines of the coordinates of the grid[i][j] but the outside sides of the rectangles that represent a 2D division of the map plane.
{
    float stepX = (float)MAP_WIDTH / (4 * NUM_OF_RECTANGLES_X);
    float stepY = (float)MAP_HEIGHT / (4 * NUM_OF_RECTANGLES_Y);
    for (int i = 1; i < 4 * NUM_OF_RECTANGLES_Y; i++) // For every iteration of the number of divisions of the plane:
        DrawLine3D((Vector3){0, 0, i * stepY}, (Vector3){MAP_WIDTH, 0, i * stepY}, YELLOW);

    for (int j = 1; j < 4 * NUM_OF_RECTANGLES_X; j++) // Be carefull!We are starting from 1(If I use the function later)

        DrawLine3D((Vector3){j * stepX, 0, 0}, (Vector3){j * stepX, 0, MAP_HEIGHT}, YELLOW);
}
void draw_astar_results(best_possible_path A_STAR_RESULT) // Decodes the string of the A* results.
{
    float sizeofboxX = MAP_WIDTH / (4.0f * NUM_OF_RECTANGLES_X), sizeofboxY = MAP_HEIGHT / (4.0f * NUM_OF_RECTANGLES_Y);
    if (A_STAR_RESULT.result == 0) // if result is valid
    {
        for (int i = 0; i < A_STAR_RESULT.number_of_points - 1; i++)
        {
            float xval = 0, yval = 0, nextxval = 0, nextyval = 0;
            xval = sizeofboxX * ((float)A_STAR_RESULT.MATRIX_OUT[i][0] + 0.5f); // See documentation why this holds(lets hope it does)
            yval = sizeofboxY * ((float)A_STAR_RESULT.MATRIX_OUT[i][1] + 0.5f);
            nextxval = sizeofboxX * ((float)A_STAR_RESULT.MATRIX_OUT[i + 1][0] + 0.5f);
            nextyval = sizeofboxY * ((float)A_STAR_RESULT.MATRIX_OUT[i + 1][1] + 0.5f);
            DrawLine(xval, yval, nextxval, nextyval, YELLOW);
        }
    }
}

void draw_astar_results3D(best_possible_path A_STAR_RESULT) // Decodes the string of the A* results.
{
    float sizeofboxX = MAP_WIDTH / (4.0f * NUM_OF_RECTANGLES_X), sizeofboxY = MAP_HEIGHT / (4.0f * NUM_OF_RECTANGLES_Y);
    if (A_STAR_RESULT.result == 0) // if result is valid
    {
        for (int i = 0; i < A_STAR_RESULT.number_of_points - 1; i++)
        {
            float xval = 0, yval = 0, nextxval = 0, nextyval = 0;
            xval = sizeofboxX * ((float)A_STAR_RESULT.MATRIX_OUT[i][0] + 0.5f); // See documentation why this holds(lets hope it does)
            yval = sizeofboxY * ((float)A_STAR_RESULT.MATRIX_OUT[i][1] + 0.5f);
            nextxval = sizeofboxX * ((float)A_STAR_RESULT.MATRIX_OUT[i + 1][0] + 0.5f);
            nextyval = sizeofboxY * ((float)A_STAR_RESULT.MATRIX_OUT[i + 1][1] + 0.5f);
            DrawLine3D((Vector3){xval, 0, yval}, (Vector3){nextxval, 0, nextyval}, YELLOW);
        }
    }
}

void draw_npc3D(NPC chaser)
{
    DrawCube((Vector3){chaser.position.x, 2.0f, chaser.position.y}, chaser.WIDTH, 2.0f, chaser.HEIGHT, ORANGE);
    DrawCubeWires((Vector3){chaser.position.x, 2.0f, chaser.position.y}, chaser.WIDTH, 2.0f, chaser.HEIGHT, BLACK);
}
void draw_npc(NPC chaser)
{
    int drawx = chaser.position.x - chaser.WIDTH / 2; // Calculate the pos of the top-left part
    int drawy = chaser.position.y - chaser.HEIGHT / 2;
    DrawRectangle(drawx, drawy, chaser.WIDTH * 5, chaser.HEIGHT * 5, ORANGE);
}
void drawspeed(void) //(in draw.c). Draws a speedometer.
{
    DrawRing((Vector2){WINDOW_WIDTH / 2.0f, 110}, 40, 50, 0, -180, 0, BLACK);
    DrawRing((Vector2){WINDOW_WIDTH / 2.0f, 110}, 42, 48, -180, -180 + 180 * speed / 4, 0, WHITE);
    char ch[30] = {0};
    sprintf(ch, "%.0f", speed * 10);
    DrawText(ch, (WINDOW_WIDTH / 2.0f) - (MeasureText(ch, 20) / 2.0f), 105, 20, GREEN);
}
void draw_cars(void) // Draws all the cars.
{
    for (int i = 0; i < NUM_OF_RECTANGLES_Y + 1; i++)       // for all x roads
        for (int j = 0; j < NUM_OF_NPC_CARS_ON_X_ROAD; j++) // for all cars on those roads
            if (cars_horizontal[i][j].is_visible == true)
            {
                DrawCube((Vector3){cars_horizontal[i][j].pos.x, cars_horizontal[i][j].sizeZ / 2.0f, cars_horizontal[i][j].pos.y}, cars_horizontal[i][j].sizeX, cars_horizontal[i][j].sizeZ, cars_horizontal[i][j].sizeY, cars_horizontal[i][j].col);
            }
    for (int i = 0; i < NUM_OF_RECTANGLES_X + 1; i++)       // for all y roads
        for (int j = 0; j < NUM_OF_NPC_CARS_ON_Y_ROAD; j++) // for all cars on those roads
            if (cars_vertical[i][j].is_visible == true)
            {
                DrawCube((Vector3){cars_vertical[i][j].pos.x, cars_vertical[i][j].sizeZ / 2.0f, cars_vertical[i][j].pos.y}, cars_vertical[i][j].sizeX, cars_vertical[i][j].sizeZ, cars_vertical[i][j].sizeY, cars_vertical[i][j].col);
            }
}

void draw_mission_score(void) // Draws the score that is going to be awarded if no more points are deducted
{
    Font font = LoadFontEx("OldNewspaperTypes.ttf", 20, 0, 0);
    char ch[50] = {0};
    int fps = GetFPS();
    sprintf(ch, "MISSION REWARDS: %d FPS: %d", score_for_current_mission, fps);
    DrawTextEx(font, ch, (Vector2){800, 25}, 20, 2, WHITE); // Draw
}