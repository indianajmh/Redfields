// Imports
#include <cstdio>
#include <string>
#include <iostream>
#include "raylib.h"
#include "raymath.h"
#include "Item.h"
using namespace std;

// Item Constructor
Item::Item(Vector2 pos, Texture2D tex, string type, float size) :
    worldPos(pos),
    texture(tex),
    tag(type),
    scale(size)
{
    
}

// Rendering Items
void Item::Render(Vector2 playerPos)
{
    Vector2 screenPos = Vector2Subtract(worldPos, playerPos);
    DrawTextureEx(texture, screenPos, 0.f, scale, WHITE);
}

// Collision Rectangle for Items
Rectangle Item::getCollisionRec(Vector2 playerPos)
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