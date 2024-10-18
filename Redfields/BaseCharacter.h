// Definition Check
#ifndef BASE_CHARACTER_H
#define BASE_CHARACTER_H

// Imports
#include <cstdio>
#include <string>
#include <iostream>
#include "raylib.h"
#include "raymath.h"
using namespace std;

// Base Character Class
class BaseCharacter
{
public:
    // Declarations
    BaseCharacter();
    Vector2 getWorldPos() { return worldPos; }
    void setWorldPos(Vector2 pos) { worldPos = pos; }
    virtual void tick(float deltaTime);
    void redoMovementX();
    void undoMovementX();
    void undoMovementY();
    Rectangle getCollisionRec();
    virtual Vector2 getScreenPos() = 0;
    bool getAlive() const { return alive; }
    float getDamage() const { return damage; }
    void setDamage(float hurt) { damage = hurt; }
    float getBaseDamage() const { return baseDamage; }
    void setBaseDamage(float hurt) { baseDamage = hurt; }
    float getHealth() const { return health; }
    void setHealth(float heal) { health = heal; }
    float getMaxHealth() const { return maxHealth; }
    void setMaxHealth(float max) { maxHealth = max; }
    float getSpeed() const { return speed; }
    void setSpeed(float fast) { speed = fast; }
    Vector2 getVelocity() const { return velocity; }
    void setVelocity(Vector2 fast) { velocity = fast; }
    Vector2 getLastVelocity() const { return lastVelocity; }
    void setLastVelocity(Vector2 fast) { lastVelocity = fast; }
    float getKnockback() const { return knockback; }
    void setKnockback(float ran) { knockback = ran; }
    Vector2 getKnockVector() const { return knockVector; }
    void setKnockVector(Vector2 ran) { knockVector = ran; }
    float getWeight() const {return weight; }
    void setWeight(float lbs) {weight = lbs; };
    float getForce() const { return force; }
    void setForce(float ran) { force = ran; }
    float getCooldown() const {return cooldown; }
    void setCooldown(float cool) { cooldown = cool; }
    float getCritMult() const {return critMult; }
    void setCritMult(float num) { critMult = num; }
    float getCritChance() const {return critChance; }
    void setCritChance(float num) { critChance = num; }
    virtual void giveDamage(float damage);
    void takeDamage(float damage);
    void setAlive(bool isAlive) { alive = isAlive; }
    float getLastHit() const {return lastHit; }
    void setLastHit(float ticks) {lastHit = ticks; };
    float getTexWidth() const {return texture.width; }
    float getTexHeight() const {return texture.height; }
    Color getTint() const { return tint; }
    void setTint(Color cool) { tint = cool; }
    float getScale() const {return scale; }
    void setScale(float num) { scale = num; }
    float getRightLeft() const {return rightLeft; }
    virtual void die() {alive = false;}
protected:
    // Textures
    Texture2D texture = LoadTexture("textures/characters/knight_idle_spritesheet.png");
    Texture2D idle = LoadTexture("textures/characters/knight_idle_spritesheet.png");
    Texture2D run = LoadTexture("textures/characters/knight_run_spritesheet.png");
    Color tint = WHITE;
    
    // Variables for Scrolling
    Vector2 worldPos{};
    float worldXNewFrame{};
    float worldXLastFrame{};
    float worldYLastFrame{};

    // Animation Variables
    float rightLeft = 1.f; // 1 = facing right, -1 = facing left
    float runningTime{};
    int frame{};
    int maxFrames{6};
    float updateTime{1.f / 12.f};
    Vector2 knockVector{};
    float width{};
    float height{};
    float scale{4.0f};
    Vector2 velocity{};
    Vector2 lastVelocity{};
    float knockback{};
    float lastHit{};

    // Stats
    float speed{};
    float weight{};
    float damage{};
    float baseDamage{};
    float force{};
    float maxHealth{};
    float health{};
    float cooldown{};
    float critMult{};
    float critChance{};

    float baseSpeed{};
    float baseWeight{};
    float cleanDamage{};
    float baseForce{};
    float baseMaxHealth{};
    float baseCooldown{};
    float baseCritMult{};
    float baseCritChance{};

    float baseSpeedS{};
    float baseWeightS{};
    float cleanDamageS{};
    float baseForceS{};
    float baseMaxHealthS{};
    float baseCooldownS{};
    float baseCritMultS{};
    float baseCritChanceS{};

    float baseSpeedA{};
    float baseWeightA{};
    float cleanDamageA{};
    float baseForceA{};
    float baseMaxHealthA{};
    float baseCooldownA{};
    float baseCritMultA{};
    float baseCritChanceA{};
private:
    // Alive
    bool alive{true};
};

#endif