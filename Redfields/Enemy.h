// Definition Check
#ifndef ENEMY_H
#define ENEMY_H

// Imports
#include <cstdio>
#include <string>
#include <iostream>
#include "raylib.h"
#include "raymath.h"
#include "BaseCharacter.h"
#include "Character.h"
using namespace std;

// Enemy Class
class Enemy : public BaseCharacter
{
public:
    // Declarations
    Enemy(Vector2 pos, Texture2D idleTex, Texture2D runTex);
    virtual void tick(float deltaTime) override;
    void setTarget(Character* character) { target = character; }
    virtual Vector2 getScreenPos() override;
    virtual void giveDamage(float damage) override;
    float getDelay() const {return delay; }
    void setDelay(float ticks) {delay = ticks; };
    float getReach() const { return reach; }
    void setReach(float ran) { reach = ran; }
    float getRange() const { return range; }
    void setRange(float ran) { range = ran; }
    string getTag() const { return tag; }
    void setTag(string nam) { tag = nam; }
    bool getDropped() const { return dropped; }
    void setDropped(float drop) { dropped = drop; }
    bool isSpawned() const { return spawned; }
    void spawn() { spawned = true; }
    void setAgro(bool angry) { agro = angry; }
    virtual void die() override;
private:
    Character* target;      // Enemy targets a character (player)

    // Stats
    float delay{15.f};      // Delay in ticks when in contact with player before attacking
    float reach{};          // Range in which to attack player
    float range{};          // Range in which to see player and chase
    bool agro{false};       // When enemy is actively attacking player
    bool dropped{false};    // Loot has been dropped by the enemy
    bool spawned{false};    // Enemy has spawned in
    string tag{};           // The type of enemy
};

#endif