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
