// Imports
#include <cstdio>
#include <string>
#include <iostream>
#include "raylib.h"
#include "raymath.h"
#include "Prop.h"
using namespace std;

// Prop Constructor
Prop::Prop()
{

}
Prop::Prop(Vector2 pos, Texture2D tex) :
    worldPos(pos), 
    texture(tex)
{

}

// Rendering Props
void Prop::Render(Vector2 playerPos)
{
    Vector2 screenPos = Vector2Subtract(worldPos, playerPos);
    DrawTextureEx(texture, screenPos, 0.f, scale, WHITE);
}

// Collision Rectangle for Props
Rectangle Prop::getCollisionRec(Vector2 playerPos)
{
    Vector2 screenPos = Vector2Subtract(worldPos, playerPos);
    return Rectangle
    {
        screenPos.x,
        screenPos.y,
        texture.width * scale,
        texture.height * scale
    };

}

// Interactive Rectangle for Shops
Rectangle Prop::getInteractiveRec(Vector2 playerPos)
{
    Vector2 screenPos = Vector2Subtract(worldPos, playerPos);
    return Rectangle
    {
        screenPos.x - 35.f,
        screenPos.y - 15.f,
        texture.width * scale + 55.f,
        texture.height * scale + 55.f
    };

}