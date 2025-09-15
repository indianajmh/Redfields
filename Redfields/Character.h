// Definition Check
#ifndef CHARACTER_H
#define CHARACTER_H

// Imports
#include <cstdio>
#include <string>
#include <iostream>
#include "raylib.h"
#include "raymath.h"
#include "BaseCharacter.h"
using namespace std;

// Character Class
class Character : public BaseCharacter
{
public:
    // Declarations
    Character(int winWidth, int winHeight);
    virtual void tick(float deltaTime) override;
    virtual Vector2 getScreenPos() override;
    Rectangle getWeaponCollisionRec() { return weaponCollisionRec; }
    bool isSwinging() const {return swinging; }
    string getLastMove() const {return lastMove; }
    float getXP() const {return xp;}
    void addXP(float num) {xp += num;}
    float getXPreq() const {return xpReq;}
    int getLevel() const {return level;}
    int getGold() const {return gold;}
    void addGold(int num) {gold += num;}
    void subtractGold(int num) {gold -= num;}
    int getSkillPoints() const {return skillPoints;}
    void addSkillPoints(int num) {skillPoints += num;}
    void subtractSkillPoints(int num) {skillPoints -= num;}
    bool getInstaKill() const {return instaKill; }
    void setInstaKill(bool num) {instaKill = num; }
    int getSword() const {return swordLevel;}
    void setSword(int num) {swordLevel = num;}
    int getArmor() const {return armorLevel;}
    void setArmor(int num) {armorLevel = num;}

    void applySword();
    void applyArmor();
    void applyEquipment();
    void applyLevel();
    void levelUp();

    void displayHP();
private:
    int windowWidth{};  // Window width
    int windowHeight{}; // Window height
    Texture2D weapon{}; // Base weapon texture
    Texture2D weapon0{LoadTexture("textures/characters/weapon_rustysword.png")};    // Rusty sword texture
    Texture2D weapon1{LoadTexture("textures/characters/weapon_ironsword.png")};     // Iron sword texture
    Texture2D weapon2{LoadTexture("textures/characters/weapon_steelsword.png")};    // Steel sword texture
    Texture2D weapon3{LoadTexture("textures/characters/weapon_gildedsword.png")};   // Gilded sword texture
    Rectangle weaponCollisionRec{};     // Rectangle for weapon collision (can deal damage in this hit box)
    float swing{};  // Duration of attack move
    string lastMove{};  // Type of last attack (to continue attack for each tick)
    bool swinging{false};   // Whether the attack move is still in progress

    // Currency
    int level{1};       // Level
    float xp{};         // XP
    float xpReq{16.f};  // XP requirement to level up
    int gold{};         // Gold/currency
    int skillPoints{};  // Skill points (WIP)

    // Upgrades
    int swordLevel{0};  // Sword ID (what sword you have) 
    int armorLevel{0};  // Armor ID (what armor you have)

    // Effects
    bool instaKill{false};  // Is instakill active
};

#endif