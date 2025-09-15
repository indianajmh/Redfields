// Imports
#include <cstdio>
#include <string>
#include <iostream>
#include "raylib.h"
#include "raymath.h"
#include "BaseCharacter.h"
using namespace std;

// Character Constructor
BaseCharacter::BaseCharacter()
{

}

// Redo X Movement for Collision
void BaseCharacter::redoMovementX()
{
    worldPos.x = worldXNewFrame;    // Set current x position to next frame's x position
}

// Undo X Movement for Collision
void BaseCharacter::undoMovementX()
{
    worldPos.x = worldXLastFrame;   // Set current x position to previous frame's x position
}

// Undo Y Movement for Collision
void BaseCharacter::undoMovementY()
{
    worldPos.y = worldYLastFrame;   // Set current y position to previous frame's y position
}

// Collision Rectangle for Character
Rectangle BaseCharacter::getCollisionRec()
{
    return Rectangle
    {
        getScreenPos().x,   // X
        getScreenPos().y,   // Y
        width * scale,      // Width
        height * scale      // Height
    };
}

// Ticks as in passing of time (like minecraft) 
void BaseCharacter::tick(float deltaTime)
{
    // Max Health Limit
    if (health > maxHealth) {health = maxHealth; }

    // Saving Last Locational Frame
    worldXLastFrame = worldPos.x;
    worldYLastFrame = worldPos.y;

    // Updating Animation Frame
    runningTime += deltaTime;   // Updating animation tick
    if (runningTime >= updateTime)  // Switching animation frame
    {
        frame++;    // Animation frame
        runningTime = 0.f;  // Animation tick
        if (frame > maxFrames)  // Replay animation chain
            frame = 0;
    }

    // Applying Direction
    if (Vector2Length(velocity) != 0.0 || knockback > 0)    // If motion should occur
    {
        // Saving last velocity vector
        lastVelocity = velocity;

        // set worldPos = worldPos + direction - knockback
        worldPos = Vector2Subtract(Vector2Subtract(worldPos, Vector2Scale(Vector2Normalize(lastVelocity), speed)), 
                Vector2Scale(Vector2Normalize(knockVector), knockback));
        if (velocity.x < 0.f)   // if negative x velocity
            rightLeft = 1.f;    // facing right
        if (velocity.x > 0.f)   // if positive x velocity
            rightLeft = -1.f;   // facing left
        texture = run;
    }
    else
    {
        texture = idle;     // Not moving, idle texture
    }
    velocity = {};  // Set velocity to zero

    // Knockback Effect
    if (knockback > 0)  // If knockback present
    {
        knockback -= 0.5 * speed;   // Knockback recovery
        if (knockback < 0) knockback = 0;   // Cap min at zero
    }

    // Drawing Character
    Rectangle source{frame * width, 0.f, rightLeft * width, height};    // Character source texture
    Rectangle dest{getScreenPos().x, getScreenPos().y, scale * width, scale * height};  // Character destination rectangle
    DrawTexturePro(texture, source, dest, Vector2{}, 0.f, tint);    // Draw character

}

// Damage
void BaseCharacter::giveDamage(float damage)
{
    
}

// Receiving Damage
void BaseCharacter::takeDamage(float damage)
{
    health -= damage;   // Applying damage
    if (health <= 0.f && alive) // If alive and should die
    {
        die();  // Die
    }
}