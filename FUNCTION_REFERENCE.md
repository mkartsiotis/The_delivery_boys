# Function Reference

Full listing of every function implemented in **The Delivery Boys**, organized by translation unit (`.c` file), as declared in `headers.h`. This is the detailed companion to the main [README](../README.md).

---

## `layout.c`

```c
void Initialize_Map(Rectangle (*map)[NUM_OF_RECTANGLES_Y][NUM_OF_RECTANGLES_X]);
```
Initializes the map of square blocks that constitute the roads. Parameter: the map array to initialize.

```c
void set_game_parameters(ScreenStatus *GameScreen, NPC *npc);
```
Sets all parameters needed before a game level starts. Parameters: the current screen state and the NPC (police chaser) struct.

---

## `draw.c`

```c
void draw_pickup_and_dropoff(Vector2 PICKUP, Vector2 DROPOFF);
```
Draws small circles at the dropoff and pickup locations (2D). Parameters: the pickup and dropoff points.

```c
void draw_pickup_and_dropoff3D(Vector2 PICKUP, Vector2 DROPOFF);
```
Draws cones at the dropoff and pickup locations (3D). Parameters: the pickup and dropoff points.

```c
void DrawRectangles(Rectangle map[NUM_OF_RECTANGLES_Y][NUM_OF_RECTANGLES_X]);
```
Draws the rectangle array initialized as `map` in `main`, to render the road layout. Parameter: the map array of building blocks/walls.

```c
void Draw_and_update_score_window(int sucessful_deliveries, int HIGH, ScreenStatus GameScreen);
```
Draws the score window. Parameters: current score, high score, and current screen state.

```c
void draw_astar_results(best_possible_path A_STAR_RESULT);
```
Draws the A\* pathfinding result in 2D. Parameter: the result struct from `aStarSearch`.

```c
void draw_astar_results3D(best_possible_path A_STAR_RESULT);
```
Draws the A\* pathfinding result in 3D. Parameter: the result struct from `aStarSearch`.

```c
void draw_npc(NPC chaser);
```
Draws an NPC (the police car), 2D. Parameter: the NPC struct.

```c
void drawspeed(void);
```
Draws the speedometer.

```c
void draw_fuel_bar(void);
```
Draws the remaining fuel level in the tank.

```c
void draw_grid(void);
```
Draws the outer edges of the map grid in world-map coordinates. Note: this draws the outside sides of the rectangles representing the plane's 2D division, not the internal grid-cell lines used by A\*.

```c
void DrawCubes(Rectangle map[NUM_OF_RECTANGLES_Y][NUM_OF_RECTANGLES_X], Vector2 playerPos);
```
Draws the 3D cubes representing the road/building layout. Parameters: the rectangle array and the player's position.

```c
void draw_npc3D(NPC chaser);
```
Draws the NPC (police car) in 3D. Parameter: the NPC struct with all police-car info.

```c
void draw_cars(Vector3 camera3d);
```
Draws all NPC traffic cars. Parameter: the camera/player position, used to draw selectively and efficiently.

```c
void draw_mission_score(int selected_mission_index);
```
Draws the score that will be awarded if no further points are deducted. Parameter: index of the selected mission.

```c
void Draw_list_of_deliveries(Delivery_Location PICKUP[NUM_OF_ITEMS_ON_LIST], Delivery_Location DROPOFF[NUM_OF_ITEMS_ON_LIST]);
```
Draws the list of available deliveries. Parameters: the pickup and dropoff location arrays.

```c
void draw_nitro_bar(int t);
```
Draws the Nitro bar. Parameter: `t`, a counter used to calculate the remaining nitro.

```c
Color choseRandomColour(void);
```
Returns a random color via a `rand()`-based decoding method.

```c
Color LerpColor(Color start, Color end, float factor);
```
Fades/interpolates between two colors. Parameters: start color, end color, and interpolation factor.

---

## `player_movement.c`

```c
bool check_for_collisions(Rectangle Player, Rectangle map[NUM_OF_RECTANGLES_Y][NUM_OF_RECTANGLES_X]);
```
Checks for collisions between the player and the grid/building objects. Returns `false` if no collision, `true` if collided. Parameters: player's hitbox and the map array.

```c
void keep_in_boundaries(Vector2 *pos);
```
Checks and clamps `pos.x`/`pos.y` if the player has gone outside the window bounds. Parameter: pointer to the player's position.

```c
float delta_move(void);
```
Calculates the required movement delta based on user input. Returns the movement along one axis (called separately for X and Y).

---

## `gamehandling.c`

```c
Delivery_Location initialize_pickup_location(Rectangle map[NUM_OF_RECTANGLES_Y][NUM_OF_RECTANGLES_X]);
```
Sets the pickup location for a mission. Returns a `Delivery_Location` struct. Parameter: the map array of building blocks.

```c
Delivery_Location initialize_dropoff_location(Rectangle map[NUM_OF_RECTANGLES_Y][NUM_OF_RECTANGLES_X], Vector2 PICKUP);
```
Sets the dropoff location for a mission. Returns a `Delivery_Location` struct. Parameters: the map array and the pickup location.

```c
void check_and_apply_mission(Delivery_Location *PICKUP, Delivery_Location *DROPOFF);
```
Sets all parameters for the currently active mission. Parameters: pointers to the pickup and dropoff arrays.

```c
void init_PICKUP_and_DROPOFF(Delivery_Location *PICKUP, Delivery_Location *DROPOFF);
```
Initializes the pickup and dropoff location arrays. Parameters: pointers to the pickup and dropoff arrays.

```c
void burn_fuel(void);
```
Decreases the amount of fuel in the tank (called once per frame/tick).

```c
void set_score_for_current_mission(Vector2 pos, Vector2 PICKUP, Vector2 DROPOFF, int i);
```
Calculates the score for a given mission (Manhattan distance-based) and assigns it to the global `score_for_current_mission` array. Parameters: player position, pickup and dropoff locations, and mission index.

```c
void deduce_score_for_mission(int n, int selected_mission_index);
```
Deducts score from the current mission — called for time-based deduction or as a penalty for collisions/illegal driving. Parameters: amount to deduct, and mission index.

```c
Gas_Station refuel_station(void);
```
Places a gas station at a random position once fuel is low enough. Returns a `Gas_Station` struct.

```c
void print_refuel_station(Gas_Station SET_STATION);
```
Draws the gas station if it's currently visible. Parameter: the `Gas_Station` struct.

```c
void check_for_refuel(Gas_Station *STATION, Vector2 pos);
```
Checks whether the player is near a gas station and refuels the tank if so. Parameters: pointer to the gas station struct, and the player's position.

---

## `astar_search.c`

```c
void initGrid(void);
```
Initializes the grid used for A\* pathfinding.

```c
void CreateWalls(void);
```
Marks wall/obstacle cells on the grid.

```c
double calculateH(int x, int y, int destX, int destY);
```
Computes the heuristic component of A\* (`h(n)`, combined with `g(n)` to give `f(n)`). Returns the heuristic value. Parameters: current coordinates and destination coordinates.

```c
bool isValid(int x, int y);
```
Validates whether a given grid coordinate is in-bounds/usable. Returns `false` if invalid, `true` if valid.

```c
best_possible_path aStarSearch(int startX, int startY, int destX, int destY);
```
Runs the A\* search and returns the resulting path. Return type: `best_possible_path` struct. Parameters: start and destination grid coordinates.

```c
grid_coordinates RealToGrid(Vector2 pos);
```
Converts real (world-map) coordinates to grid coordinates for use in `aStarSearch`. Note: `pos` is treated as a center position, not top-left.

```c
Vector2 GridToReal(int gridX, int gridY);
```
The inverse of `RealToGrid` — converts grid coordinates back to real map coordinates.

---

## `npc.c`

```c
void updateNPC(NPC *chaser, Vector2 player_pos, Rectangle map[NUM_OF_RECTANGLES_Y][NUM_OF_RECTANGLES_X]);
```
Checks conditions and recalculates the police chaser's path if needed. Parameters: pointer to the NPC, player position, and the map array.

```c
int check_if_caught(Vector2 playerpos, NPC npc);
```
Checks whether the player and the police chaser have made contact. Parameters: player position and NPC struct.

```c
void update_npc_cars(void);
```
Controls ambient traffic NPC cars — appearance, disappearance, position, color, and start/end position.

```c
void init_cars(void);
```
Initializes the array of ambient traffic cars.

```c
int check_for_car_crashes(Rectangle Player);
```
Checks for collisions between the player and NPC traffic cars. Parameter: player's hitbox. Returns `0` if not hit, `1` if hit.

```c
void adjust_speedx(int i, int j);
```
Matches the speed of a following car (index `j`) to the slower car ahead of it (index `i`) on the X-axis road.

```c
void adjust_speedy(int i, int j);
```
Matches the speed of a following car (index `j`) to the slower car ahead of it (index `i`) on the Y-axis road.

```c
void adress_traffic(void);
```
Manages the global vertical/horizontal "go" signal for traffic at intersections.

---

## `cam.c`

```c
void TurnCam(Camera3D *camera3d, Vector2 pos);
```
Rotates/updates the 3D camera. Parameters: pointer to the camera, and the player's position.

---

## `audio.c`

```c
void InitGameAudio(void);
```
Initializes the audio system.

```c
void UpdateGameAudio(void);
```
Updates audio playback state; called once per frame.

```c
void StopGameAudio(void);
```
Stops music playback.

```c
void CloseGameAudio(void);
```
Unloads audio resources.

```c
void SwitchTrack(int track);
```
Switches between music tracks. Parameter: `1` to switch from background music to drift/chase music, `0` for the reverse.
