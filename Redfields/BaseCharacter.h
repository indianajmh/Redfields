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
    void setTint(Color cool) { tint = cool; }
    float getScale() const {return scale; }
    void setScale(float num) { scale = num; }
    float getRightLeft() const {return rightLeft; }
    virtual void die() {alive = false;}
protected:
    // Textures
    Texture2D texture = LoadTexture("textures/characters/knight_idle_spritesheet.png");     // Base texture
    Texture2D idle = LoadTexture("textures/characters/knight_idle_spritesheet.png");        // Idle texture
    Texture2D run = LoadTexture("textures/characters/knight_run_spritesheet.png");          // Run texture
    Color tint = WHITE;     // tint
    
    // Variables for Scrolling
    Vector2 worldPos{}; // Position in the world
    float worldXNewFrame{}; // Next frame x position
    float worldXLastFrame{};    // Last frame x position
    float worldYLastFrame{};    // Last frame y position

    // Animation Variables
    float rightLeft = 1.f;          // 1 = facing right, -1 = facing left
    float runningTime{};            // Animation tick
    int frame{};                    // Animation frame
    int maxFrames{6};               // Max animation frames
    float updateTime{1.f / 12.f};   // Animation time
    Vector2 knockVector{};          // Knockback vector
    float width{};                  // Texture width
    float height{};                 // Texture height
    float scale{4.0f};              // Texture scaling
    Vector2 velocity{};             // Movement velocity vector
    Vector2 lastVelocity{};         // Previous movement velocity vector
    float knockback{};              // Knockback effect
    float lastHit{};                // When was last hit? for cooldown

    // Stats
    float speed{};      // Movement speed
    float weight{};     // Knockback resistance
    float damage{};     // End damage
    float baseDamage{}; // Damage before modifiers (after level up modifiers)
    float force{};      // How much knockback you give
    float maxHealth{};  // Max health
    float health{};     // Current health
    float cooldown{};   // Attack base cooldown
    float critMult{};   // Critical hit multiplier
    float critChance{}; // Critical hit chance

    // Equipment stat modifiers
    float baseSpeed{};      // Base movement speed before level up modifiers
    float baseWeight{};     // Base knockback resistance before level up modifiers
    float cleanDamage{};    // Base damage before level up modifiers
    float baseForce{};      // Base force before level up modifiers
    float baseMaxHealth{};  // Base max health before level up modifiers
    float baseCooldown{};   // Base attack cooldown before level up modifiers
    float baseCritMult{};   // Base critical hit multiplier before level up modifiers
    float baseCritChance{}; // Base critical hit chance before level up modifiers

    // Stat modifiers from sword type
    float baseSpeedS{};         // Base movement speed bonus from sword type
    float baseWeightS{};        // Base knockback resistance bonus from sword type
    float cleanDamageS{};       // Base damage from sword type
    float baseForceS{};         // Base force from sword type
    float baseMaxHealthS{};     // Base max health bonus from sword type
    float baseCooldownS{};      // Base attack cooldown from sword type
    float baseCritMultS{};      // Base critical hit multiplier from sword type
    float baseCritChanceS{};    // Base critical hit chance bonus from sword type

    // Stat modifiers from armor type
    float baseSpeedA{};         // Base movement speed bonus from armor type
    float baseWeightA{};        // Base knockback resistance bonus from armor type
    float cleanDamageA{};       // Base damage from armor type
    float baseForceA{};         // Base force from armor type
    float baseMaxHealthA{};     // Base max health bonus from armor type
    float baseCooldownA{};      // Base attack cooldown from armor type
    float baseCritMultA{};      // Base critical hit multiplier from armor type
    float baseCritChanceA{};    // Base critical hit chance bonus from armor type
private:
    // If alive
    bool alive{true};
};

#endif