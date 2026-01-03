Στόχοι:
-Δημιουργία 3 επιπέδων. ()
-Εισαγή 3d model για κτήρια και για το σκούτερ και npc. 
-Backround στις τέσσερις πλευρές.
-Διαγράμμιση ή και πεζοδόμια.
-High score logs. Το score να γίνει coins.
-Ο τερματισμος να γινεται με βενζίνη όχι με χρόνο. Μ ε αντάλλαγμα score.
-Η βαθμολογία να γίνεται με αρνητική βαθμολογία. Να μου δίνουν βαθμούς και να μειώνονται όσο αργούμε. 
-Να επηρεάζεται κα ο αστυνόμος από συγκρούσεις με αυτοκίνητα
->Χρονόδιάγραμμα:









This is the progress file #2!  
Today I tried to use differt techniques of moving a player in a world map created by me.  
Steps:  
1. Created a simple coordinate mooving system(if is in boundaries move)
2. Created a different always centered player system but by moving the whole world
3. Wanted to create a new intermediate system in which both the player and the system could move
4. And all of that with avoiding using the raylib camera function!It was very painfull...
5. This backup signals the end of the first step of the process. This is because we reached critical limitations.
    A. Creation of outer bounds failed due to complexity
    B. Too many arguments and variables without any need
    C. Lack of a good coordinate system 
    D. Not great file and function management
    E. Even the window was not full screen
Considering all of the limitations it is obvious that we need to start all over again.
So what are the next steps?
    1.Recreation of the same game but by using the raylib camera function(this step will likely be moved to a later time since it is complex and not mathematiclly urgent)
    2.VERY IMPORTANT: Creating an algorithm with the task of finding the best possible roote between any given coordinates in the world map(see micromouse competitions)
    3.Thinking and executing the main logic of the game in a small map(pickup-delivery-award-time schedule-obstacles-chasing)
    4. Then full scaling
    5. 3D vectoring and chasing
    6.Celebrate
This is the current plan. Will see you on the next backup!
Hello Again!
This is update #4:
And we have something functional!
What we did:
1. Created a simple version with no camera movement to test the logic and the proportions of the game
2. Made a simple order pickup and dropoff system
3. Increased stability of system by implementing control before movement execution
4. Saw many youtube videos considering pseudo3d and this will hopefullly be the final touch of the game

So we have a plan and as always we set new objectives!
1. Create a new branch on git and test there all the new algorithms that will be used to test the find the best roote scenario
2. Find such algorithms on github from micromouse competitions etc
3. Add a camera system
4. Test the pseudo 3d theory and learn how it works
5. Implemengt what you learned to further improove the game
This is the plan for the final result. Untill the next one see you!Have a nice day!

This is update #5:
What we did:
1. Implemented Time logic. Y
2. Think of a rewarding system to discuss it later. ~
3. Test differnt algoriths.
4. Based on A.I. research A* algorithm should do the job now I am trying to find ways to handle it and maybe I will test it and check some online examples. Once done we should be good and then easily draw the shortest path to the target.

Next Update!Update #6:
Today I studied the A* algorithm and quickly realized that we need to change the way we print the whole map to ensure that we can utilize and find the best possible roote.
Soo:
1.Created a function that splits the map into a grid.
2.Modified the array functions so that the map is initialized via this
3.Verified that the game still works
4.Implemented and drew the best possible roote.
THE END.-
Update #7:
Today I finally made the A* work!
Next up:
1.Add NPC'S
2.Add 2d CAMERA
3. MAKE THE GAME 3d
Play!!!!!
Seee yall
Hello again. Its me...Devastated
One day of A* work for nothing.
It is literaly trash in the NPC logic
Now I will implement standard zombie logic.
F A*. F complexity!
SAVE THIS ON THE ARCHIVE.
IT IS THE MOST TERRIBLE MOST COMPLEX CODE I HAVE WRITTEN AND IT IS TRASH>
SEEEE YALL(SORRY FOR THE RAIGEBAIT).
Updates of the last few days:
1. Buily the 3d game
2. Twuicked handling and logic
3. Added a speedometer
4. Created 3d effects.
Next goals:
1. Put more npc's that act like walls.
2. Add the blue sky around
3. Add a better grading system and explain its logic
4. According to Mr. Katsimanis suggestion decrease the maximum speed.
5. Check if adding trafic lights is a good approach or not.
So what we did is that we just added some cars that can move around(check out backup)
The problem is that this move is a bit robotic and generally not nice for a quality game.
We asked the A.I. and got these suggestions.
Via google Gemini:
"That system is the standard "industry trick" for background traffic in open-world games because it is computationally cheap.

To make it feel professional without overcomplicating your code, you need to add "Emergent Behavior." This means simple rules that result in complex-looking situations (like traffic jams or distinct flows).

Here are 4 logical upgrades you can implement, ordered from easiest to most impactful:

1. The "Alternating Flow" Logic (Easiest)
Currently, all your cars likely move from Left -> Right and Top -> Bottom. This looks unnatural.

The Logic:

Use the Road Index (the i in your loop).

If the Road Index is Even (0, 2, 4...), the cars move Left to Right.

If the Road Index is Odd (1, 3, 5...), the cars move Right to Left.

The Effect: This instantly creates the illusion of two-way traffic or a busy city grid without adding any new collision logic. You just flip the speed to negative and swap the start/end points.

2. The "Brake Light" Logic (Anti-Stacking)
Right now, if a fast car spawns behind a slow car, it will drive through it. Real cars brake.

The Logic:

Every frame, before a car moves, it checks the distance to the car directly ahead of it on the same road.

If Distance < Safe_Zone:

The car's speed becomes 0 (or matches the speed of the car ahead).

If Distance > Safe_Zone:

The car accelerates back to its normal speed.

The Effect: You will see natural traffic jams form. If one car stops (perhaps due to the traffic lights below), the cars behind it will queue up nicely.

3. The "Global Traffic Light" System
You don't need a complex object for every intersection. You can cheat using a global timer.

The Logic:

Create a global variable: TrafficState. It cycles between HORIZONTAL_GO and VERTICAL_GO every few seconds.

Define "Intersection Zones" (ranges of X or Y coordinates where roads cross).

For Horizontal Cars:

If they are approaching an intersection AND TrafficState is VERTICAL_GO (Red Light for them):

They treat the intersection line as a "solid wall" and stop just before it.

Once the state changes to HORIZONTAL_GO, the "wall" disappears, and they accelerate.

The Effect: This creates waves of traffic. You will see empty roads, then a huge cluster of cars moving together, which adds massive realism.

4. Variable "Personalities"
Uniformity looks robotic.

The Logic:

When you Spawn a car, assign it a random Type.

Type A (Sports Car): Small size, High speed, Bright color (Red/Yellow).

Type B (Truck): Long size (2x normal), Slow speed, Dull color (Gray/Blue).

Type C (Sedan): Average size, Average speed.

The Effect: Seeing a fast car get stuck behind a slow truck (using the Brake Light logic from #2) creates a little "story" that the player notices, making the world feel alive.

Summary of the Plan
Alternate Directions: Immediate visual variety.

Variable Speeds/Sizes: Breaks the visual pattern.

Brake Logic: Prevents overlap and allows queues.

Traffic Lights: Controls the flow and creates clumps of traffic."
So first lets try to implement the alternating pattern.



