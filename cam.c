#include "headers.h"

double angleRad = 0;//Angle in which the player has turned

void TurnCam(Camera3D *camera3d, Vector2 pos) // Turns 3D cam to the left.
{
    //Calculate offset
    Vector3 baseOffset = { 0.0f, 13.0f, 45.0f }; 
    Vector3 rotatedOffset = Vector3RotateByAxisAngle(baseOffset, (Vector3){0.0f, 1.0f, 0.0f}, angleRad);
    
    Vector3 DesiredPosition = Vector3Add((Vector3){pos.x, 0.0f, pos.y}, rotatedOffset);
    Vector3 DesiredTarget = (Vector3){pos.x, 0.0f, pos.y};

    //Smoothe movement with vector lerp
    camera3d->position = Vector3Lerp(camera3d->position, DesiredPosition, 0.1f);
    
    //Smooth the looking direction.
    camera3d->target = Vector3Lerp(camera3d->target, DesiredTarget, 0.1f);
}
/*
Camera linear algebra logic
1. We create a vector V which is at first equal to the vector AC where A is the initial player's position and C is the camrera position. This means that we intitialize the vector as the initial vector of the camera position(no dynamic change)and with the roatation that we will discuss later(see number 2) we set the angle of rotation to match the player'sperspective.
2. We rotate it by the same angle as the player turns.
3. We implement a smoothing teqnichue to minimize player sickness. 
4. We asssign that value to the camera position.
That's it!
*/