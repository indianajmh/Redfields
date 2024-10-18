// Definition Check
#ifndef PROP_H
#define PROP_H

// Imports
#include <cstdio>
#include <string>
#include <iostream>
#include "raylib.h"
#include "raymath.h"
using namespace std;

// Prop Class
class Prop
{
public:
    // Declarations
    Prop();
    Prop(Vector2 pos, Texture2D tex);
    void Render(Vector2 playerPos);
    Rectangle getCollisionRec(Vector2 playerPos);
    Rectangle getInteractiveRec(Vector2 playerPos);
private:
    // Prop Variables
    Texture2D texture{};
    Vector2 worldPos{};
    float scale = 4.f;
};

#endif