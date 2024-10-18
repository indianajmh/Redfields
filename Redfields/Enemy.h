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
    Character* target;

    // Stats
    float delay{15.f};
    float reach{};
    float range{};
    bool agro{false};
    bool dropped{false};
    bool spawned{false};
    string tag{};
};

#endif