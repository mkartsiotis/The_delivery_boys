/*
 * Όνομα Παιχνιδιού: Ο Διανομέας (The Delivery Man)
 * Συγγραφείς: Καρτσιώτης Μιχαήλ
 *             Κατσιμάνης Δημήτριος 
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

enum Sides
{
    TOP1,
    TOP2,
    BOTTOM1,
    BOTTOM2,
    LEFT1,
    LEFT2,
    RIGHT1,
    RIGHT2
};

int score_for_current_mission[NUM_OF_ITEMS_ON_LIST] = {0}; // Variable that is responsible for storing the score for a mission.
float gas = (float)INITIAL_GASOLINE;

Delivery_Location initialize_pickup_location(Rectangle map[NUM_OF_RECTANGLES_Y][NUM_OF_RECTANGLES_X])
{
    Delivery_Location coordinates;
    enum Sides PICKUP_SIDE;
    int block_x, block_y;
    block_x = rand() % NUM_OF_RECTANGLES_X;
    block_y = rand() % NUM_OF_RECTANGLES_Y;
    PICKUP_SIDE = rand() % 8;
    (coordinates.REAL).x = map[block_y][block_x].x;
    (coordinates.REAL).y = map[block_y][block_x].y;
    if (PICKUP_SIDE == TOP2)
    {
        (coordinates.REAL).x += 2 * SIZE_OF_RECTANGLES_X / 3.0f;
        coordinates.grid_x = block_x * 4 + 2; // We know we owe you an explanation...But in short this is the conversion algorithm.(due to limited time and resources the explanation will not be on the docs. Contact us if you wish to know more!)
        coordinates.grid_y = block_y * 4;
    }
    if (PICKUP_SIDE == TOP1)
    {
        (coordinates.REAL).x += SIZE_OF_RECTANGLES_X / 3.0f;
        coordinates.grid_x = block_x * 4 + 1; // Similar to before(ask us).
        coordinates.grid_y = block_y * 4;
    }
    if (PICKUP_SIDE == BOTTOM1)
    {
        (coordinates.REAL).x += SIZE_OF_RECTANGLES_X / 3.0f;
        (coordinates.REAL).y += SIZE_OF_RECTANGLES_Y;
        coordinates.grid_x = block_x * 4 + 1; // Similar to before.
        coordinates.grid_y = block_y * 4 + 3;
    }
    if (PICKUP_SIDE == BOTTOM2)
    {
        (coordinates.REAL).x += 2 * SIZE_OF_RECTANGLES_X / 3.0f;
        (coordinates.REAL).y += SIZE_OF_RECTANGLES_Y;
        coordinates.grid_x = block_x * 4 + 2; // Similar to before.
        coordinates.grid_y = block_y * 4 + 3;
    }
    if (PICKUP_SIDE == LEFT1)
    {
        (coordinates.REAL).y += SIZE_OF_RECTANGLES_Y / 3.0f;
        coordinates.grid_x = block_x * 4; // Similar to before.
        coordinates.grid_y = block_y * 4 + 1;
    }
    if (PICKUP_SIDE == LEFT2)
    {
        (coordinates.REAL).y += 2 * SIZE_OF_RECTANGLES_Y / 3.0f;
        coordinates.grid_x = block_x * 4; // Similar to before.
        coordinates.grid_y = block_y * 4 + 2;
    }
    if (PICKUP_SIDE == RIGHT1)
    {
        (coordinates.REAL).x += SIZE_OF_RECTANGLES_X;
        (coordinates.REAL).y += SIZE_OF_RECTANGLES_Y / 3.0f;
        coordinates.grid_x = block_x * 4 + 3; // Similar to before.
        coordinates.grid_y = block_y * 4 + 1;
    }
    if (PICKUP_SIDE == RIGHT2)
    {
        (coordinates.REAL).x += SIZE_OF_RECTANGLES_X;
        (coordinates.REAL).y += 2 * SIZE_OF_RECTANGLES_Y / 3.0f;
        coordinates.grid_x = block_x * 4 + 3; // Similar to before.
        coordinates.grid_y = block_y * 4 + 2;
    }
    coordinates.is_set = true;
    return coordinates;
}
Delivery_Location initialize_dropoff_location(Rectangle map[NUM_OF_RECTANGLES_Y][NUM_OF_RECTANGLES_X], Vector2 PICKUP)
{
    Delivery_Location coordinates;
    enum Sides DROPOFF_SIDE;
    int block_x, block_y;
    block_x = rand() % NUM_OF_RECTANGLES_X;
    block_y = rand() % NUM_OF_RECTANGLES_Y;

    DROPOFF_SIDE = rand() % 4;
    (coordinates.REAL).x = map[block_y][block_x].x;
    (coordinates.REAL).y = map[block_y][block_x].y;
    if (DROPOFF_SIDE == TOP2)
    {
        (coordinates.REAL).x += 2 * SIZE_OF_RECTANGLES_X / 3.0f;
        coordinates.grid_x = block_x * 4 + 2; // We know we owe you an explanation...But in short this is the conversion algorithm.(due to limited time and resources the explanation will not be on the docs. Contact us if you wish to know more!)
        coordinates.grid_y = block_y * 4;
    }
    if (DROPOFF_SIDE == TOP1)
    {
        (coordinates.REAL).x += SIZE_OF_RECTANGLES_X / 3.0f;
        coordinates.grid_x = block_x * 4 + 1; // Similar to before.
        coordinates.grid_y = block_y * 4;
    }
    if (DROPOFF_SIDE == BOTTOM1)
    {
        (coordinates.REAL).x += SIZE_OF_RECTANGLES_X / 3.0f;
        (coordinates.REAL).y += SIZE_OF_RECTANGLES_Y;
        coordinates.grid_x = block_x * 4 + 1; // Similar to before.
        coordinates.grid_y = block_y * 4 + 3;
    }
    if (DROPOFF_SIDE == BOTTOM2)
    {
        (coordinates.REAL).x += 2 * SIZE_OF_RECTANGLES_X / 3.0f;
        (coordinates.REAL).y += SIZE_OF_RECTANGLES_Y;
        coordinates.grid_x = block_x * 4 + 2; // Similar to before.
        coordinates.grid_y = block_y * 4 + 3;
    }
    if (DROPOFF_SIDE == LEFT1)
    {
        (coordinates.REAL).y += SIZE_OF_RECTANGLES_Y / 3.0f;
        coordinates.grid_x = block_x * 4; // Similar to before.
        coordinates.grid_y = block_y * 4 + 1;
    }
    if (DROPOFF_SIDE == LEFT2)
    {
        (coordinates.REAL).y += 2 * SIZE_OF_RECTANGLES_Y / 3.0f;
        coordinates.grid_x = block_x * 4; // Similar to before.
        coordinates.grid_y = block_y * 4 + 2;
    }
    if (DROPOFF_SIDE == RIGHT1)
    {
        (coordinates.REAL).x += SIZE_OF_RECTANGLES_X;
        (coordinates.REAL).y += SIZE_OF_RECTANGLES_Y / 3.0f;
        coordinates.grid_x = block_x * 4 + 3; // Similar to before.
        coordinates.grid_y = block_y * 4 + 1;
    }
    if (DROPOFF_SIDE == RIGHT2)
    {
        (coordinates.REAL).x += SIZE_OF_RECTANGLES_X;
        (coordinates.REAL).y += 2 * SIZE_OF_RECTANGLES_Y / 3.0f;
        coordinates.grid_x = block_x * 4 + 3; // Similar to before(See docs).
        coordinates.grid_y = block_y * 4 + 2;
    }
    // If we see that dropoff = pickup we just run the function again
    while ((coordinates.REAL).x == PICKUP.x && (coordinates.REAL).y == PICKUP.y)
    {
        initialize_dropoff_location(map, PICKUP);
    }
    coordinates.is_set = true;
    return coordinates;
}

void burn_fuel(void) //(In gamehandling.c)Decreases the fuel amount.
{
    gas -= speed / 50; // This formula calculates the amount of fuel that is burnt in every frame.
}

void set_score_for_current_mission(Vector2 pos, Vector2 PICKUP, Vector2 DROPOFF, int i) // Calculates the score for a given mission and assigns it to the global external variable score_for_current_mission.
{
    float points_award = 0;
    points_award = (fabs(pos.x - PICKUP.x) + fabs(pos.y - PICKUP.y) + fabs(DROPOFF.x - PICKUP.x) + fabs(DROPOFF.y - PICKUP.y)) / 10.0f;
    score_for_current_mission[i] = (int)points_award; // Assign the score to the global variable score_for_current_mission
}
void deduce_score_for_mission(int n, int selected_mission_index) // Deduces score for current mission when called in main(for time deduction and punishment when hitting a car or when doing something illegal)
{
    if (score_for_current_mission[selected_mission_index] > 0)
        score_for_current_mission[selected_mission_index] -= n; // Probably an unnecessary function but ok...
}
// Gas staion logic
Gas_Station refuel_station(void) //(In gamehandling.c)This is the function that is responsible for setting the gas station when the fuel low enough at a random position
{
    Gas_Station return_station = {0};
    if (gas > (2.0f / 7.0f) * (float)INITIAL_GASOLINE)
        return return_station;
    else
    {
        return_station.isvisible = true;
        // Randomly select an available grid position on the map
        // select a random point on the grid,
        int randx = (rand() % (4 * NUM_OF_RECTANGLES_X));
        int randy = (rand() % (4 * NUM_OF_RECTANGLES_Y));
        return_station.grid_x = grid[randx][randy].x;
        return_station.grid_y = grid[randx][randy].y;
        while (grid[randx][randy].isObstacle == true) // If the grid point is an obstacle
        {
            randx = (rand() % (4 * NUM_OF_RECTANGLES_X)); // Try again to find a valid point
            randy = (rand() % (4 * NUM_OF_RECTANGLES_Y));
            return_station.grid_x = grid[randx][randy].x;
            return_station.grid_y = grid[randx][randy].y;
        }
        return_station.REAL = GridToReal(return_station.grid_x, return_station.grid_y);
        return return_station;
    }
}

void check_for_refuel(Gas_Station *STATION, Vector2 pos) //(In gamehandling.c)Checks if the player is near to a gas station and refuels
{
    if (STATION->isvisible == false)
        return;
    else
    {
        // Check if the player is near to the station.
        if (fabs(pos.x - STATION->REAL.x) < MAN_RECTANGLE_WIDTH && fabs(pos.y - STATION->REAL.y) < MAN_RECTANGLE_HEIGHT)
        {
            gas += 170;
            (*STATION) = (Gas_Station){0};
        }
    }
}

/*
List of active deliveries creation.....
Steps...
1. Create a global array of pickups and dropoffs
2. Assign them values at the beginnig
3. Add user controls with the w s and enter keys(up-down-select)
4. If select is pressed select that mission
5. Add a draw function of course as well*/

void check_and_apply_mission(Delivery_Location *PICKUP, Delivery_Location *DROPOFF) // Sets all the parameters for current mission
{
    // Find the is_preset = 1;
    int is_preset_index = -1;
    for (int i = 0; i < NUM_OF_ITEMS_ON_LIST; i++)
        if (PICKUP[i].is_pre_selected == 1)
            is_preset_index = i;
    // If no item is selected set 0 to selected
    if (is_preset_index == -1)
    {
        is_preset_index = 0;
        PICKUP[is_preset_index].is_pre_selected = true;
        DROPOFF[is_preset_index].is_pre_selected = true;
    }
    // Move up if we pressed W
    if (IsKeyPressed(KEY_W) && is_preset_index > 0)
    {
        PICKUP[is_preset_index].is_pre_selected = false;
        DROPOFF[is_preset_index].is_pre_selected = false;
        is_preset_index--;
        PICKUP[is_preset_index].is_pre_selected = true;
        DROPOFF[is_preset_index].is_pre_selected = true;
    }
    // Move down if we pressed S
    if (IsKeyPressed(KEY_S) && is_preset_index < NUM_OF_ITEMS_ON_LIST)
    {
        PICKUP[is_preset_index].is_pre_selected = false;
        DROPOFF[is_preset_index].is_pre_selected = false;
        is_preset_index++;
        PICKUP[is_preset_index].is_pre_selected = true;
        DROPOFF[is_preset_index].is_pre_selected = true;
    }
    if (IsKeyPressed(KEY_ENTER) && is_preset_index < NUM_OF_ITEMS_ON_LIST)
    {
        for (int i = 0; i < NUM_OF_ITEMS_ON_LIST; i++) // Set all the selected to 0 for safety
        {
            PICKUP[i].is_selected = false;
            DROPOFF[i].is_selected = false;
        }
        PICKUP[is_preset_index].is_selected = true;
        DROPOFF[is_preset_index].is_selected = true;
    }
}

void init_PICKUP_and_DROPOFF(Delivery_Location *PICKUP, Delivery_Location *DROPOFF) // Initializes PICKUP and DROPOFF location
{
    for (int i = 0; i < NUM_OF_ITEMS_ON_LIST; i++) // For all the items of the array initialize
    {
        PICKUP[i] = (Delivery_Location){
            {{-100, -100}, -1, -1}, // grid_and_map_coords
            false,                  // is_set
            false,                  // is_selected
            false                   // is_pre_selected
        };

        DROPOFF[i] = (Delivery_Location){
            {{-100, -100}, -1, -1},
            false,
            false,
            false};
    }
}
