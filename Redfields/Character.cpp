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
    // Simplifying Player Variables
    width = texture.width / maxFrames;
    height = texture.height;

    // Stats
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

    swordLevel = 0;
    armorLevel = 0;
}

// Screen Position Definition
Vector2 Character::getScreenPos()
{
    return Vector2
    {
        static_cast<float>(windowWidth) / 2.0f - scale * (0.5f * width),
        static_cast<float>(windowHeight) / 2.0f - scale * (0.5f * height)
    };
}

// Ticks as in passing of time (like minecraft) 
void Character::tick(float deltaTime)
{
    // Is Alive
    if (!getAlive()) return;

    // Calling Base Character Functions
    BaseCharacter::tick(deltaTime);
    
    // Moving Map
    if (IsKeyDown(KEY_A)) velocity.x += 1.0;
    if (IsKeyDown(KEY_D)) velocity.x -= 1.0;
    if (IsKeyDown(KEY_W)) velocity.y += 1.0;
    if (IsKeyDown(KEY_S)) velocity.y -= 1.0;

    // Swords
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
    // Armor
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

    // Equipment Stats
    baseSpeed = baseSpeedS + baseSpeedA;
    baseWeight = baseWeightS + baseWeightA;
    cleanDamage = cleanDamageS + cleanDamageA;
    baseForce = baseForceS + baseForceA;
    baseMaxHealth = baseMaxHealthS + baseMaxHealthA;
    baseCooldown = baseCooldownS + baseCooldownA;
    baseCritMult = baseCritMultS + baseCritMultA;
    baseCritChance = baseCritChanceS + baseCritChanceA;

    // Level Buffs
    cooldown = baseCooldown - (level - 1.f)*0.01;
    maxHealth = baseMaxHealth + (level - 1.f);
    baseDamage = cleanDamage + (level - 1.f)*0.05;
    force = baseForce + (level - 1.f)*0.02;
    weight = baseWeight + (level - 1.f)*0.2;
    speed = baseSpeed + (level - 1.f)*0.05;
    critChance = baseCritChance + (level - 1.f);
    critMult = baseCritMult + (level - 1.f)*0.05;
    damage = baseDamage;

    // Level System
    xpReq = 20.f * pow(1.6, level - 1.f);
    if (xp >= xpReq)    // Level Up
    {
        level++;
        skillPoints++;
        xp -= xpReq;
        maxHealth = baseMaxHealth + (level - 1.f);
        health = maxHealth;
    }

    // Sword Origin
    Vector2 origin{};
    Vector2 offset{};
    float rotation{};
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
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && getLastHit() > cooldown && getSwing() <= 0)
        {
            lastMove = "Swing";
            rotation = 40.f;
            setSwinging(true);
            setSwing(cooldown/2);
        }
        else if (getSwing() > 0 && lastMove == "Swing") // Swing duration
        {
            rotation = 40.f;
            setSwinging(true);
            setSwing(getSwing() - 1);
        }
        // Stab Attack
        else if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON) && getLastHit() > cooldown * 1.5 && getSwing() <= 0)
        {
            lastMove = "Stab";
            rotation = 45.f;
            offset = {45.f, 36.f};
            setSwinging(true);
            setSwing(cooldown);
        }
        else if (getSwing() >= cooldown * 0.7 && lastMove == "Stab") // Swing duration
        {
            rotation = 45.f;
            offset = {45.f, 36.f};
            setSwinging(true);
            setSwing(getSwing() - 1);
        }
        else if (getSwing() < cooldown * 0.7 && getSwing() > 0 && lastMove == "Stab") // Swing duration
        {
            rotation = 45.f;
            offset = {45.f, 36.f};
            setSwinging(false);
            setSwing(getSwing() - 1);
        }
        else
        {
            rotation = 0.f;
            offset = {38.f, 55.f};;
            setSwinging(false);
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
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && getLastHit() > cooldown && getSwing() <= 0)
        {
            lastMove = "Swing";
            rotation = -40.f;
            setSwinging(true);
            setSwing(cooldown/2);
        }
        else if (getSwing() > 0 && lastMove == "Swing") // Swing duration
        {
            rotation = -40.f;
            setSwinging(true);
            setSwing(getSwing() - 1);
        }
        // Stab Attack
        else if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON) && getLastHit() > cooldown * 1.5 && getSwing() <= 0)
        {
            lastMove = "Stab";
            rotation = -45.f;
            offset = {18.f, 36.f};
            setSwinging(true);
            setSwing(cooldown);
        }
        else if (getSwing() >= cooldown * 0.7 && lastMove == "Stab") // Swing duration
        {
            rotation = -45.f;
            offset = {18.f, 36.f};
            setSwinging(true);
            setSwing(getSwing() - 1);
        }
        else if (getSwing() < cooldown * 0.7 && getSwing() > 0 && lastMove == "Stab") // Swing duration
        {
            rotation = -45.f;
            offset = {18.f, 36.f};
            setSwinging(false);
            setSwing(getSwing() - 1);
        }
        else
        {
            rotation = 0.f;
            offset = {25.f, 55.f};;
            setSwinging(false);
        }
    }

    // Drawing Sword
    Rectangle source{0.f, 0.f, static_cast<float>(weapon.width) * rightLeft, static_cast<float>(weapon.height)};
    Rectangle dest{getScreenPos().x + offset.x, getScreenPos().y + offset.y, weapon.width * scale, weapon.height * scale};
    DrawTexturePro(weapon, source, dest, origin, rotation, WHITE);

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