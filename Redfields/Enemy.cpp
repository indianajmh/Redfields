// Imports
#include <cstdio>
#include <string>
#include <iostream>
#include "raylib.h"
#include "raymath.h"
#include "Enemy.h"
using namespace std;

// Enemy Constructor
Enemy::Enemy(Vector2 pos, Texture2D idleTex, Texture2D runTex)
{
    // Simplifying and Declaring Variables
    worldPos = pos;
    texture = idleTex;
    idle = idleTex;
    run = runTex;
    width = texture.width / maxFrames;
    height = texture.height;
}

// Giving Damage
void Enemy::giveDamage(float damage)
{
    target->takeDamage(damage);
}

// Tick Function
void Enemy::tick(float deltaTime)
{
    // Is Alive
    if (!isSpawned()) return;
    if (!getAlive()) return;
    
    //DrawRectangle(getScreenPos().x - range, getScreenPos().y - range, range*2 + width*5, range*2 + height*5, RED);

    // Calling Base Character Functions
    BaseCharacter::tick(deltaTime);
    
    // Enemy Movement
    if (CheckCollisionRecs(target->getCollisionRec(), {getScreenPos().x - range, getScreenPos().y - range, range*2 + width*5, range*2 + height*5}) && !agro)
    {
        velocity = Vector2Subtract(getScreenPos(), target->getScreenPos());
        if (Vector2Length(velocity) < reach) velocity = {};
        agro = true;
    }
    else if (agro)
    {
        velocity = Vector2Subtract(getScreenPos(), target->getScreenPos());
        if (Vector2Length(velocity) < reach) velocity = {};
    }

    // Saving New Locational Frame
    worldXNewFrame = worldPos.x;

    // Giving Damage to Player
    if (CheckCollisionRecs(target->getCollisionRec(), getCollisionRec()) && getLastHit() >  cooldown)
    {
        if (getDelay() == 0.f)
        {
            (rand() % 100 <= critChance) ? giveDamage(getDamage() * critMult) : giveDamage(getDamage());
            target->setKnockVector(getLastVelocity());
            target->setKnockback(target->getSpeed() * getForce() / (sqrt(target->getWeight()) / 2.f));
            if (rand() % 100 <= critChance)
            {
                giveDamage(getDamage() * critMult);
                target->setKnockback(target->getKnockback() * (0.67 + getCritMult() / 3.f));
            }
            else
            {
                giveDamage(getDamage());
            }
            setLastHit(0.f);
        }
        else
        {
            setDelay(getDelay() - 1);
        }
        
    }
    else
    {
        setDelay(15.f);
    }

    // Enemy Health GUI
    if (tag == "Gobby the Goblin" || tag == "Nickelodeon the Slime")
    {
        DrawText(tag.c_str(), 228.f, 615.f, 30.f, WHITE);
        DrawRectangle(216.f, 648.f, 824.f, 40.f, BLACK);
        DrawRectangle(218.f, 650.f, getHealth() * (820.f) / getMaxHealth(), 36.f, {255, 21, 5, 255});
    }
    else
    {
        float font = 20.f - tag.length();
        DrawText(tag.c_str(), getScreenPos().x - 8.f + font/3.f, getScreenPos().y - 31.f - font/2.5, font, WHITE);
        DrawRectangle(getScreenPos().x - 6.f, getScreenPos().y - 21.f, width*scale + 12.f, 14.f, BLACK);
        DrawRectangle(getScreenPos().x - 5.f, getScreenPos().y - 20.f, getHealth() * (width*scale + 10.f) / getMaxHealth(), 12.f, {255, 21, 5, 255});
    }
}

// Getting Screen Position
Vector2 Enemy::getScreenPos()
{
    return Vector2Subtract(worldPos, target->getWorldPos());
}

// Player XP Gain
void Enemy::die()
{
    if (getTag() == "Goblin")
    {
        target->addXP(3.f);
    }
    if (getTag() == "Super Goblin")
    {
        target->addXP(8.f);
    }
    if (getTag() == "Goblin Bodyguard")
    {
        target->addXP(16.f);
    }
    if (getTag() == "Godblin")
    {
        target->addXP(80.f);
    }
    if (getTag() == "Slime")
    {
        target->addXP(5.f);
    }
    if (getTag() == "Super Slime")
    {
        target->addXP(12.f);
    }
    if (getTag() == "Nickelodeon the Slime")
    {
        target->addXP(25.f);
    }
    if (getTag() == "Gobby the Goblin")
    {
        target->addXP(75.f);
    }
    BaseCharacter::die();
}