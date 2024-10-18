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
    float getSwing() const {return swing; }
    void setSwing(float num) {swing = num; }
    bool getSwinging() const {return swinging; }
    void setSwinging(bool num) {swinging = num; }
    string getLastMove() const {return lastMove; }
    float getXP() const {return xp;}
    void addXP(float num) {xp += num;}
    float getXPreq() const {return xpReq;}
    float getLevel() const {return level;}
    float getGold() const {return gold;}
    void addGold(float num) {gold += num;}
    void subtractGold(float num) {gold -= num;}
    float getSkillPoints() const {return skillPoints;}
    void addSkillPoints(float num) {skillPoints += num;}
    void subtractSkillPoints(float num) {skillPoints -= num;}
    bool getInstaKill() const {return instaKill; }
    void setInstaKill(bool num) {instaKill = num; }
    int getSword() const {return swordLevel;}
    void setSword(int num) {swordLevel = num;}
    int getArmor() const {return armorLevel;}
    void setArmor(int num) {armorLevel = num;}
private:
    int windowWidth{};
    int windowHeight{};
    Texture2D weapon{};
    Texture2D weapon0{LoadTexture("textures/characters/weapon_rustysword.png")};
    Texture2D weapon1{LoadTexture("textures/characters/weapon_ironsword.png")};
    Texture2D weapon2{LoadTexture("textures/characters/weapon_steelsword.png")};
    Texture2D weapon3{LoadTexture("textures/characters/weapon_gildedsword.png")};
    Rectangle weaponCollisionRec{};
    float swing{};
    string lastMove{};
    bool swinging{false};

    // Currency
    float level{1.f};
    float xp{};
    float xpReq{16.f};
    float gold{};
    float skillPoints{};

    // Upgrades
    int swordLevel{0};
    int armorLevel{0};

    // Effects
    bool instaKill{false};
};

#endif