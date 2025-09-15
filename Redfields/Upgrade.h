// Imports
#include <cstdio>
#include <string>
#include <iostream>
#include "raylib.h"
#include "raymath.h"
#include "Character.h"
using namespace std;

// Upgrade Class
class Upgrade
{
public:
    // Declarations
    Upgrade();
    Upgrade(Vector2 pos, Texture2D tex1, Texture2D tex2, Texture2D tex3, int gold, float point, float level);
    Rectangle getCollisionRec();
    void tick();
    bool getBought() const { return isBought; }
    void setBought(bool nam) { isBought = nam; }
    bool getPurchasable() const { return isPurchasable; }
    void setPurchasable(bool nam) { isPurchasable = nam; }
    bool getUnknown() const { return isUnknown; }
    void setUnknown(bool nam) { isUnknown = nam; }
    int getGoldCost() const { return goldCost; }
    void setGoldCost(int nam) { goldCost = nam; }
    int getPointCost() const { return pointCost; }
    int setPointCost(int nam) { pointCost = nam; }
    float getLevelReq() const { return levelReq; }
    void setLevelReq(float nam) { levelReq = nam; }
    
private:
    // Upgrade Variables
    int goldCost{};             // Cost of upgrade in gold
    int pointCost{};            // Cost of upgrade in skill points
    float levelReq{};           // Level requirement of upgrade
    bool isPurchasable{};       // If player meets requirements to purchase upgrade
    bool isBought{};            // If upgrade is already purchased
    bool isUnknown{true};       // If upgrade is not yet known
    Texture2D vacant{};         // Upgrade icon texture
    Texture2D purchasable{};    // Upgrade icon texture when purchasable
    Texture2D bought{};         // Upgrade icon texture when already purchased
    Texture2D unknown{LoadTexture("textures/gui/unknown.png")}; // Upgrade icon texture when not yet known
    Vector2 screenPos{};        // Upgrade icon screen pos
    float scale = 0.8;          // Upgrade icon texture scale
};