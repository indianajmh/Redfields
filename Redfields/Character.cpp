// Imports
#include <cstdio>
#include <string>
#include <iostream>
#include "raylib.h"
#include "raymath.h"
#include "Character.h"
using namespace std;

// Character Constructor
Character::Character(int winWidth, int winHeight) :
        windowWidth(winWidth),
        windowHeight(winHeight)
{
    // Player Texture Dimensions
    width = texture.width / maxFrames;
    height = texture.height;

    // Player Stats
    cooldown = 14.f;
    maxHealth = 10.f;
    health = maxHealth;
    baseDamage = 1.f;
    damage = baseDamage;
    force = 1.f;
    weight = 3.f;
    speed = 5.f;
    critChance = 5.f;
    critMult = 1.5;

    // Equipment
    swordLevel = 0;
    armorLevel = 0;
}

// Screen Position Definition
Vector2 Character::getScreenPos()
{
    return Vector2
    {
        // x and y position on screen based on window dimensions, player texture dimensions, and scale
        static_cast<float>(windowWidth) / 2.0f - scale * (0.5f * width),
        static_cast<float>(windowHeight) / 2.0f - scale * (0.5f * height)
    };
}

// Sword Types
void Character::applySword()
{
    if (swordLevel == 0)    // Rusty Sword
    {
        weapon = weapon0;
        baseSpeedS = 0.f;
        baseWeightS = 0.f;
        cleanDamageS = 1.f;
        baseForceS = 1.f;
        baseMaxHealthS = 0.f;
        baseCooldownS = 14.f;
        baseCritMultS = 1.5;
        baseCritChanceS = 5.f;
    }
    if (swordLevel == 1)    // Iron Sword
    {
        weapon = weapon1;
        baseSpeedS = 0.f;
        baseWeightS = 0.f;
        cleanDamageS = 1.5;
        baseForceS = 1.05;
        baseMaxHealthS = 0.f;
        baseCooldownS = 13.f;
        baseCritMultS = 1.5;
        baseCritChanceS = 6.f;
    }
    if (swordLevel == 2)    // Steel Sword
    {
        weapon = weapon2;
        baseSpeedS = 0.f;
        baseWeightS = 0.f;
        cleanDamageS = 2.f;
        baseForceS = 1.1;
        baseMaxHealthS = 0.f;
        baseCooldownS = 13.f;
        baseCritMultS = 1.6;
        baseCritChanceS = 6.f;
    }
    if (swordLevel == 3)    // Gilded Sword
    {
        weapon = weapon3;
        baseSpeedS = 0.f;
        baseWeightS = 0.f;
        cleanDamageS = 3.f;
        baseForceS = 1.15;
        baseMaxHealthS = 0.f;
        baseCooldownS = 12.f;
        baseCritMultS = 1.5;
        baseCritChanceS = 7.f;
    }
}

// Armor Types
void Character::applyArmor()
{
    if (armorLevel == 0)    // Basic Armor
    {
        baseSpeedA = 5.f;
        baseWeightA = 3.f;
        cleanDamageA = 0.f;
        baseForceA = 0.f;
        baseMaxHealthA = 10.f;
        baseCooldownA = 0.f;
        baseCritMultA = 0.f;
        baseCritChanceA = 0.f;
    }
}

// Combining Equipment Stats
void Character::applyEquipment()
{
    baseSpeed = baseSpeedS + baseSpeedA;
    baseWeight = baseWeightS + baseWeightA;
    cleanDamage = cleanDamageS + cleanDamageA;
    baseForce = baseForceS + baseForceA;
    baseMaxHealth = baseMaxHealthS + baseMaxHealthA;
    baseCooldown = baseCooldownS + baseCooldownA;
    baseCritMult = baseCritMultS + baseCritMultA;
    baseCritChance = baseCritChanceS + baseCritChanceA;
}

// Applying Level Buffs
void Character::applyLevel()
{
    cooldown = baseCooldown - (level - 1)*0.01;     // Decreases by 0.01 every level
    maxHealth = baseMaxHealth + (level - 1);        // Increases by 1 every level
    baseDamage = cleanDamage + (level - 1)*0.05;    // Increases by 0.05 every level
    force = baseForce + (level - 1)*0.02;           // Increases by 0.02 every level
    weight = baseWeight + (level - 1)*0.2;          // Increases by 0.2 every level
    speed = baseSpeed + (level - 1)*0.05;           // Increases by 0.05 every level
    critChance = baseCritChance + (level - 1);      // Increases by 1% chance every level
    critMult = baseCritMult + (level - 1)*0.05;     // Increases by 0.05x every level
    if (!instaKill) damage = baseDamage;            // Update damage if instakill is not on
}

// Level Up
void Character::levelUp()
{
    if (xp >= xpReq)    // Meets XP requirement to level up
    {
        level++;
        skillPoints++;
        xp -= xpReq;    // Spend XP
        health = maxHealth + 1;                             // Refill health after level up
    }
}

void Character::displayHP()
{
    // Player XP GUI
    DrawRectangle(29.f, 61.f, xp / xpReq * 20.f * maxHealth + 2.f * (xp/xp), 15.f, BLACK);  // XP bar background
    DrawRectangle(30.f, 61.f, xp / xpReq * 20.f * maxHealth, 14.f, BLUE);                   // XP bar
    DrawText(std::to_string(level).c_str(), 12.f, 32.f, 30, {0, 0, 255, 255});              // Level number text
    DrawText(std::to_string(gold).c_str(), 50.f, 1.f, 30, GOLD);                            // Gold supply text

    // Player Health GUI
    DrawRectangle(29.f, 29.f, 20.f * maxHealth + 2.f, 32.f, BLACK); // Health bar background
    DrawRectangle(30.f, 30.f, 20.f * maxHealth, 30.f, RED);         // Health bar midground/amount of damage taken
    DrawRectangle(30.f, 30.f, 20.f * health, 30.f, GREEN);          // Health bar
}

// Ticks as in passing of time (like minecraft) 
void Character::tick(float deltaTime)
{
    // Is Alive
    if (!getAlive()) return;

    // Calling Base Character Functions
    BaseCharacter::tick(deltaTime);
    
    // Moving Map
    if (IsKeyDown(KEY_A)) velocity.x += 1.0;    // Left
    if (IsKeyDown(KEY_D)) velocity.x -= 1.0;    // Right
    if (IsKeyDown(KEY_W)) velocity.y += 1.0;    // Up
    if (IsKeyDown(KEY_S)) velocity.y -= 1.0;    // Down

    // Applying Stats
    applySword();
    applyArmor();
    applyEquipment();

    // Level System
    xpReq = round(20.f * pow(1.6, level - 1));  // Setting XP requirement
    levelUp();
    applyLevel();

    // Sword Drawing
    Vector2 origin{};   // Origin of sword texture
    Vector2 offset{};   // Offset of sword from player
    float rotation{};   // Rotation of sword texture
    if (rightLeft > 0.f)    // Facing Right
    {
        origin = {0.f, weapon.height * scale};
        offset = {38.f, 55.f};
        // Collision Rec
        weaponCollisionRec = 
        {
            getScreenPos().x + offset.x,
            getScreenPos().y + offset.y - weapon.height * scale,
            weapon.width * scale,
            weapon.height * scale
        };
        // Slash Attack
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && lastHit > cooldown && swing <= 0)    // Cooldown delay and not swinging sword
        {
            lastMove = "Swing"; // Setting attack to continue for multiple ticks
            rotation = 40.f;
            swinging = true;
            swing = cooldown/2; // Swing duration
        }
        else if (swing > 0 && lastMove == "Swing") // Duration of swing (loop)
        {
            rotation = 40.f;
            swinging = true;
            swing--;    // Counting down swing time
        }
        // Stab Attack
        else if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON) && lastHit > cooldown * 1.5 && swing <= 0)    // Cooldown delay and not swinging sword
        {
            lastMove = "Stab";  // Setting attack to continue for multiple ticks
            rotation = 45.f;
            offset = {45.f, 36.f};  // Stab attack has greater offset from player
            swinging = true;
            swing = cooldown;   // Swing duration
        }
        else if (swing >= cooldown * 0.7 && lastMove == "Stab") // Duration of swing (loop)
        {
            rotation = 45.f;
            offset = {45.f, 36.f};
            swinging = true;
            swing--;    // Counting down swing time
        }
        else if (swing < cooldown * 0.7 && swing > 0 && lastMove == "Stab") // Swing duration after 30% cooldown time
        {
            rotation = 45.f;
            offset = {45.f, 36.f};
            swinging = false;   // Swing "ends" early so damage is not applied when not actively stabbing
            swing--;
        }
        else    // Sword back at rest
        {
            rotation = 0.f;
            offset = {38.f, 55.f};;
            swinging = false;
        }
    }
    else    // Facing Left
    {
        origin = {weapon.width * scale, weapon.height * scale};
        offset = {25.f, 55.f};
        // Collision Rec
        weaponCollisionRec = 
        {
            getScreenPos().x + offset.x - weapon.width * scale,
            getScreenPos().y + offset.y - weapon.height * scale,
            weapon.width * scale,
            weapon.height * scale
        };
        // Slash Attack
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && lastHit > cooldown && swing <= 0)    // Cooldown delay and not swinging sword
        {
            lastMove = "Swing"; // Setting attack to continue for multiple ticks
            rotation = -40.f;
            swinging = true;
            swing = cooldown/2; // Swing duration
        }
        else if (swing > 0 && lastMove == "Swing") // Duration of swing (loop)
        {
            rotation = -40.f;
            swinging = true;
            swing--;    // Counting down swing time
        }
        // Stab Attack
        else if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON) && lastHit > cooldown * 1.5 && swing <= 0)    // Cooldown delay and not swinging sword
        {
            lastMove = "Stab";  // Setting attack to continue for multiple ticks
            rotation = -45.f;
            offset = {18.f, 36.f};  // Stab attack has greater offset from player
            swinging = true;
            swing = cooldown;   // Swing duration
        }
        else if (swing >= cooldown * 0.7 && lastMove == "Stab") // Duration of swing (loop)
        {
            rotation = -45.f;
            offset = {18.f, 36.f};
            swinging = true;
            swing--;    // Counting down swing time
        }
        else if (swing < cooldown * 0.7 && swing > 0 && lastMove == "Stab") // Swing duration after 30% cooldown time
        {
            rotation = -45.f;
            offset = {18.f, 36.f};
            swinging = false;   // Swing "ends" early so damage is not applied when not actively stabbing
            swing--;
        }
        else    // Sword back at rest
        {
            rotation = 0.f;
            offset = {25.f, 55.f};;
            swinging = false;
        }
    }

    // Drawing Sword
    Rectangle source{0.f, 0.f, static_cast<float>(weapon.width) * rightLeft, static_cast<float>(weapon.height)};    // Sword source texture
    Rectangle dest{getScreenPos().x + offset.x, getScreenPos().y + offset.y, weapon.width * scale, weapon.height * scale};  // Sword destination rectangle
    DrawTexturePro(weapon, source, dest, origin, rotation, WHITE);  // Draw sword

    // Sword Collision Box
    /*
    DrawRectangleLines
    (
        weaponCollisionRec.x,
        weaponCollisionRec.y,
        weaponCollisionRec.width,
        weaponCollisionRec.height,
        RED
    );  */

    // Saving New Locational Frame
    worldXNewFrame = worldPos.x;
}