// Imports
#include <cstdio>
#include <string>
#include <iostream>
#include "raylib.h"
#include "raymath.h"
#include "Upgrade.h"
using namespace std;

// Upgrade Constructor
Upgrade::Upgrade()
{

}
Upgrade::Upgrade(Vector2 pos, Texture2D tex1, Texture2D tex2, Texture2D tex3, int gold, float point, float level) :
    screenPos(pos), 
    vacant(tex1),
    purchasable(tex2),
    bought(tex3),
    goldCost(gold),
    pointCost(point),
    levelReq(level)
{

}

// Collision Rectangle for Upgrade
Rectangle Upgrade::getCollisionRec()
{
    return Rectangle
    {
        screenPos.x,
        screenPos.y,
        vacant.width * scale,
        vacant.height * scale
    };

}

// Tick for Upgrade
void Upgrade::tick()
{
    if (isBought)   // if upgrade is purchased, display subsequent texture
    {
        DrawTextureEx(bought, screenPos, 0.f, scale, WHITE);
    }
    else if (isPurchasable) // if upgrade is purchasable, display subsequent texture
    {
        DrawTextureEx(purchasable, screenPos, 0.f, scale, WHITE);
    }
    else if (isUnknown) // if upgrade is unknown, display subsequent texture
    {
        DrawTextureEx(unknown, screenPos, 0.f, scale, WHITE);
    }
    else    // if upgrade is not purchased or anything else, display subsequent texture
    {
        DrawTextureEx(vacant, screenPos, 0.f, scale, WHITE);
    }
}