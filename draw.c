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
#include "headers.h"

// Initialize external models
Model GasStationModel = {0};
Model Building = {0};
Model playerModel = {0};
Model Chaser = {0};
Model NPCmodel1 = {0};
Model NPCmodel2 = {0};
Model NPCmodel3 = {0};
Model Fractals = {0};

void DrawRectangles(Rectangle map[NUM_OF_RECTANGLES_Y][NUM_OF_RECTANGLES_X])
{
    for (int i = 0; i < NUM_OF_RECTANGLES_Y; i++)
        for (int j = 0; j < NUM_OF_RECTANGLES_X; j++)
            DrawRectangle(map[i][j].x, map[i][j].y, SIZE_OF_RECTANGLES_X, SIZE_OF_RECTANGLES_Y, RED);
}

// In draw.c
// In draw.c
void DrawCubes(Rectangle map[NUM_OF_RECTANGLES_Y][NUM_OF_RECTANGLES_X], Vector2 playerPos)
{
    BoundingBox box = GetModelBoundingBox(Building);
    float modelWidth = box.max.x - box.min.x;
    float modelHeight = box.max.y - box.min.y;
    float modelLength = box.max.z - box.min.z;

    if (modelWidth == 0.0f)
        modelWidth = 1.0f;

    float scaleX_Axis = (SIZE_OF_RECTANGLES_X / modelWidth);
    float scaleZ_Axis = (SIZE_OF_RECTANGLES_Y / modelLength);
    float scaleY_Axis = (SIZE_OF_RECTANGLES_3DHEIGHT / modelHeight);

    float yOffset = -box.min.y * scaleY_Axis;

    float localCenterX = (box.min.x + box.max.x) / 2.0f;
    float localCenterZ = (box.min.z + box.max.z) / 2.0f;

    float xOffset = -localCenterX * scaleX_Axis;
    float zOffset = -localCenterZ * scaleZ_Axis;

    float drawDistance = 1200.0f;

    for (int i = 0; i < NUM_OF_RECTANGLES_Y; i++)
    {
        for (int j = 0; j < NUM_OF_RECTANGLES_X; j++)
        {
            float mapCenterX = map[i][j].x + SIZE_OF_RECTANGLES_X / 2.0f;
            float mapCenterZ = map[i][j].y + SIZE_OF_RECTANGLES_Y / 2.0f;

            float distX = mapCenterX - playerPos.x;
            float distZ = mapCenterZ - playerPos.y;

            if ((distX * distX + distZ * distZ) > (drawDistance * drawDistance))
            {
                continue;
            }

            Vector3 finalPos = {
                mapCenterX + xOffset,
                +yOffset,
                mapCenterZ + zOffset};

            DrawModelEx(Building,
                        finalPos,
                        (Vector3){0, 1, 0},
                        0.0f,
                        (Vector3){scaleX_Axis, scaleY_Axis, scaleZ_Axis},
                        WHITE);
        }
    }
}

void draw_pickup_and_dropoff(Vector2 PICKUP, Vector2 DROPOFF)
{
    DrawCircle(PICKUP.x, PICKUP.y, 10, YELLOW);
    DrawCircle(DROPOFF.x, DROPOFF.y, 10, GREEN);
}
void draw_pickup_and_dropoff3D(Vector2 PICKUP, Vector2 DROPOFF)
{
    DrawCylinder((Vector3){
                     PICKUP.x, 0.0f, PICKUP.y},
                 3, 6, 3.0f, 30, YELLOW);
    DrawCylinder((Vector3){
                     DROPOFF.x, 0.0f, DROPOFF.y},
                 3, 6, 3.0f, 30, GREEN);
}
void Draw_and_update_score_window(int sucessful_deliveries, int HIGH, ScreenStatus GameScreen)
{
    Color Mycolour = Fade(BLUE, 0.2f);
    DrawRectangle(0, 0, WINDOW_WIDTH, 50, Mycolour);
    char score[15] = {0};
    sprintf(score, "SCORE: %d", sucessful_deliveries);
    DrawText(score, 25, 25, 20, WHITE);
    // Find HIGH SCORE and READ IT
    char high_score_text[50] = {0};
    if (GameScreen.CurrentScreen == LEVEL1)
        sprintf(high_score_text, "%d / 600 COINS", HIGH + sucessful_deliveries); // Print the highscore relevant to the level
    else if (GameScreen.CurrentScreen == LEVEL2)
        sprintf(high_score_text, "%d / 1200 COINS", HIGH + sucessful_deliveries); // Print the highscore relevant to the level
    else if (GameScreen.CurrentScreen == LEVEL3)
        sprintf(high_score_text, "%d / 2000 COINS", HIGH + sucessful_deliveries); // Print the highscore relevant to the level
    DrawText(high_score_text, 300, 25, 20, YELLOW);
}
void draw_fuel_bar(void) //(In draw.c)Draws the remaining fuel in the depoisit.
{
    DrawRectangleLines(1800, (float)WINDOW_HEIGHT * 2.0f / 3.0f, 20, 100, WHITE);
    Color tankcol;
    if (gas > 80)
        tankcol = GREEN;
    else
        tankcol = RED;
    DrawRectangle(1800, ((float)WINDOW_HEIGHT * 2.0f / 3.0f + 100 - (gas / (float)INITIAL_GASOLINE * 100)), 19, (gas / (float)INITIAL_GASOLINE * 100), tankcol);
    char gasText[10] = {0};
    sprintf(gasText, "FUEL");
    int length = MeasureText(gasText, 20);
    DrawText(gasText, 1810 - (length / 2), (float)WINDOW_HEIGHT * 2.0f / 3.0f - 30, 20, GREEN);
}
void draw_grid(void) //(In draw.c)Draws the grid of the big map in world-map coordinates. Note that this function does not draw the lines of the coordinates of the grid[i][j] but the outside sides of the rectangles that represent a 2D division of the map plane.
{
    float stepX = (float)MAP_WIDTH / (4 * NUM_OF_RECTANGLES_X);
    float stepY = (float)MAP_HEIGHT / (4 * NUM_OF_RECTANGLES_Y);
    for (int i = 1; i < 4 * NUM_OF_RECTANGLES_Y; i++) // For every iteration of the number of divisions of the plane:
        DrawLine3D((Vector3){
                       0, 0, i * stepY},
                   (Vector3){MAP_WIDTH, 0, i * stepY}, YELLOW);

    for (int j = 1; j < 4 * NUM_OF_RECTANGLES_X; j++) // Be careful!We are starting from 1(If I use the function later)
        DrawLine3D((Vector3){
                       j * stepX, 0, 0},
                   (Vector3){j * stepX, 0, MAP_HEIGHT}, YELLOW);
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
            xval = sizeofboxX * ((float)A_STAR_RESULT.MATRIX_OUT[i][0] + 0.5f); // See documentation why this holds(lets hope it does. Ask us since we did m=not have the time   )
            yval = sizeofboxY * ((float)A_STAR_RESULT.MATRIX_OUT[i][1] + 0.5f);
            nextxval = sizeofboxX * ((float)A_STAR_RESULT.MATRIX_OUT[i + 1][0] + 0.5f);
            nextyval = sizeofboxY * ((float)A_STAR_RESULT.MATRIX_OUT[i + 1][1] + 0.5f);
            float brightness = (sin(GetTime() * 5.0f) + 1.0f) / 2.0f; // Range 0.0 to 1.0
            float hue = 60.0f;
            Color yellowShade = ColorFromHSV(hue, 1.0f, brightness);
            DrawLine3D((Vector3){
                           xval, 0, yval},
                       (Vector3){nextxval, 0, nextyval}, yellowShade);
        }
    }
}

void draw_npc3D(NPC chaser)
{

    Vector3 pos3D = {chaser.position.x, 2.0f, chaser.position.y}; // 3D position
    DrawModelEx(Chaser, pos3D, (Vector3){0, 1.0f, 0.0f}, chaser_angle, (Vector3){1.0f, 1.0f, 1.0f}, WHITE);
}
void draw_npc(NPC chaser)
{
    int drawx = chaser.position.x - chaser.WIDTH / 2; // Calculate the pos of the top-left part
    int drawy = chaser.position.y - chaser.HEIGHT / 2;
    DrawRectangle(drawx, drawy, chaser.WIDTH * 5, chaser.HEIGHT * 5, ORANGE);
}
void drawspeed(void) //(in draw.c). Draws a speedometer.
{
    DrawRing((Vector2){
                 WINDOW_WIDTH / 2.0f, 110},
             40, 50, 0, -180, 0, BLACK);
    DrawRing((Vector2){
                 WINDOW_WIDTH / 2.0f, 110},
             42, 48, -180, -180 + 180 * speed / 4, 0, WHITE);
    char ch[30] = {0};
    sprintf(ch, "%.0f", speed * 10);
    DrawText(ch, (WINDOW_WIDTH / 2.0f) - (MeasureText(ch, 20) / 2.0f), 105, 20, GREEN);
}
void draw_cars(Vector3 camera3d) // Draws all the cars.
{
    // 1. HORIZONTAL ROADS (Even)
    for (int i = 0; i < NUM_OF_RECTANGLES_Y + 1; i += 2)
    {
        for (int j = 0; j < NUM_OF_NPC_CARS_ON_X_ROAD; j++)
        {
            if (cars_horizontal[i][j].is_visible == 0)
                continue;

            if (cars_horizontal[i][j].has_been_assigned == 0)
            {
                cars_horizontal[i][j].has_been_assigned = GetRandomValue(1, 3);
            }

            Vector3 pos3D = {cars_horizontal[i][j].pos.x, cars_horizontal[i][j].sizeZ / 2.0f, cars_horizontal[i][j].pos.y};
            if (fabs(camera3d.x - cars_horizontal[i][j].pos.x) < 600 && fabs(camera3d.z - cars_horizontal[i][j].pos.y) < 600)
                switch (cars_horizontal[i][j].has_been_assigned)
                {
                case 1:
                    DrawModelEx(NPCmodel1, pos3D, (Vector3){0.0f, 1.0f, 0.0f}, 0.0f, (Vector3){1.0f, 1.0f, 1.0f}, WHITE);
                    break;
                case 2:
                    DrawModelEx(NPCmodel2, pos3D, (Vector3){0.0f, 1.0f, 0.0f}, 90.0f, (Vector3){1.0f, 1.0f, 1.0f}, WHITE);
                    break;
                case 3:
                    DrawModelEx(NPCmodel3, pos3D, (Vector3){0.0f, 1.0f, 0.0f}, +90.0f, (Vector3){1.0f, 1.0f, 1.0f}, WHITE);
                    break;
                default:
                    break;
                }
        }
    }

    // HORIZONTAL ROADS (Odd Rows)
    for (int i = 1; i < NUM_OF_RECTANGLES_Y + 1; i += 2)
    {
        for (int j = 0; j < NUM_OF_NPC_CARS_ON_X_ROAD; j++)
        {
            if (cars_horizontal[i][j].is_visible == 0)
                continue;

            if (cars_horizontal[i][j].has_been_assigned == 0)
            {
                cars_horizontal[i][j].has_been_assigned = GetRandomValue(1, 3);
            }

            Vector3 pos3D = {cars_horizontal[i][j].pos.x, cars_horizontal[i][j].sizeZ / 2.0f, cars_horizontal[i][j].pos.y};
            if (fabs(camera3d.x - cars_horizontal[i][j].pos.x) < 600 && fabs(camera3d.z - cars_horizontal[i][j].pos.y) < 600)
                switch (cars_horizontal[i][j].has_been_assigned)
                {
                case 1:
                    DrawModelEx(NPCmodel1, pos3D, (Vector3){0.0f, 1.0f, 0.0f}, 180.0f, (Vector3){1.0f, 1.0f, 1.0f}, WHITE);
                    break;
                case 2:
                    DrawModelEx(NPCmodel2, pos3D, (Vector3){0.0f, 1.0f, 0.0f}, -90.0f, (Vector3){1.0f, 1.0f, 1.0f}, WHITE);
                    break;
                case 3:
                    DrawModelEx(NPCmodel3, pos3D, (Vector3){0.0f, 1.0f, 0.0f}, -90.0f, (Vector3){1.0f, 1.0f, 1.0f}, WHITE);
                    break;
                }
        }
    }

    // 3. VERTICAL ROADS (Even Columns)
    for (int i = 0; i < NUM_OF_RECTANGLES_X + 1; i += 2)
    {
        for (int j = 0; j < NUM_OF_NPC_CARS_ON_Y_ROAD; j++)
        {
            if (cars_vertical[i][j].is_visible == 0)
                continue;

            if (cars_vertical[i][j].has_been_assigned == 0)
            {
                cars_vertical[i][j].has_been_assigned = GetRandomValue(1, 3);
            }

            Vector3 pos3D = {cars_vertical[i][j].pos.x, cars_vertical[i][j].sizeZ / 2.0f, cars_vertical[i][j].pos.y};
            if (fabs(camera3d.x - cars_horizontal[i][j].pos.x) < 600 && fabs(camera3d.z - cars_vertical[i][j].pos.y) < 600)

                switch (cars_vertical[i][j].has_been_assigned)
                {
                case 1:
                    DrawModelEx(NPCmodel1, pos3D, (Vector3){0.0f, 1.0f, 0.0f}, -90.0f, (Vector3){1.0f, 1.0f, 1.0f}, WHITE);
                    break;
                case 2:
                    DrawModelEx(NPCmodel2, pos3D, (Vector3){0.0f, 1.0f, 0.0f}, 0.0f, (Vector3){1.0f, 1.0f, 1.0f}, WHITE);
                    break;
                case 3:
                    DrawModelEx(NPCmodel3, pos3D, (Vector3){0.0f, 1.0f, 0.0f}, 0.0f, (Vector3){1.0f, 1.0f, 1.0f}, WHITE);
                    break;
                }
        }
    }

    // 4. VERTICAL ROADS (Odd Columns)
    for (int i = 1; i < NUM_OF_RECTANGLES_X + 1; i += 2)
    {
        for (int j = 0; j < NUM_OF_NPC_CARS_ON_Y_ROAD; j++)
        {
            if (cars_vertical[i][j].is_visible == 0)
                continue;

            if (cars_vertical[i][j].has_been_assigned == 0)
            {
                cars_vertical[i][j].has_been_assigned = GetRandomValue(1, 3);
            }

            Vector3 pos3D = {cars_vertical[i][j].pos.x, cars_vertical[i][j].sizeZ / 2.0f, cars_vertical[i][j].pos.y};
            if (fabs(camera3d.x - cars_vertical[i][j].pos.x) < 600 && fabs(camera3d.z - cars_vertical[i][j].pos.y) < 600)

                switch (cars_vertical[i][j].has_been_assigned)
                {
                case 1:
                    DrawModelEx(NPCmodel1, pos3D, (Vector3){0.0f, 1.0f, 0.0f}, 90.0f, (Vector3){1.0f, 1.0f, 1.0f}, WHITE);
                    break;
                case 2:
                    DrawModelEx(NPCmodel2, pos3D, (Vector3){0.0f, 1.0f, 0.0f}, 180.0f, (Vector3){1.0f, 1.0f, 1.0f}, WHITE);
                    break;
                case 3:
                    DrawModelEx(NPCmodel3, pos3D, (Vector3){0.0f, 1.0f, 0.0f}, 180.0f, (Vector3){1.0f, 1.0f, 1.0f}, WHITE);
                    break;
                }
        }
    }
}

void draw_mission_score(int selected_mission_index) // Draws the score that is going to be awarded if no more points are deducted
{
    char ch[50] = {0};
    sprintf(ch, "MISSION REWARDS: %d", score_for_current_mission[selected_mission_index]);
    DrawText(ch, 800, 25, 20, WHITE); // Draw
}

// Refueling
void print_refuel_station(Gas_Station SET_STATION) //(In gamehandling.c)Prints the station if visible
{
    static int rotation_angle = 0; // Set the rotation angle
    rotation_angle += 2;           // Make the thing rotate!!!
    if (SET_STATION.isvisible == true)
    {
        Vector3 pos3D = {SET_STATION.REAL.x, 0.0f, SET_STATION.REAL.y}; // 3d position
        DrawModelEx(GasStationModel, pos3D, (Vector3){0, 1, 0}, rotation_angle, (Vector3){4.0f, 4.0f, 4.0f}, WHITE);
    }
}

// Color transition
Color LerpColor(Color start, Color end, float factor) // Fades a color
{
    if (factor < 0.0f)
        factor = 0.0f;
    if (factor > 1.0f)
        factor = 1.0f;

    return (Color){
        (unsigned char)(start.r + (end.r - start.r) * factor),
        (unsigned char)(start.g + (end.g - start.g) * factor),
        (unsigned char)(start.b + (end.b - start.b) * factor),
        255};
}

void Draw_list_of_deliveries(Delivery_Location PICKUP[NUM_OF_ITEMS_ON_LIST], Delivery_Location DROPOFF[NUM_OF_ITEMS_ON_LIST]) // Draws the list of possible deliveries
{
    // First draw rectangle lines and draw as many rectangles as the mission.
    for (int i = 0; i < NUM_OF_ITEMS_ON_LIST; i++)
    {
        if (PICKUP[i].is_pre_selected == true && DROPOFF[i].is_pre_selected == true)
            DrawRectangleLines(30, (61 * (i + 1) + (WINDOW_HEIGHT / 1.5f) - 1), 151, 61, ORANGE);
        DrawRectangle(30, i + (60 * (i + 1) + (WINDOW_HEIGHT / 1.5f)), 150, 60, DARKBLUE);
        char text_info[50] = {0};
        sprintf(text_info, "Rewards: %d", score_for_current_mission[i]);
        int length = MeasureText(text_info, 20);
        int height = (int)MeasureTextEx(GetFontDefault(), text_info, 20, 2).y;
        DrawText(text_info, 30 + ((150 - length)) / 2, (i + (60 * (i + 1)) + (WINDOW_HEIGHT / 1.5f) + ((60 - height) / 2)), 20, WHITE);
    }
    char text_info[50] = {0};
    sprintf(text_info, "Choose Mission");
    int length = MeasureText(text_info, 20);
    int height = (int)MeasureTextEx(GetFontDefault(), text_info, 20, 2).y;
    DrawText(text_info, 30 + ((150 - length)) / 2, ((WINDOW_HEIGHT / 1.5f) + ((60 - height) / 2)), 20, WHITE);
}
void draw_nitro_bar(int t) // Same things as in draw fuel
{
    DrawRectangleLines(1870, (float)WINDOW_HEIGHT * 2.0f / 3.0f, 20, 100, WHITE);
    Color tankcol = YELLOW;
    if (t > 300)
        tankcol = GREEN;
    else
        tankcol = RED;
    DrawRectangle(1870, ((float)WINDOW_HEIGHT * 2.0f / 3.0f + 100 - (t / (float)(15 * 60)) * 100), 19, ((t / (float)(15 * 60)) * 100), tankcol);
    char gasText[10] = {0};
    sprintf(gasText, "NITRO");
    int length = MeasureText(gasText, 20);
    DrawText(gasText, 1880 - (length / 2), (float)WINDOW_HEIGHT * 2.0f / 3.0f - 30, 20, GREEN);
}
