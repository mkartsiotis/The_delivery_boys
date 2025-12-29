#include "headers.h"

double angleRad = 0;//Angle in which the player has turned

void TurnCam(Camera3D *camera3d, Vector2 pos) // Turns 3D cam to the left.
{
    //Calculate offset
    Vector3 baseOffset = { 0.0f, 600.0f, 400.0f }; 
    Vector3 rotatedOffset = Vector3RotateByAxisAngle(baseOffset, (Vector3){0.0f, 1.0f, 0.0f}, angleRad);
    
    Vector3 DesiredPosition = Vector3Add((Vector3){pos.x, 0.0f, pos.y}, rotatedOffset);
    Vector3 DesiredTarget = (Vector3){pos.x, 0.0f, pos.y};

    //Smoothe movement with vector lerp
    camera3d->position = Vector3Lerp(camera3d->position, DesiredPosition, 0.1f);
    
    //Smooth the looking direction.
    camera3d->target = Vector3Lerp(camera3d->target, DesiredTarget, 0.1f);
}