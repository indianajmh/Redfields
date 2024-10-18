// Imports
#include <cstdio>
#include <string>
#include <vector>
#include <iostream>
#include "raylib.h"
#include "raymath.h"
#include "Character.h"
#include "Prop.h"
#include "Item.h"
#include "Enemy.h"
#include "Upgrade.h"
using namespace std;

// Window Variables
const int winHeight = 720;
const int winWidth = 1280;

// Random Spawnpoint
Vector2 randPos()
{
    return {static_cast <float> (rand()) / static_cast <float> (RAND_MAX), static_cast <float> (rand()) / static_cast <float> (RAND_MAX)};
}

// Main Struct
int main()
{
    // Creating Window
    InitWindow(winWidth, winHeight, "Classy Clasher");

    // Map Components
    Texture2D map = LoadTexture("textures/nature_tileset/OpenWorldMap24x24.png");
    Vector2 mapPos{0.0, 0.0};
    const float mapScale = 4.0f;
    Rectangle worldBorder{190, 190, 2690, 2680};

    // GUI
    Texture2D shop = LoadTexture("textures/gui/Shop_Background.png");

    // Initializing Player
    Character player{winWidth, winHeight};
    player.setWorldPos({worldBorder.width/2 - winWidth/2, worldBorder.height/2 - winHeight/2});
    float spawnRad = 600.f;
    Rectangle spawnProt{winWidth/2 - spawnRad, winHeight/2 - spawnRad, spawnRad*2, spawnRad*2};

    // Initializing Upgrades
    int swordCount = 4;
    Upgrade swords[swordCount]
    {
        // Basic Sword
        Upgrade{{100, 300}, 
        LoadTexture("textures/gui/rustysword_bought.png"), 
        LoadTexture("textures/gui/rustysword_bought.png"), 
        LoadTexture("textures/gui/rustysword_bought.png"), 
        0, 0, 1},
        // Iron Sword
        Upgrade{{380, 300}, 
        LoadTexture("textures/gui/ironsword_vacant.png"), 
        LoadTexture("textures/gui/ironsword_purchasable.png"), 
        LoadTexture("textures/gui/ironsword_bought.png"), 
        25, 0, 3},
        // Steel Sword
        Upgrade{{660, 300}, 
        LoadTexture("textures/gui/steelsword_vacant.png"), 
        LoadTexture("textures/gui/steelsword_purchasable.png"), 
        LoadTexture("textures/gui/steelsword_bought.png"), 
        50, 0, 5},
        // Gilded Sword
        Upgrade{{940, 300}, 
        LoadTexture("textures/gui/gildedsword_vacant.png"), 
        LoadTexture("textures/gui/gildedsword_purchasable.png"), 
        LoadTexture("textures/gui/gildedsword_bought.png"), 
        200, 0, 10}
    };
    Texture2D swordStats[swordCount]
    {
        // Basic Sword
        Texture2D{LoadTexture("textures/gui/rustysword_stats.png")},
        // Iron Sword
        Texture2D{LoadTexture("textures/gui/ironsword_stats.png")},
        // Steel Sword
        Texture2D{LoadTexture("textures/gui/steelsword_stats.png")},
        // Gilded Sword
        Texture2D{LoadTexture("textures/gui/gildedsword_stats.png")}
    };
    swords[0].setBought(true);
    swords[0].setUnknown(false);

    // Initializing Props
    Prop props[8]
    {
        // Totem Shop
        Prop{Vector2{1100.f, 1400.f}, LoadTexture("textures/nature_tileset/Totem.png")},

        // Rocks
        Prop{Vector2{600.f, 600.f}, LoadTexture("textures/nature_tileset/Rock.png")},
        Prop{Vector2{1200.f, 500.f}, LoadTexture("textures/nature_tileset/Rock.png")},
        Prop{Vector2{1500.f, 1000.f}, LoadTexture("textures/nature_tileset/Rock.png")},
        Prop{Vector2{1800.f, 1600.f}, LoadTexture("textures/nature_tileset/Rock.png")},
        
        // Logs
        Prop{Vector2{400.f, 500.f}, LoadTexture("textures/nature_tileset/Log.png")},
        Prop{Vector2{800.f, 1000.f}, LoadTexture("textures/nature_tileset/Log.png")},
        Prop{Vector2{1600.f, 1500.f}, LoadTexture("textures/nature_tileset/Log.png")}
    };

    // Enemies + Wave Variables
    int totalGoblins = 0;
    int totalSuperGoblins = 0;
    int totalGoblinBodyguards = 0;
    int totalGodblins = 0;
    int totalSlimes = 0;
    int totalSuperSlimes = 0;
    int totalNickelodeons = 0;
    int totalGobbies = 0;
    int totalBosses = totalNickelodeons + totalGobbies;
    int totalEnemies = totalGoblins + totalSuperGoblins + totalGoblinBodyguards + totalGodblins + totalSlimes + totalSuperSlimes + totalBosses;
    int enemiesDefeated = totalEnemies;
    int enemiesLeft = 0;
    int enemiesSpawned = 0;
    float enemiesHit = 0.f;
    float damageDelt = 0.f;
    bool waveStarted = false;
    bool waveActive = false;
    bool gamemodeSelected = false;
    int wave = 0;
    float spawnRate = 0.f;
    float difficulty = 0.f;
    float gamemode = 0.f;
    float spawnCooldown = 0.f;

    // Initializing Goblin
    Enemy goblin
    {
        Vector2{},
        LoadTexture("textures/characters/goblin_idle_spritesheet.png"),
        LoadTexture("textures/characters/goblin_run_spritesheet.png")
    };

    // Initializing Super Goblin
    Enemy superGoblin
    {
        Vector2{},
        LoadTexture("textures/characters/goblin_idle_spritesheet.png"),
        LoadTexture("textures/characters/goblin_run_spritesheet.png")
    };

    // Initializing Goblin Bodyguard
    Enemy goblinBodyguard
    {
        Vector2{},
        LoadTexture("textures/characters/goblin_idle_spritesheet.png"),
        LoadTexture("textures/characters/goblin_run_spritesheet.png")
    };

    // Initializing Godblin
    Enemy godblin
    {
        Vector2{},
        LoadTexture("textures/characters/goblin_idle_spritesheet.png"),
        LoadTexture("textures/characters/goblin_run_spritesheet.png")
    };

    // Initializing Slime
    Enemy slime
    {
        Vector2{},
        LoadTexture("textures/characters/slime_idle_spritesheet.png"),
        LoadTexture("textures/characters/slime_run_spritesheet.png")
    };

    // Initializing Super Slime
    Enemy superSlime
    {
        Vector2{},
        LoadTexture("textures/characters/slime_idle_spritesheet.png"),
        LoadTexture("textures/characters/slime_run_spritesheet.png")
    };

    // Initializing Nickelodeon the Slime
    Enemy nickelodeon
    {
        Vector2{},
        LoadTexture("textures/characters/slime_idle_spritesheet.png"),
        LoadTexture("textures/characters/slime_run_spritesheet.png")
    };

    // Initializing Gobby the Goblin
    Enemy gobby
    {
        Vector2{},
        LoadTexture("textures/characters/goblin_idle_spritesheet.png"),
        LoadTexture("textures/characters/goblin_run_spritesheet.png")
    };

    // Initializing Enemy Vector Array
    vector<Enemy> enemies;

    // Initializing Gold
    Item gold
    {
        Vector2{},
        LoadTexture("textures/items/coin.png"),
        "Gold",
        1.3
    };

    // Initializing Hearts
    Item heart
    {
        Vector2{},
        LoadTexture("textures/items/heart.png"),
        "Heart",
        2.3
    };

    // Initializing Super Hearts
    Item superHeart
    {
        Vector2{},
        LoadTexture("textures/items/super_heart.png"),
        "Super Heart",
        2.3
    };

    // Initializing Insta Kill
    Item instaKill
    {
        Vector2{},
        LoadTexture("textures/items/insta_kill.png"),
        "Insta Kill",
        1.4
    };

    // Initializing Fire Bomb
    Item fireBomb
    {
        Vector2{},
        LoadTexture("textures/items/fire_bomb.png"),
        "Fire Bomb",
        1.6
    };

    // Initializing Item Vector Array
    vector<Item> items;

    // Hit Marker Variables
    Vector2 markerPos{};
    Vector2 dWorldPos = player.getWorldPos();
    float markerYMod{};
    float markerDelay{};
    float markerSplit{};

    // Ticks
    float tick{};
    float instaCool{};

    // Program Loop
    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground({99, 155, 255, 255});

        // Moving Map
        mapPos = Vector2Scale(player.getWorldPos(), -1.f);

        // Drawing Map
        DrawTextureEx(map, mapPos, 0.0, mapScale, WHITE);
        
        /*
        DrawRectangle(worldBorder.x - player.getWorldPos().x, 
            worldBorder.y - player.getWorldPos().y, 
            worldBorder.width, worldBorder.height, {230, 41, 55, 100});
        */
        //DrawRectangle(props[8].getCollisionRec(player.getWorldPos()).x, props[8].getCollisionRec(player.getWorldPos()).y, props[8].getCollisionRec(player.getWorldPos()).width, props[8].getCollisionRec(player.getWorldPos()).height, {0, 120, 240, 105});
        
        
        // Rendering Props
        for (auto prop : props)
        {
            prop.Render(player.getWorldPos());
        }

        // Ticks
        tick++;
        player.setLastHit(player.getLastHit() + 1); // Player Tick Cooldown
        for(int i = 0; i < static_cast<int>(items.size()); i++) // Item Tick
        {
            items[i].Render(player.getWorldPos());
            //  Collision
            if (CheckCollisionRecs(items[i].getCollisionRec(player.getWorldPos()), player.getCollisionRec()))
            {
                if (items[i].getTag() == "Gold")
                {
                    player.addGold(1.f);
                }
                if (items[i].getTag() == "Heart")
                {
                    player.setHealth(player.getHealth() + 1.f);
                }
                if (items[i].getTag() == "Super Heart")
                {
                    player.setHealth(player.getMaxHealth());
                }
                if (items[i].getTag() == "Insta Kill")
                {
                    if (gamemode == 1) instaCool = 30.f * (1.f / GetFrameTime());
                    if (gamemode == 2) instaCool = 20.f * (1.f / GetFrameTime());
                    if (gamemode == 3) instaCool = 15.f * (1.f / GetFrameTime());
                    if (gamemode == 4) instaCool = 10.f * (1.f / GetFrameTime());
                }
                if (items[i].getTag() == "Fire Bomb")
                {
                    for(int i = 0; i < static_cast<int>(enemies.size()); i++)
                    {
                        if (enemies[i].isSpawned() && enemies[i].getAlive() && !(enemies[i].getTag() == "Nickelodeon the Slime" || enemies[i].getTag() == "Gobby the Goblin"))
                        {
                            enemies[i].die();
                        }
                    }
                }
                items.erase(items.begin() + i);
            }
        }
        for(int i = 0; i < static_cast<int>(enemies.size()); i++)    // Enemy Tick Cooldown
        {
            enemies[i].setLastHit(enemies[i].getLastHit() + 1);
        }
        player.tick(GetFrameTime());

        // Insta Kill Timer
        if (instaCool > 0.f)
        {
            player.setDamage(999999.9);
            std::string instaText = "Insta Kill: ";
            instaText.append(std::to_string((int)(instaCool/60.f)), 0, 5);
            DrawText(instaText.c_str(), winWidth - 300.f, winHeight - 50.f, 40, WHITE);
            instaCool--;
        }
        else
        {
            player.setDamage(player.getBaseDamage());
        }

        //DrawText(std::to_string(swords.size()).c_str(), winWidth/2.5, winHeight - 45.f, 40, WHITE);

        // Enemy Healing... ;)
        /*
        if (((int)tick % 120) == 0)
        {
            for(int i = 0; i < static_cast<int>(enemies.size()); i++)
            {
                enemies[i].takeDamage(-1);
            }
        }   */

        // Checking Map X Boundaries
        if (player.getWorldPos().x < worldBorder.x - winWidth/2 + player.getTexWidth()/6 ||
            player.getWorldPos().x > worldBorder.x - winWidth/2 - player.getTexWidth()/6 + worldBorder.width)
        {
            player.undoMovementX();
        }
        // Checking Map Y Boundaries
        else if (player.getWorldPos().y < worldBorder.y - winHeight/2 - player.getTexHeight() ||
            player.getWorldPos().y > worldBorder.y - winHeight/2 - player.getTexHeight() + worldBorder.height)
        {
            player.undoMovementY();
        }

        // Checking Prop Collision
        for (auto prop : props)
        {
            // Collision X
            if (CheckCollisionRecs(prop.getCollisionRec(player.getWorldPos()), player.getCollisionRec()))
            {
                player.undoMovementX();
            }
            // Collision Y
            if (CheckCollisionRecs(prop.getCollisionRec(player.getWorldPos()), player.getCollisionRec()))
            {
                player.redoMovementX();
                player.undoMovementY();
            }
            // Collision X + Y
            if (CheckCollisionRecs(prop.getCollisionRec(player.getWorldPos()), player.getCollisionRec()))
            {
                player.undoMovementX();
            }
        }

        // Starting Wave
        if (waveStarted)
        {
            // Initializing Wave Settings
            if (wave == 1)  // Wave 1
            {
                // Enemies
                totalGoblins = 8;
                totalSuperGoblins = 0;
                totalGoblinBodyguards = 0;
                totalGodblins = 0;
                totalSlimes = 0;
                totalSuperSlimes = 0;
                totalNickelodeons = 0;
                totalGobbies = 0;

                // Settings
                spawnRate = 0.3;
                difficulty = 0.2;
            }
            if (wave == 2)  // Wave 2
            {
                // Enemies
                totalGoblins = 16;
                totalSuperGoblins = 0;
                totalGoblinBodyguards = 0;
                totalGodblins = 0;
                totalSlimes = 0;
                totalSuperSlimes = 0;
                totalNickelodeons = 0;
                totalGobbies = 0;

                // Settings
                spawnRate = 0.33;
                difficulty = 0.22;
            }
            if (wave == 3)  // Wave 3 (BOSS)
            {
                // Enemies
                totalGoblins = 0;
                totalSuperGoblins = 0;
                totalGoblinBodyguards = 0;
                totalGodblins = 0;
                totalSlimes = 15;
                totalSuperSlimes = 0;
                totalNickelodeons = 1;
                totalGobbies = 0;

                // Settings
                spawnRate = 0.36;
                difficulty = 0.24;
            }
            if (wave == 4)  // Wave 4
            {
                // Enemies
                totalGoblins = 16;
                totalSuperGoblins = 0;
                totalGoblinBodyguards = 0;
                totalGodblins = 0;
                totalSlimes = 5;
                totalSuperSlimes = 0;
                totalNickelodeons = 0;
                totalGobbies = 0;

                // Settings
                spawnRate = 0.39;
                difficulty = 0.26;
            }
            if (wave == 5)  // Wave 5
            {
                // Enemies
                totalGoblins = 20;
                totalSuperGoblins = 0;
                totalGoblinBodyguards = 0;
                totalGodblins = 0;
                totalSlimes = 10;
                totalSuperSlimes = 0;
                totalNickelodeons = 0;
                totalGobbies = 0;

                // Settings
                spawnRate = 0.42;
                difficulty = 0.28;
            }
            if (wave == 6)  // Wave 6 (CHALLENGE)
            {
                // Enemies
                totalGoblins = 0;
                totalSuperGoblins = 28;
                totalGoblinBodyguards = 0;
                totalGodblins = 0;
                totalSlimes = 0;
                totalSuperSlimes = 0;
                totalNickelodeons = 0;
                totalGobbies = 0;

                // Settings
                spawnRate = 0.4;
                difficulty = 0.3;
            }
            if (wave == 7)  // Wave 7
            {
                // Enemies
                totalGoblins = 24;
                totalSuperGoblins = 4;
                totalGoblinBodyguards = 0;
                totalGodblins = 0;
                totalSlimes = 12;
                totalSuperSlimes = 0;
                totalNickelodeons = 0;
                totalGobbies = 0;

                // Settings
                spawnRate = 0.44;
                difficulty = 0.32;
            }
            if (wave == 8)  // Wave 8
            {
                // Enemies
                totalGoblins = 24;
                totalSuperGoblins = 8;
                totalGoblinBodyguards = 0;
                totalGodblins = 0;
                totalSlimes = 16;
                totalSuperSlimes = 0;
                totalNickelodeons = 0;
                totalGobbies = 0;

                // Settings
                spawnRate = 0.47;
                difficulty = 0.34;
            }
            if (wave == 9)  // Wave 9 (BOSS)
            {
                // Enemies
                totalGoblins = 0;
                totalSuperGoblins = 0;
                totalGoblinBodyguards = 40;
                totalGodblins = 0;
                totalSlimes = 0;
                totalSuperSlimes = 0;
                totalNickelodeons = 0;
                totalGobbies = 1;

                // Settings
                spawnRate = 0.44;
                difficulty = 0.36;
            }
            if (wave == 10)  // Wave 10
            {
                // Enemies
                totalGoblins = 27;
                totalSuperGoblins = 12;
                totalGoblinBodyguards = 1;
                totalGodblins = 0;
                totalSlimes = 20;
                totalSuperSlimes = 0;
                totalNickelodeons = 0;
                totalGobbies = 0;

                // Settings
                spawnRate = 0.5;
                difficulty = 0.38;
            }
            if (wave == 11)  // Wave 11
            {
                // Enemies
                totalGoblins = 26;
                totalSuperGoblins = 16;
                totalGoblinBodyguards = 4;
                totalGodblins = 0;
                totalSlimes = 24;
                totalSuperSlimes = 0;
                totalNickelodeons = 0;
                totalGobbies = 0;

                // Settings
                spawnRate = 0.53;
                difficulty = 0.4;
            }
            if (wave == 12)  // Wave 12 (CHALLENGE)
            {
                // Enemies
                totalGoblins = 0;
                totalSuperGoblins = 0;
                totalGoblinBodyguards = 0;
                totalGodblins = 0;
                totalSlimes = 0;
                totalSuperSlimes = 64;
                totalNickelodeons = 0;
                totalGobbies = 0;

                // Settings
                spawnRate = 0.56;
                difficulty = 0.42;
            }

            // Applying Modifiers
            totalGoblins = (int)((totalGoblins)*(gamemode/5.f + 0.7));
            totalSuperGoblins = (int)((totalSuperGoblins)*(gamemode/5.f + 0.7));
            totalGoblinBodyguards = (int)((totalGoblinBodyguards)*(gamemode/5.f + 0.7));
            totalGodblins = (int)((totalGodblins)*(gamemode/5.f + 0.7));
            totalSlimes = (int)((totalSlimes)*(gamemode/5.f + 0.7));
            totalSuperSlimes = (int)((totalSuperSlimes)*(gamemode/5.f + 0.7));
            spawnRate *= gamemode/6.f + 0.8;

            // Initializing Other Variables
            totalBosses = totalNickelodeons + totalGobbies;
            totalEnemies = totalGoblins + totalSuperGoblins + totalGoblinBodyguards + totalGodblins + totalSlimes + totalSuperSlimes + totalBosses;
            enemiesDefeated = 0;
            enemiesSpawned = 0;
            enemiesLeft = totalEnemies;
            waveStarted = false;
            enemies.clear();
            
            // Initializing Vector Array for Wave and Different Enemy Types
            for(int i = 0; i < totalGoblins; i++)   // Goblins
            {
                // Spawning
                enemies.push_back(goblin);
                enemies[i].setWorldPos(Vector2Add(Vector2Scale(randPos(), worldBorder.height - enemies[i].getTexHeight()), {190, 190}));
                enemies[i].setTarget(&player);
                // Stats
                if (gamemode == 1.f) enemies[i].setMaxHealth(2.f * (difficulty + 0.8));
                if (gamemode == 2.f) enemies[i].setMaxHealth(3.f * (difficulty + 0.8));
                if (gamemode == 3.f) enemies[i].setMaxHealth(4.f * (difficulty + 0.8));
                if (gamemode == 4.f) enemies[i].setMaxHealth(6.f * (difficulty + 0.8));
                enemies[i].setHealth(enemies[i].getMaxHealth());
                if (gamemode == 1.f) enemies[i].setDamage(0.8 * (difficulty + 0.8));
                if (gamemode == 2.f) enemies[i].setDamage(1.f * (difficulty + 0.8));
                if (gamemode == 3.f) enemies[i].setDamage(1.5 * (difficulty + 0.8));
                if (gamemode == 4.f) enemies[i].setDamage(2.f * (difficulty + 0.8));
                if (gamemode == 1.f) enemies[i].setCritChance(0.f);
                if (gamemode == 2.f) enemies[i].setCritChance(0.f + (difficulty - 0.2)*15.f);
                if (gamemode == 3.f) enemies[i].setCritChance(1.f + (difficulty - 0.2)*25.f);
                if (gamemode == 4.f) enemies[i].setCritChance(2.f + (difficulty - 0.2)*50.f);
                if (gamemode == 1.f) enemies[i].setCritMult(0.f);
                if (gamemode == 2.f) enemies[i].setCritMult(1.f * (difficulty + 0.8));
                if (gamemode == 3.f) enemies[i].setCritMult(1.2 * (difficulty + 0.8));
                if (gamemode == 4.f) enemies[i].setCritMult(1.5 * (difficulty + 0.8));
                enemies[i].setCooldown(35.f);
                enemies[i].setSpeed(3.f);
                enemies[i].setForce(1.f);
                enemies[i].setWeight(3.f);
                enemies[i].setReach(25.f);
                enemies[i].setRange(500.f);
                enemies[i].setScale(4.f);
                enemies[i].setAgro(false);
                enemies[i].setTint(WHITE);
                enemies[i].setTag("Goblin");
            }
            for(int i = totalGoblins; i < totalGoblins + totalSuperGoblins; i++)   // Super Goblins
            {
                // Spawning
                enemies.push_back(superGoblin);
                enemies[i].setWorldPos(Vector2Add(Vector2Scale(randPos(), worldBorder.height - enemies[i].getTexHeight()), {190, 190}));
                enemies[i].setTarget(&player);
                // Stats
                if (gamemode == 1.f) enemies[i].setMaxHealth(4.f * (difficulty + 0.8));
                if (gamemode == 2.f) enemies[i].setMaxHealth(6.f * (difficulty + 0.8));
                if (gamemode == 3.f) enemies[i].setMaxHealth(8.f * (difficulty + 0.8));
                if (gamemode == 4.f) enemies[i].setMaxHealth(12.f * (difficulty + 0.8));
                enemies[i].setHealth(enemies[i].getMaxHealth());
                if (gamemode == 1.f) enemies[i].setDamage(1.6 * (difficulty + 0.8));
                if (gamemode == 2.f) enemies[i].setDamage(2.f * (difficulty + 0.8));
                if (gamemode == 3.f) enemies[i].setDamage(3.f * (difficulty + 0.8));
                if (gamemode == 4.f) enemies[i].setDamage(4.f * (difficulty + 0.8));
                if (gamemode == 1.f) enemies[i].setCritChance(0.f);
                if (gamemode == 2.f) enemies[i].setCritChance(0.f + (difficulty - 0.2)*15.f);
                if (gamemode == 3.f) enemies[i].setCritChance(1.f + (difficulty - 0.2)*25.f);
                if (gamemode == 4.f) enemies[i].setCritChance(2.f + (difficulty - 0.2)*50.f);
                if (gamemode == 1.f) enemies[i].setCritMult(0.f);
                if (gamemode == 2.f) enemies[i].setCritMult(1.f * (difficulty + 0.8));
                if (gamemode == 3.f) enemies[i].setCritMult(1.2 * (difficulty + 0.8));
                if (gamemode == 4.f) enemies[i].setCritMult(1.5 * (difficulty + 0.8));
                enemies[i].setCooldown(30.f);
                enemies[i].setSpeed(3.5);
                enemies[i].setForce(1.2);
                enemies[i].setWeight(4.f);
                enemies[i].setReach(25.f);
                enemies[i].setRange(500.f);
                enemies[i].setScale(4.f);
                enemies[i].setAgro(false);
                enemies[i].setTint(RED);
                enemies[i].setTag("Super Goblin");
            }
            for(int i = totalGoblins + totalSuperGoblins; i < totalGoblins + totalSuperGoblins + totalGoblinBodyguards; i++)   // Goblin Bodyguards
            {
                // Spawning
                enemies.push_back(goblinBodyguard);
                enemies[i].setWorldPos(Vector2Add(Vector2Scale(randPos(), worldBorder.height - enemies[i].getTexHeight()), {190, 190}));
                enemies[i].setTarget(&player);
                // Stats
                if (gamemode == 1.f) enemies[i].setMaxHealth(8.f * (difficulty + 0.8));
                if (gamemode == 2.f) enemies[i].setMaxHealth(12.f * (difficulty + 0.8));
                if (gamemode == 3.f) enemies[i].setMaxHealth(16.f * (difficulty + 0.8));
                if (gamemode == 4.f) enemies[i].setMaxHealth(24.f * (difficulty + 0.8));
                enemies[i].setHealth(enemies[i].getMaxHealth());
                if (gamemode == 1.f) enemies[i].setDamage(2.4 * (difficulty + 0.8));
                if (gamemode == 2.f) enemies[i].setDamage(3.f * (difficulty + 0.8));
                if (gamemode == 3.f) enemies[i].setDamage(4.5 * (difficulty + 0.8));
                if (gamemode == 4.f) enemies[i].setDamage(6.f * (difficulty + 0.8));
                if (gamemode == 1.f) enemies[i].setCritChance(0.f);
                if (gamemode == 2.f) enemies[i].setCritChance(0.f + (difficulty - 0.2)*15.f);
                if (gamemode == 3.f) enemies[i].setCritChance(1.f + (difficulty - 0.2)*25.f);
                if (gamemode == 4.f) enemies[i].setCritChance(2.f + (difficulty - 0.2)*50.f);
                if (gamemode == 1.f) enemies[i].setCritMult(0.f);
                if (gamemode == 2.f) enemies[i].setCritMult(1.f * (difficulty + 0.8));
                if (gamemode == 3.f) enemies[i].setCritMult(1.2 * (difficulty + 0.8));
                if (gamemode == 4.f) enemies[i].setCritMult(1.5 * (difficulty + 0.8));
                enemies[i].setCooldown(30.f);
                enemies[i].setSpeed(2.7);
                enemies[i].setForce(1.5);
                enemies[i].setWeight(8.f);
                enemies[i].setReach(25.f);
                enemies[i].setRange(500.f);
                enemies[i].setScale(4.4);
                enemies[i].setAgro(false);
                enemies[i].setTint({ 205, 205, 205, 255});
                enemies[i].setTag("Goblin Bodyguard");
            }
            for(int i = totalGoblins + totalSuperGoblins + totalGoblinBodyguards; i < totalGoblins + totalSuperGoblins + totalGoblinBodyguards + totalGodblins; i++)   // Godblins
            {
                // Spawning
                enemies.push_back(godblin);
                enemies[i].setWorldPos(Vector2Add(Vector2Scale(randPos(), worldBorder.height - enemies[i].getTexHeight()), {190, 190}));
                enemies[i].setTarget(&player);
                // Stats
                if (gamemode == 1.f) enemies[i].setMaxHealth(40.f * (difficulty + 0.8));
                if (gamemode == 2.f) enemies[i].setMaxHealth(60.f * (difficulty + 0.8));
                if (gamemode == 3.f) enemies[i].setMaxHealth(80.f * (difficulty + 0.8));
                if (gamemode == 4.f) enemies[i].setMaxHealth(120.f * (difficulty + 0.8));
                enemies[i].setHealth(enemies[i].getMaxHealth());
                if (gamemode == 1.f) enemies[i].setDamage(4.f * (difficulty + 0.8));
                if (gamemode == 2.f) enemies[i].setDamage(5.f * (difficulty + 0.8));
                if (gamemode == 3.f) enemies[i].setDamage(7.f * (difficulty + 0.8));
                if (gamemode == 4.f) enemies[i].setDamage(10.f * (difficulty + 0.8));
                if (gamemode == 1.f) enemies[i].setCritChance(0.f);
                if (gamemode == 2.f) enemies[i].setCritChance(0.f + (difficulty - 0.2)*15.f);
                if (gamemode == 3.f) enemies[i].setCritChance(1.f + (difficulty - 0.2)*25.f);
                if (gamemode == 4.f) enemies[i].setCritChance(2.f + (difficulty - 0.2)*50.f);
                if (gamemode == 1.f) enemies[i].setCritMult(0.f);
                if (gamemode == 2.f) enemies[i].setCritMult(1.f * (difficulty + 0.8));
                if (gamemode == 3.f) enemies[i].setCritMult(1.2 * (difficulty + 0.8));
                if (gamemode == 4.f) enemies[i].setCritMult(1.5 * (difficulty + 0.8));
                enemies[i].setCooldown(30.f);
                enemies[i].setSpeed(2.5);
                enemies[i].setForce(4.f);
                enemies[i].setWeight(40.f);
                enemies[i].setReach(25.f);
                enemies[i].setRange(500.f);
                enemies[i].setScale(8.f);
                enemies[i].setAgro(false);
                enemies[i].setTint(BLACK);
                enemies[i].setTag("Godblin");
            }
            for(int i = totalGoblins + totalSuperGoblins + totalGoblinBodyguards + totalGodblins; i < totalGoblins + totalSuperGoblins + totalGoblinBodyguards + totalGodblins + totalSlimes; i++)  // Slimes
            {
                // Spawning
                enemies.push_back(slime);
                enemies[i].setWorldPos(Vector2Add(Vector2Scale(randPos(), worldBorder.height - enemies[i].getTexHeight()), {190, 190}));
                enemies[i].setTarget(&player);
                // Stats
                if (gamemode == 1.f) enemies[i].setMaxHealth(3.f * (difficulty + 0.8));
                if (gamemode == 2.f) enemies[i].setMaxHealth(4.f * (difficulty + 0.8));
                if (gamemode == 3.f) enemies[i].setMaxHealth(5.f * (difficulty + 0.8));
                if (gamemode == 4.f) enemies[i].setMaxHealth(8.f * (difficulty + 0.8));
                enemies[i].setHealth(enemies[i].getMaxHealth());
                if (gamemode == 1.f) enemies[i].setDamage(1.6 * (difficulty + 0.8));
                if (gamemode == 2.f) enemies[i].setDamage(2.f * (difficulty + 0.8));
                if (gamemode == 3.f) enemies[i].setDamage(3.f * (difficulty + 0.8));
                if (gamemode == 4.f) enemies[i].setDamage(4.f * (difficulty + 0.8));
                if (gamemode == 1.f) enemies[i].setCritChance(0.f);
                if (gamemode == 2.f) enemies[i].setCritChance(1.f + (difficulty - 0.2)*15.f);
                if (gamemode == 3.f) enemies[i].setCritChance(2.f + (difficulty - 0.2)*30.f);
                if (gamemode == 4.f) enemies[i].setCritChance(3.f + (difficulty - 0.2)*60.f);
                if (gamemode == 1.f) enemies[i].setCritMult(0.f);
                if (gamemode == 2.f) enemies[i].setCritMult(1.1 * (difficulty + 0.8));
                if (gamemode == 3.f) enemies[i].setCritMult(1.3 * (difficulty + 0.8));
                if (gamemode == 4.f) enemies[i].setCritMult(1.6 * (difficulty + 0.8));
                enemies[i].setCooldown(40.f);
                enemies[i].setSpeed(2.6);
                enemies[i].setForce(1.1);
                enemies[i].setWeight(2.5);
                enemies[i].setReach(20.f);
                enemies[i].setRange(450.f);
                enemies[i].setScale(4.f);
                enemies[i].setAgro(false);
                enemies[i].setTint(WHITE);
                enemies[i].setTag("Slime");
            }
            for(int i = totalEnemies - totalSuperSlimes - totalNickelodeons - totalGobbies; i < totalEnemies - totalGobbies - totalNickelodeons; i++)  // Super Slimes
            {
                // Spawning
                enemies.push_back(superSlime);
                enemies[i].setWorldPos(Vector2Add(Vector2Scale(randPos(), worldBorder.height - enemies[i].getTexHeight()), {190, 190}));
                enemies[i].setTarget(&player);
                // Stats
                if (gamemode == 1.f) enemies[i].setMaxHealth(6.f * (difficulty + 0.8));
                if (gamemode == 2.f) enemies[i].setMaxHealth(8.f * (difficulty + 0.8));
                if (gamemode == 3.f) enemies[i].setMaxHealth(10.f * (difficulty + 0.8));
                if (gamemode == 4.f) enemies[i].setMaxHealth(16.f * (difficulty + 0.8));
                enemies[i].setHealth(enemies[i].getMaxHealth());
                if (gamemode == 1.f) enemies[i].setDamage(3.2 * (difficulty + 0.8));
                if (gamemode == 2.f) enemies[i].setDamage(4.f * (difficulty + 0.8));
                if (gamemode == 3.f) enemies[i].setDamage(6.f * (difficulty + 0.8));
                if (gamemode == 4.f) enemies[i].setDamage(8.f * (difficulty + 0.8));
                if (gamemode == 1.f) enemies[i].setCritChance(0.f);
                if (gamemode == 2.f) enemies[i].setCritChance(1.f + (difficulty - 0.2)*15.f);
                if (gamemode == 3.f) enemies[i].setCritChance(2.f + (difficulty - 0.2)*30.f);
                if (gamemode == 4.f) enemies[i].setCritChance(3.f + (difficulty - 0.2)*60.f);
                if (gamemode == 1.f) enemies[i].setCritMult(0.f);
                if (gamemode == 2.f) enemies[i].setCritMult(1.1 * (difficulty + 0.8));
                if (gamemode == 3.f) enemies[i].setCritMult(1.3 * (difficulty + 0.8));
                if (gamemode == 4.f) enemies[i].setCritMult(1.6 * (difficulty + 0.8));
                enemies[i].setCooldown(35.f);
                enemies[i].setSpeed(3.f);
                enemies[i].setForce(1.4);
                enemies[i].setWeight(3.f);
                enemies[i].setReach(20.f);
                enemies[i].setRange(450.f);
                enemies[i].setScale(4.f);
                enemies[i].setAgro(false);
                enemies[i].setTint(RED);
                enemies[i].setTag("Super Slime");
            }
            for(int i = totalEnemies - totalGobbies - totalNickelodeons; i < totalEnemies - totalGobbies; i++)  // Nickelodeon the Slime
            {
                // Spawning
                enemies.push_back(nickelodeon);
                enemies[i].setWorldPos(Vector2Add(Vector2Scale(randPos(), worldBorder.height - enemies[i].getTexHeight()), {190, 190}));
                enemies[i].setTarget(&player);
                // Stats
                if (gamemode == 1.f) enemies[i].setMaxHealth(16.f);
                if (gamemode == 2.f) enemies[i].setMaxHealth(20.f);
                if (gamemode == 3.f) enemies[i].setMaxHealth(25.f);
                if (gamemode == 4.f) enemies[i].setMaxHealth(40.f);
                enemies[i].setHealth(enemies[i].getMaxHealth());
                if (gamemode == 1.f) enemies[i].setDamage(3.f);
                if (gamemode == 2.f) enemies[i].setDamage(4.f);
                if (gamemode == 3.f) enemies[i].setDamage(5.f);
                if (gamemode == 4.f) enemies[i].setDamage(8.f);
                if (gamemode == 1.f) enemies[i].setCritChance(0.f);
                if (gamemode == 2.f) enemies[i].setCritChance(1.f + (difficulty - 0.2)*15.f);
                if (gamemode == 3.f) enemies[i].setCritChance(2.f + (difficulty - 0.2)*30.f);
                if (gamemode == 4.f) enemies[i].setCritChance(3.f + (difficulty - 0.2)*60.f);
                if (gamemode == 1.f) enemies[i].setCritMult(0.f);
                if (gamemode == 2.f) enemies[i].setCritMult(1.1 * (difficulty + 0.8));
                if (gamemode == 3.f) enemies[i].setCritMult(1.3 * (difficulty + 0.8));
                if (gamemode == 4.f) enemies[i].setCritMult(1.6 * (difficulty + 0.8));
                enemies[i].setCooldown(35.f);
                enemies[i].setSpeed(2.5);
                enemies[i].setForce(3.f);
                enemies[i].setWeight(30.f);
                enemies[i].setReach(20.f);
                enemies[i].setRange(450.f);
                enemies[i].setScale(8.f);
                enemies[i].setAgro(false);
                enemies[i].setTint({ 0, 225, 255, 255 });
                enemies[i].setTag("Nickelodeon the Slime");
            }
            for(int i = totalEnemies - totalGobbies; i < totalEnemies; i++)  // Gobby the Goblin
            {
                // Spawning
                enemies.push_back(gobby);
                enemies[i].setWorldPos(Vector2Add(Vector2Scale(randPos(), worldBorder.height - enemies[i].getTexHeight()), {190, 190}));
                enemies[i].setTarget(&player);
                // Stats
                if (gamemode == 1.f) enemies[i].setMaxHealth(48.f);
                if (gamemode == 2.f) enemies[i].setMaxHealth(60.f);
                if (gamemode == 3.f) enemies[i].setMaxHealth(75.f);
                if (gamemode == 4.f) enemies[i].setMaxHealth(120.f);
                enemies[i].setHealth(enemies[i].getMaxHealth());
                if (gamemode == 1.f) enemies[i].setDamage(3.f);
                if (gamemode == 2.f) enemies[i].setDamage(5.f);
                if (gamemode == 3.f) enemies[i].setDamage(7.f);
                if (gamemode == 4.f) enemies[i].setDamage(10.f);
                if (gamemode == 1.f) enemies[i].setCritChance(0.f);
                if (gamemode == 2.f) enemies[i].setCritChance(0.f + (difficulty - 0.2)*15.f);
                if (gamemode == 3.f) enemies[i].setCritChance(1.f + (difficulty - 0.2)*25.f);
                if (gamemode == 4.f) enemies[i].setCritChance(2.f + (difficulty - 0.2)*50.f);
                if (gamemode == 1.f) enemies[i].setCritMult(0.f);
                if (gamemode == 2.f) enemies[i].setCritMult(1.f * (difficulty + 0.8));
                if (gamemode == 3.f) enemies[i].setCritMult(1.2 * (difficulty + 0.8));
                if (gamemode == 4.f) enemies[i].setCritMult(1.5 * (difficulty + 0.8));
                enemies[i].setCooldown(35.f);
                enemies[i].setSpeed(2.6);
                enemies[i].setForce(2.8);
                enemies[i].setWeight(30.f);
                enemies[i].setReach(25.f);
                enemies[i].setRange(500.f);
                enemies[i].setScale(7.f);
                enemies[i].setAgro(false);
                enemies[i].setTint(GREEN);
                enemies[i].setTag("Gobby the Goblin");
            }

            // Universal Multipliers
            for(int i = 0; i < static_cast<int>(enemies.size()); i++)
            {
                enemies[i].setSpeed(enemies[i].getSpeed() * (gamemode/10.f + 0.8));
                if (gamemode == 1.f) enemies[i].setForce(enemies[i].getForce() * 0.9);
                if (gamemode == 2.f) enemies[i].setForce(enemies[i].getForce() * 1.f);
                if (gamemode == 3.f) enemies[i].setForce(enemies[i].getForce() * 1.2);
                if (gamemode == 4.f) enemies[i].setForce(enemies[i].getForce() * 1.8);
                if (gamemode == 1.f) enemies[i].setWeight(enemies[i].getWeight() * 0.85);
                if (gamemode == 2.f) enemies[i].setWeight(enemies[i].getWeight() * 1.f);
                if (gamemode == 3.f) enemies[i].setWeight(enemies[i].getWeight() * 1.2);
                if (gamemode == 4.f) enemies[i].setWeight(enemies[i].getWeight() * 1.8);
            }
        }

        // Spawning Enemies
        if (totalEnemies > 0 && enemiesSpawned < totalEnemies)
        {
            if (spawnCooldown < 0.f)
            {
                if (totalBosses > 0 && enemiesSpawned == totalEnemies - totalBosses) // Spawning Boss
                {
                    while (CheckCollisionRecs(enemies[totalEnemies - 1].getCollisionRec(), spawnProt))   // Spawn protection (600p radius)
                    {
                        enemies[totalEnemies - 1].setWorldPos(Vector2Add(Vector2Scale(randPos(), worldBorder.height - enemies[totalEnemies - 1].getTexHeight()), {190, 190}));    // Reset random spawn until works
                    }
                    enemies[totalEnemies - 1].spawn();
                }
                else
                {
                    int choice = rand() % (enemies.size() - totalBosses); 
                    while (enemies[choice].isSpawned()) choice = rand() % (enemies.size() - totalBosses);
                    while (CheckCollisionRecs(enemies[choice].getCollisionRec(), spawnProt))   // Spawn protection (600p radius)
                    {
                        enemies[choice].setWorldPos(Vector2Add(Vector2Scale(randPos(), worldBorder.height - enemies[choice].getTexHeight()), {190, 190}));    // Reset random spawn until works
                    }
                    enemies[choice].spawn();    // Spawning enemies
                }
                enemiesSpawned++;
                spawnCooldown = (float)(rand() % (int)(40.f / spawnRate)) + 40.f / spawnRate;   // Randomness to cooldown
            }
            else
            {
                spawnCooldown--;
            }
        }

        // Enemy Tick
        for(int i = 0; i < static_cast<int>(enemies.size()); i++)
        {
            enemies[i].tick(GetFrameTime());

            //  Item Drops
            if (!enemies[i].getAlive() && !enemies[i].getDropped())
            {
                int loot = 0;
                if (enemies[i].getTag() == "Goblin")
                {
                    loot = 1.f;
                }
                if (enemies[i].getTag() == "Super Goblin")
                {
                    loot = 3.f;
                }
                if (enemies[i].getTag() == "Goblin Bodyguard")
                {
                    loot = 5.f;
                }
                if (enemies[i].getTag() == "Godblin")
                {
                    loot = 25.f;
                }
                if (enemies[i].getTag() == "Slime")
                {
                    loot = 2.f;
                }
                if (enemies[i].getTag() == "Super Slime")
                {
                    loot = 5.f;
                }
                if (enemies[i].getTag() == "Nickelodeon the Slime")
                {
                    loot = 9.f;
                }
                if (enemies[i].getTag() == "Gobby the Goblin")
                {
                    loot = 25.f;
                }

                loot = sqrt(2) * loot / sqrt(gamemode);

                for (int e = 0; e < loot; e++)  // Coins
                {
                    if (rand() % 100 < 60 * sqrt(loot))
                    {
                        items.push_back(gold);
                        items[items.size() - 1].setWorldPos({enemies[i].getWorldPos().x + enemies[i].getTexWidth()/2, enemies[i].getWorldPos().y + enemies[i].getTexHeight()/2});
                    }
                }

                if (rand() % 100 < 1 * (1.f + log10(loot)) && enemies[i].getTag() != "Goblin" && enemies[i].getTag() != "Slime")    // Fire Bomb
                {
                    items.push_back(fireBomb);
                    items[items.size() - 1].setWorldPos(enemies[i].getWorldPos());
                }
                else if (rand() % 100 < 1 * (1.f + log10(loot)) && enemies[i].getTag() != "Goblin" && enemies[i].getTag() != "Slime")    // Insta Kill
                {
                    items.push_back(instaKill);
                    items[items.size() - 1].setWorldPos(enemies[i].getWorldPos());
                }
                else if (rand() % 100 < 2 * (1.f + log10(loot)) && enemies[i].getTag() != "Goblin")    // Super Hearts
                {
                    items.push_back(superHeart);
                    items[items.size() - 1].setWorldPos(enemies[i].getWorldPos());
                }
                else if (rand() % 100 < 18 * (1.f + log10(loot)) && enemies[i].getTag() != "Goblin")    // Hearts
                {
                    items.push_back(heart);
                    items[items.size() - 1].setWorldPos(enemies[i].getWorldPos());
                }

                enemies[i].setDropped(true);
            }
        }

        // Player XP GUI
        DrawRectangle(29.f, 61.f, player.getXP() / player.getXPreq() * 20.f * player.getMaxHealth() + 2.f * (player.getXP()/player.getXP()), 15.f, BLACK);
        DrawRectangle(30.f, 61.f, player.getXP() / player.getXPreq() * 20.f * player.getMaxHealth(), 14.f, BLUE);
        DrawText(std::to_string((int)(player.getLevel())).c_str(), 12.f, 32.f, 30, {0, 0, 255, 255});   // Level
        DrawText(std::to_string((int)(player.getGold())).c_str(), 50.f, 1.f, 30, GOLD);                 // Gold

        // Player Health GUI
        DrawRectangle(29.f, 29.f, 20.f * player.getMaxHealth() + 2.f, 32.f, BLACK);
        DrawRectangle(30.f, 30.f, 20.f * player.getMaxHealth(), 30.f, RED);
        DrawRectangle(30.f, 30.f, 20.f * player.getHealth(), 30.f, GREEN);

        // Displaying Wave Number
        std::string waveText = "Wave: ";
        waveText.append(std::to_string(wave), 0, 5);
        DrawText(waveText.c_str(), winWidth - 200.f, 150.f, 40, WHITE);
        if ((wave + 3) % 6 == 0)
        {
            DrawText("Boss Wave", winWidth - 357.f, 90.f, 40, {195, 8, 2, 255});
        }
        else if (wave % 6 == 0 && wave != 0)
        {
            DrawText("Challenge Wave", winWidth - 357.f, 90.f, 40, {195, 8, 2, 255});
        }
        else if (wave > 0)
        {
            DrawText("Normal Wave", winWidth - 357.f, 90.f, 40, WHITE);
        }

        // Displaying Difficulty
        if (gamemode == 1.f)
        {
            DrawText("Easy", winWidth - 200.f, 210.f, 40, {0, 225, 32, 255});
        }
        if (gamemode == 2.f)
        {
            DrawText("Normal", winWidth - 200.f, 210.f, 40, WHITE);
        }
        if (gamemode == 3.f)
        {
            DrawText("Hard", winWidth - 200.f, 210.f, 40, YELLOW);
        }
        if (gamemode == 4.f)
        {
            DrawText("Joaquin", winWidth - 200.f, 210.f, 40, {195, 8, 2, 255});
        }

        // Are Any Enemies Still Alive?
        enemiesDefeated = 0;
        for(int i = 0; i < static_cast<int>(enemies.size()); i++)
        {
            if (!enemies[i].getAlive()) {enemiesDefeated++;}
        }

        // Endings
        if (!player.getAlive()) // Character Dies
        {
            DrawText("GAME OVER!", winWidth/3.8, winHeight/2.5, 100, RED);
            if (IsKeyPressed(KEY_SPACE))    // Restart game
            {
                player.subtractGold(player.getGold());
                waveStarted = false;
                wave = 0;
                items.clear();
                for(int i = 0; i < static_cast<int>(enemies.size()); i++)
                {
                    enemies[i].setAlive(false);
                }
                player.setAlive(true);
                player.setHealth(player.getMaxHealth());
            }
        }
        else if (enemiesDefeated == totalEnemies)    // Enemies are dead; Next Wave
        {
            if (wave == 12)
            {
                DrawText("VICTORY!", winWidth/3.3, winHeight/2.5, 100, GREEN);
            }
            else if (gamemodeSelected)
            {
                waveActive = false;
                DrawText("NEXT WAVE", winWidth/2.6, winHeight/1.11, 50, WHITE);
                if (IsKeyPressed(KEY_SPACE))
                {
                    player.addGold(2.f * wave);
                    waveStarted = true;
                    waveActive = true;
                    wave++;
                }
            }
            else
            {
                DrawText("CHOOSE DIFFICULTY", 150.f, winHeight/2.6, 90, WHITE);
                DrawText("Easy: 1       Normal: 2       Hard: 3       Joaquin: 4", 320.f, winHeight/2.6 + 85.f, 27, WHITE);
                if (IsKeyPressed(KEY_ONE))
                {
                    gamemode = 1.f;
                    gamemodeSelected = true;
                }
                if (IsKeyPressed(KEY_TWO))
                {
                    gamemode = 2.f;
                    gamemodeSelected = true;
                }
                if (IsKeyPressed(KEY_THREE))
                {
                    gamemode = 3.f;
                    gamemodeSelected = true;
                }
                if (IsKeyPressed(KEY_FOUR))
                {
                    gamemode = 4.f;
                    gamemodeSelected = true;
                }
            }
        }
        else    // Character and Enemy Still Alive
        {
            enemiesLeft = enemiesSpawned - enemiesDefeated;
            std::string playerHealth = "Enemies Left: ";
            playerHealth.append(std::to_string(enemiesLeft), 0, 5);
            DrawText(playerHealth.c_str(), winWidth - 357.f, 30.f, 40, WHITE);
        }

        // Shop GUI
        if (!waveActive && gamemodeSelected && CheckCollisionRecs(props[0].getInteractiveRec(player.getWorldPos()), player.getCollisionRec()))
        {
            // Shop GUI Textures
            DrawTextureEx(shop, {0, 0}, 0.f, 4.f / 3.f, WHITE);
            DrawText(std::to_string((int)(player.getLevel())).c_str(), 145.f, 28.f, 32, {183, 183, 183, 255});
            DrawText(std::to_string((int)(player.getGold())).c_str(), 145.f, 68.f, 32, {183, 183, 183, 255});
            
            // Upgrade Ticks
            for (auto upgrade : swords)
            {
                upgrade.tick();
            }
            
            // Sword Upgrades
            for(int i = 0; i < swordCount; i++)
            {
                // Buying Upgrades
                if (swords[i].getBought())
                {
                    player.setSword(i);
                    if (i + 1 < swordCount) swords[i + 1].setUnknown(false);  // Unknowns
                }

                // Upgrade Stats
                if (!swords[i].getUnknown() && CheckCollisionPointRec(GetMousePosition(), swords[i].getCollisionRec()))
                {
                    DrawTextureEx(swordStats[i], {GetMousePosition().x + 5.f, GetMousePosition().y + 5.f}, 0.f, 0.9, WHITE);
                }

                // Is Purchasable
                if (player.getLevel() >= swords[i].getLevelReq() && player.getGold() >= swords[i].getGoldCost() && player.getSkillPoints() >= swords[i].getPointCost())
                {
                    swords[i].setPurchasable(true);
                }

                // Purchasing
                if (swords[i].getPurchasable() && CheckCollisionPointRec(GetMousePosition(), swords[i].getCollisionRec()) && IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                {
                    swords[i].setBought(true);
                    swords[i].setPurchasable(false);
                    player.subtractGold(swords[i].getGoldCost());
                    player.subtractSkillPoints(swords[i].getPointCost());
                }
            }
        }

        // Attacking Enemy
        if (player.getSwinging() && player.getLastHit() > player.getCooldown())
        {
            for(int i = 0; i < static_cast<int>(enemies.size()); i++)   // Splitting Damage
            {
                if (CheckCollisionRecs(enemies[i].getCollisionRec(), player.getWeaponCollisionRec()) && enemies[i].isSpawned() && enemies[i].getAlive())
                {
                    enemiesHit++;
                }
            }
            if (enemiesHit > 0)
            {
                for(int i = 0; i < static_cast<int>(enemies.size()); i++)   // Applying Damage
                {
                    if (CheckCollisionRecs(enemies[i].getCollisionRec(), player.getWeaponCollisionRec()) && enemies[i].isSpawned() && enemies[i].getAlive())
                    {
                        // Swing Attacks
                        if (player.getLastMove() == "Swing")
                        {
                            enemies[i].setKnockback(2.f);
                            (enemies[i].getTag() == "Nickelodeon the Slime" || enemies[i].getTag() == "Gobby the Goblin") ? damageDelt = player.getBaseDamage() : damageDelt = player.getDamage();  // Boss instakill immunity
                        }

                        // Stab Attacks
                        if (player.getLastMove() == "Stab")
                        {
                            enemies[i].setKnockback(3.f);
                            (enemies[i].getTag() == "Nickelodeon the Slime" || enemies[i].getTag() == "Gobby the Goblin") ? damageDelt = 0.75 * player.getBaseDamage() : damageDelt = 0.75 * player.getDamage();
                        }

                        // Knockback
                        enemies[i].setKnockVector(Vector2Add(Vector2Scale(enemies[i].getVelocity(), -1), player.getVelocity()));
                        enemies[i].setKnockback(enemies[i].getSpeed() * enemies[i].getKnockback() * player.getForce() / enemiesHit / (sqrt(enemies[i].getWeight()) / 2.f));
                        
                        // Critical Hits
                        if (rand() % 100 < player.getCritChance())
                        {
                            enemies[i].setKnockback(enemies[i].getKnockback() * (0.67 + player.getCritMult() / 3.f));
                            damageDelt *= player.getCritMult();
                        }

                         // Applying Damage + Markers
                        enemies[i].takeDamage(damageDelt/enemiesHit);
                        markerDelay = 60.f;
                        markerYMod = 0.f;
                        markerPos = {(player.getRightLeft() == -1.f) ? player.getWeaponCollisionRec().x : player.getWeaponCollisionRec().x + player.getWeaponCollisionRec().width, player.getWeaponCollisionRec().y};
                        dWorldPos = player.getWorldPos();
                    }
                }
                markerSplit = enemiesHit;
                enemiesHit = 0;
            }
            player.setLastHit(0.f);
        }

        // Drawing Hit Marker
        if (markerDelay > 0.f)
        {
            markerDelay--;
            markerYMod++;
            std::string hitMarker = std::to_string((int)(damageDelt/markerSplit)); 
            Vector2 markerPos2 = {markerPos.x - (player.getWorldPos().x - dWorldPos.x), markerPos.y - (player.getWorldPos().y - dWorldPos.y)};  // Maintaining marker's world pos
            if ((int)((damageDelt/markerSplit)*10.f) % 10 != 0)
            {
                hitMarker.append(".", 0, 5);
                hitMarker.append(std::to_string((int)((damageDelt/markerSplit - (int)(damageDelt/markerSplit))*10.f)), 0, 5);
            }
            if (damageDelt > player.getDamage())    // Critical Hit
            {
                DrawText("CRITICAL", markerPos2.x - 4.f, markerPos2.y - markerYMod - 25.f, 20, { 255, 55, 5, 255+(markerDelay*3.f-180.f) });
                DrawText(hitMarker.c_str(), markerPos2.x, markerPos2.y - markerYMod, 20, { 255, 55, 5, 255+(markerDelay*3.f-180.f) });
            }
            else
            {
                DrawText(hitMarker.c_str(), markerPos2.x, markerPos2.y - markerYMod, 20, { 255, 255, 255, 255+(markerDelay*3.f-180.f) });
            }
        }

        // End Program
        EndDrawing();
    }
    CloseWindow();
}