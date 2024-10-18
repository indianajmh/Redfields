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
Upgrade::Upgrade(Vector2 pos, Texture2D tex1, Texture2D tex2, Texture2D tex3, float gold, float point, float level) :
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
    if (isBought)
    {
        DrawTextureEx(bought, screenPos, 0.f, scale, WHITE);
    }
    else if (isPurchasable)
    {
        DrawTextureEx(purchasable, screenPos, 0.f, scale, WHITE);
    }
    else if (isUnknown)
    {
        DrawTextureEx(unknown, screenPos, 0.f, scale, WHITE);
    }
    else
    {
        DrawTextureEx(vacant, screenPos, 0.f, scale, WHITE);
    }
}