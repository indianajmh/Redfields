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
    target->takeDamage(damage); // Apply damage to player
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
    if (CheckCollisionRecs(target->getCollisionRec(), {getScreenPos().x - range, 
        getScreenPos().y - range, range*2 + width*5, range*2 + height*5}) && !agro)
    {
        agro = true;     // If not agro and in range of player, become agro
    }
    if (agro)   // If agro, chase player
    {
        velocity = Vector2Subtract(getScreenPos(), target->getScreenPos()); // movement vector towards player
        if (Vector2Length(velocity) < reach) velocity = {}; // if out of range, stop moving
    }

    // Saving New Locational Frame
    worldXNewFrame = worldPos.x;

    // Giving Damage to Player
    if (CheckCollisionRecs(target->getCollisionRec(), getCollisionRec()) && getLastHit() >  cooldown)   // In contact with player
    {
        if (getDelay() == 0.f)  // Cooldown over
        {
            // Applying Knockback
            target->setKnockVector(getLastVelocity());  // knockback direction
            target->setKnockback(target->getSpeed() * getForce() / (sqrt(target->getWeight()) / 2.f));  // knockback magnitude

            // Critical Hit
            if (rand() % 100 <= critChance) 
            {
                giveDamage(getDamage() * critMult); // crit damage
                target->setKnockback(target->getKnockback() * (0.67 + getCritMult() / 3.f));    // crit knockback
            }
            else
            {
                // Applying damage
                giveDamage(getDamage());
            }
            setLastHit(0.f);    // last hit was 0 seconds ago
        }
        else
        {
            setDelay(getDelay() - 1);   // countdown delay
        }
        
    }
    else
    {
        setDelay(15.f); // reset delay
    }

    // Enemy Health GUI
    if (tag == "Gobby the Goblin" || tag == "Nickelodeon the Slime")    // If boss, draw boss bar
    {
        DrawText(tag.c_str(), 228.f, 615.f, 30.f, WHITE);   // Boss name
        DrawRectangle(216.f, 648.f, 824.f, 40.f, BLACK);    // Boss bar background
        DrawRectangle(218.f, 650.f, getHealth() * (820.f) / getMaxHealth(), 36.f, {255, 21, 5, 255});   // Boss bar
    }
    else
    {
        float font = 20.f - tag.length();   // Enemy name tag font size based on name length
        DrawText(tag.c_str(), getScreenPos().x - 8.f + font/3.f, getScreenPos().y - 31.f - font/2.5, font, WHITE);  // Enemy name above health bar
        DrawRectangle(getScreenPos().x - 6.f, getScreenPos().y - 21.f, width*scale + 12.f, 14.f, BLACK);    // Enemy health bar background
        DrawRectangle(getScreenPos().x - 5.f, getScreenPos().y - 20.f, getHealth() * (width*scale + 10.f)   
            / getMaxHealth(), 12.f, {255, 21, 5, 255});     // Enemy health bar
    }
}

// Getting Screen Position
Vector2 Enemy::getScreenPos()
{
    return Vector2Subtract(worldPos, target->getWorldPos());    // screen pos based on world pos and player pos
}

// Player XP Gain
void Enemy::die()
{
    float exp = 0;  // initiate xp value

    // XP rewards based on enemy type
    if (getTag() == "Goblin") exp = 3;
    if (getTag() == "Super Goblin") exp = 8;
    if (getTag() == "Goblin Bodyguard") exp = 16;
    if (getTag() == "Godblin") exp = 80;
    if (getTag() == "Slime") exp = 5;
    if (getTag() == "Super Slime") exp = 12;
    if (getTag() == "Nickelodeon the Slime") exp = 25;
    if (getTag() == "Gobby the Goblin") exp = 75;

    target->addXP(exp); // give player XP

    BaseCharacter::die();   // enemy dies
}