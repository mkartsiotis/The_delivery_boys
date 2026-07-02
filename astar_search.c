/*
 * Όνομα Παιχνιδιού: Ο Διανομέας (The Delivery Man)
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
#include "headers.h"

// Initialize the Grid
void initGrid(void)
{
    for (int i = 0; i < COLS; i++)
    {
        for (int j = 0; j < ROWS; j++)
        {
            grid[i][j].x = i;
            grid[i][j].y = j;
            grid[i][j].parentX = -1;
            grid[i][j].parentY = -1;
            grid[i][j].g = INFINITY;
            grid[i][j].h = 0;
            grid[i][j].f = INFINITY;
            grid[i][j].isClosed = false;
            grid[i][j].isOpen = false;
        }
    }
}
void CreateWalls(void)//Sets the walls where they need to be.
{
    for (int i = 0; i < COLS; i++)
    {
        for (int j = 0; j < ROWS; j++)
        {
            if ((i % 4 == 1 || i % 4 == 2) && (j % 4 == 1 || j % 4 == 2))
                grid[i][j].isObstacle = true;
        }
    }
}
// Heuristic Function (Manhattan Distance)
// We are using the MANHATAN heuristic technique since we implement only 4-direction movement (for this version).
double calculateH(int x, int y, int destX, int destY)
{
    // Using the Manhatan algorithm since movement is limited to the x and y axis only
    return abs(x - destX) + abs(y - destY);
}
// Check if a coordinate is valid (inside the map)
bool isValid(int x, int y)
{
    return (x >= 0 && x < COLS && y >= 0 && y < ROWS);
}

// The Main A* Algorithm. It is of type best possible path since we need to encode the result and if it was successful or not.
best_possible_path aStarSearch(int startX, int startY, int destX, int destY) // ERROR HANDLING: -1 is Invalid coordinates. -2 is destination is an obstacle, -3 is fail to find a path.
{
    best_possible_path return_stucture;
    // Check validity
    if (!isValid(startX, startY) || !isValid(destX, destY))
    {
        return_stucture.result = -1;
        return_stucture.number_of_points = 0;
        return return_stucture;
    }

    if (grid[destX][destY].isObstacle)
    {
        return_stucture.result = -2;
        return_stucture.number_of_points = 0;
        return return_stucture;
    }

    // Initialize Start Node
    grid[startX][startY].g = 0.0;
    grid[startX][startY].h = calculateH(startX, startY, destX, destY);
    grid[startX][startY].f = grid[startX][startY].g + grid[startX][startY].h;
    grid[startX][startY].isOpen = true;

    // Loop until we find the destination
    while (true)
    {
        double minF = INFINITY;
        Node *current = NULL;

        for (int i = 0; i < COLS; i++)
        {
            for (int j = 0; j < ROWS; j++)
            {
                if (grid[i][j].isOpen && grid[i][j].f < minF)
                {
                    minF = grid[i][j].f;
                    current = &grid[i][j];
                }
            }
        }

        // If Open List is empty and we haven't found target, path is impossible
        if (current == NULL)
        {
            return_stucture.result = -3;
            return_stucture.number_of_points = 0;
            return return_stucture;
        }

        current->isOpen = false;
        current->isClosed = true;

        // Check if we reached the goal
        if (current->x == destX && current->y == destY)
        {
            Node *pathNode = current;
            int counter = 0; // Counter variable
            while (pathNode->parentX != -1)
            {
                return_stucture.MATRIX_OUT[counter][0] = pathNode->x;
                return_stucture.MATRIX_OUT[counter][1] = pathNode->y;
                pathNode = &grid[pathNode->parentX][pathNode->parentY];
                counter++;
            }
            return_stucture.MATRIX_OUT[counter][0] = startX;
            return_stucture.MATRIX_OUT[counter][1] = startY;
            return_stucture.number_of_points = ++counter; // One more since we start from 0.
            return_stucture.result = 0;
            return return_stucture;
        }
        int dx[] = {-1, 1, 0, 0};
        int dy[] = {0, 0, -1, 1};

        for (int i = 0; i < 4; i++)
        {
            int newX = current->x + dx[i];
            int newY = current->y + dy[i];

            if (isValid(newX, newY))
            {
                // If it's a wall or already closed, skip it
                if (grid[newX][newY].isObstacle || grid[newX][newY].isClosed)
                {
                    continue;
                }

                // Calculate tentative g cost
                double newG = current->g + 1.0;
                double newH = calculateH(newX, newY, destX, destY);
                double newF = newG + newH;

                if (grid[newX][newY].f == INFINITY || newG < grid[newX][newY].g)
                {
                    grid[newX][newY].g = newG;
                    grid[newX][newY].h = newH;
                    grid[newX][newY].f = newF;
                    grid[newX][newY].parentX = current->x;
                    grid[newX][newY].parentY = current->y;
                    grid[newX][newY].isOpen = true;
                }
            }
        }
    }
}

grid_coordinates RealToGrid(Vector2 pos) //(In astar_search.c) converts real map coordinates to grid coordinates for a_star_search.
{
    grid_coordinates gridpos;
    float cellwidth = (float)MAP_WIDTH / (float)COLS;   // Calculate cell width
    float cellheight = (float)MAP_HEIGHT / (float)ROWS; // Calculate cell height
    int gridX = (int)(pos.x / cellwidth);
    int gridY = (int)(pos.y / cellheight); // Find the gridX and gridY in float
    gridpos.gridX = gridX;
    gridpos.gridY = gridY;
    return gridpos;
}

Vector2 GridToReal(int gridX, int gridY) //(In astar_search.c) converts grid coordinates to real map ones.
{
    Vector2 returnvector;
    float cellwidth = (float)MAP_WIDTH / (float)COLS;   // Calculate cell width
    float cellheight = (float)MAP_HEIGHT / (float)ROWS; // Calculate cell height
    returnvector.x = cellwidth * gridX + 0.5f * cellwidth;
    returnvector.y = cellheight * gridY + 0.5f * cellheight;
    return returnvector;
}
