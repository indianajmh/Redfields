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
    worldPos.x = worldXNewFrame;
}

// Undo X Movement for Collision
void BaseCharacter::undoMovementX()
{
    worldPos.x = worldXLastFrame;
}

// Undo Y Movement for Collision
void BaseCharacter::undoMovementY()
{
    worldPos.y = worldYLastFrame;
}

// Collision Rectangle for Character
Rectangle BaseCharacter::getCollisionRec()
{
    return Rectangle
    {
        getScreenPos().x,
        getScreenPos().y,
        width * scale,
        height * scale
    };
}

// Ticks as in passing of time (like minecraft) 
void BaseCharacter::tick(float deltaTime)
{
    // Max Health
    if (health > maxHealth) {health = maxHealth; }

    // Saving Last Locational Frame
    worldXLastFrame = worldPos.x;
    worldYLastFrame = worldPos.y;

    // Updating Animation Frame
    runningTime += deltaTime;
    if (runningTime >= updateTime)
    {
        frame++;
        runningTime = 0.f;
        if (frame > maxFrames)
            frame = 0;
    }

    // Applying Direction
    if (Vector2Length(velocity) != 0.0 || knockback > 0)
    {
        // Saving last velocity vector
        lastVelocity = velocity;

        // set worldPos = worldPos + direction
        worldPos = Vector2Subtract(Vector2Subtract(worldPos, Vector2Scale(Vector2Normalize(lastVelocity), speed)), Vector2Scale(Vector2Normalize(knockVector), knockback));
        if (velocity.x < 0.f)
            rightLeft = 1.f;
        if (velocity.x > 0.f)
            rightLeft = -1.f;
        texture = run;
    }
    else
    {
        texture = idle;
    }
    velocity = {};

    // Knockback Effect
    if (knockback > 0)
    {
        knockback -= 0.5 * speed;
        if (knockback < 0) knockback = 0;
    }

    // Drawing Character
    Rectangle source{frame * width, 0.f, rightLeft * width, height};
    Rectangle dest{getScreenPos().x, getScreenPos().y, scale * width, scale * height};
    DrawTexturePro(texture, source, dest, Vector2{}, 0.f, tint);

}

// Damage
void BaseCharacter::giveDamage(float damage)
{
    
}

// Receiving Damage
void BaseCharacter::takeDamage(float damage)
{
    health -= damage;
    if (health <= 0.f && alive)
    {
        die();
    }
}