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