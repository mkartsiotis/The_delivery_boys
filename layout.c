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
Node grid[COLS][ROWS] = {0}; // Initialize the Node we declared in the header files.

void Initialize_Map(Rectangle (*map)[NUM_OF_RECTANGLES_Y][NUM_OF_RECTANGLES_X])
{
    // Here we create a map of blocks in a desired area..
    float stepX = (float)MAP_WIDTH / (NUM_OF_RECTANGLES_X * 4.0f); // Calculate stepX and StepY in the same way as the screen is spit into cells with the grid.
    float stepY = (float)MAP_HEIGHT / (NUM_OF_RECTANGLES_Y * 4.0f);
    for (int i = 0; i < NUM_OF_RECTANGLES_Y; i++) 
    {                                             // REMEMBER IN RAYLIB A POSITION IS TOP+LEFT!!!!!!
        for (int j = 0; j < NUM_OF_RECTANGLES_X; j++)
        {
            float x_value = (1 + 4 * j) * stepX;
            float y_value = (1 + 4 * i) * stepY;

            (*map)[i][j] = (Rectangle){x_value, y_value, SIZE_OF_RECTANGLES_X, SIZE_OF_RECTANGLES_Y};
        }
    }
}
// Next up we will do some very-very important stuff.
// A.Implement the A* algorithm for finding the fastest route around the blocks
// B.Draw the route
// But first we need to do something much more important. And that is drawing the grid
// More about the A* algorithm and how we are going to implement it you can see in the documentation.
// We have succesfully accomplished all of those tasks!Comment remains for historical reasons...
void set_game_parameters(ScreenStatus *GameScreen, NPC *npc) //(in layout.c) Sets all the parameters before a game level starts
{
    switch (GameScreen->CurrentScreen)
    {
    case LEVEL1:
        NUM_OF_NPC_CARS_ON_X_ROAD_ON_CURRENT_LEVEL = 0;
        NUM_OF_NPC_CARS_ON_Y_ROAD_ON_CURRENT_LEVEL = 0;
        speed = 0;
        npc->speed = 0.8;
        break;
    case LEVEL2:
        NUM_OF_NPC_CARS_ON_X_ROAD_ON_CURRENT_LEVEL = 2;
        NUM_OF_NPC_CARS_ON_Y_ROAD_ON_CURRENT_LEVEL = 4;
        npc->speed = 1.3;
        speed = 0;
        break;
    case LEVEL3:
        NUM_OF_NPC_CARS_ON_X_ROAD_ON_CURRENT_LEVEL = 10;
        NUM_OF_NPC_CARS_ON_Y_ROAD_ON_CURRENT_LEVEL = 5;
        npc->speed = 1.5f;
        speed = 0;
        break;
    case LEVEL4:
        NUM_OF_NPC_CARS_ON_X_ROAD_ON_CURRENT_LEVEL = 3;
        NUM_OF_NPC_CARS_ON_Y_ROAD_ON_CURRENT_LEVEL = 3;
        npc->speed = 3.5f;
        speed = 0;
        break;
    default:
        return;
    }
    GameScreen->isfitsttime = false;
}
