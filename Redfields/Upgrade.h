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
    Upgrade(Vector2 pos, Texture2D tex1, Texture2D tex2, Texture2D tex3, float gold, float point, float level);
    Rectangle getCollisionRec();
    void tick();
    bool getBought() const { return isBought; }
    void setBought(bool nam) { isBought = nam; }
    bool getPurchasable() const { return isPurchasable; }
    void setPurchasable(bool nam) { isPurchasable = nam; }
    bool getUnknown() const { return isUnknown; }
    void setUnknown(bool nam) { isUnknown = nam; }
    float getGoldCost() const { return goldCost; }
    void setGoldCost(float nam) { goldCost = nam; }
    float getPointCost() const { return pointCost; }
    void setPointCost(float nam) { pointCost = nam; }
    float getLevelReq() const { return levelReq; }
    void setLevelReq(float nam) { levelReq = nam; }
    
private:
    // Upgrade Variables
    float goldCost{};
    float pointCost{};
    float levelReq{};
    bool isPurchasable{};
    bool isBought{};
    bool isUnknown{true};
    Texture2D vacant{};
    Texture2D purchasable{};
    Texture2D bought{};
    Texture2D unknown{LoadTexture("textures/gui/unknown.png")};
    Vector2 screenPos{};
    float scale = 0.8;
};