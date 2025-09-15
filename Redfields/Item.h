// Definition Check
#ifndef ITEM_H
#define ITEM_H

// Imports
#include <cstdio>
#include <string>
#include <iostream>
#include "raylib.h"
#include "raymath.h"
#include "Prop.h"
using namespace std;

// Item Class
class Item : public Prop
{
public:
    // Declarations
    Item(Vector2 pos, Texture2D tex, string type, float size);
    void Render(Vector2 playerPos);
    Rectangle getCollisionRec(Vector2 playerPos);
    Vector2 getWorldPos() const { return worldPos; }
    void setWorldPos(Vector2 huh) { worldPos = huh; }
    string getTag() const { return tag; }
    void setTag(string nam) { tag = nam; }
private:
    // Item Variables
    Texture2D texture{};    // item texture
    Vector2 worldPos{};     // item position in world
    float scale = 1.f;      // item texture scale
    string tag{};           // item type
};

#endif