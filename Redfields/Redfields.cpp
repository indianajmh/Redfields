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

// Window Dimensions
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
    Texture2D map = LoadTexture("textures/nature_tileset/OpenWorldMap24x24.png");   // map texture
    Vector2 mapPos{0.0, 0.0};                       // map position
    const float mapScale = 4.0f;                    // map texture scale
    Rectangle worldBorder{190, 190, 2690, 2680};    // map world border rectangle

    // GUI
    Texture2D shop = LoadTexture("textures/gui/Shop_Background.png");   // shop background texture

    // Initializing Player
    Character player{winWidth, winHeight};  // player instance
    player.setWorldPos({worldBorder.width/2 - winWidth/2, worldBorder.height/2 - winHeight/2}); // player world pos at spawn
    float spawnRad = 600.f;     // width of enemy spawn protection
    Rectangle spawnProt{winWidth/2 - spawnRad, winHeight/2 - spawnRad, spawnRad*2, spawnRad*2}; // enemy spawn protection rectangle

    // Initializing Upgrades
    vector<Upgrade> swords  // Sword upgrades
    {
        // Rusty Sword
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
    int swordCount = swords.size(); // number of swords
    vector<Texture2D> swordStats    // sword stat info textures
    {
        // Rusty Sword
        Texture2D{LoadTexture("textures/gui/rustysword_stats.png")},
        // Iron Sword
        Texture2D{LoadTexture("textures/gui/ironsword_stats.png")},
        // Steel Sword
        Texture2D{LoadTexture("textures/gui/steelsword_stats.png")},
        // Gilded Sword
        Texture2D{LoadTexture("textures/gui/gildedsword_stats.png")}
    };
    // Set rusty sword to be already purchased and known
    swords.at(0).setBought(true);
    swords.at(0).setUnknown(false);

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
    int totalBosses = 0;
    int totalEnemies = 0;
    int enemiesDefeated = totalEnemies; // enemies the player defeated this wave
    int enemiesLeft = 0;                // enemies still alive this wave
    int enemiesSpawned = 0;             // enemies spawned in this wave
    float enemiesHit = 0.f;             // enemies hit in swing
    float damageDelt = 0.f;             // damage delt to enemies
    bool waveStarted = false;           // if wave started yet
    bool waveActive = false;            // if wave is active
    bool gamemodeSelected = false;      // if gamemode has been selected yet
    int wave = 0;
    float spawnRate = 0.f;
    float difficulty = 0.f;
    int gamemode = 0;
    float spawnCooldown = 0.f;          // cooldown of enemy spawning

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
        1.3     // size
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
    float tick{};       // tick count (not used currently)
    float instaCool{};  // instakill cooldown

    // Program Loop
    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        BeginDrawing();                         // drawing window
        ClearBackground({99, 155, 255, 255});   // clearing window

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
            items[i].Render(player.getWorldPos());  // rendering items
            //  Collision
            if (CheckCollisionRecs(items[i].getCollisionRec(player.getWorldPos()), player.getCollisionRec()))   // if player touches item
            {
                if (items[i].getTag() == "Gold")    // if gold
                {
                    player.addGold(1);  // add gold
                }
                if (items[i].getTag() == "Heart")   // if heart
                {
                    player.setHealth(player.getHealth() + 1.f); // increase health by 1
                }
                if (items[i].getTag() == "Super Heart") // if super heart
                {
                    player.setHealth(player.getMaxHealth());    // set health to max
                }
                if (items[i].getTag() == "Insta Kill")  // if insta kill
                {
                    // Instakill cooldown based on gamemode
                    if (gamemode == 1) instaCool = 30.f * (1.f / GetFrameTime());
                    if (gamemode == 2) instaCool = 20.f * (1.f / GetFrameTime());
                    if (gamemode == 3) instaCool = 15.f * (1.f / GetFrameTime());
                    if (gamemode == 4) instaCool = 10.f * (1.f / GetFrameTime());
                }
                if (items[i].getTag() == "Fire Bomb")   // if fire bomb
                {
                    for(int i = 0; i < static_cast<int>(enemies.size()); i++)
                    {
                        // Kill every enemy except bosses
                        if (enemies[i].isSpawned() && enemies[i].getAlive() && !(enemies[i].getTag() == "Nickelodeon the Slime" || enemies[i].getTag() == "Gobby the Goblin"))
                        {
                            enemies[i].die();
                        }
                    }
                }
                items.erase(items.begin() + i); // remove item once taken
            }
        }
        for(int i = 0; i < static_cast<int>(enemies.size()); i++)    // Enemy Hit Cooldown (this can be moved to enemy.cpp/tick)
        {
            enemies[i].setLastHit(enemies[i].getLastHit() + 1);     // update enemy last hit tick
        }
        player.tick(GetFrameTime());    // update player tick

        // Insta Kill Timer
        if (instaCool > 0.f)
        {
            player.setDamage(999999.9); // "infinite" damage
            std::string instaText = "Insta Kill: "; // instakill text
            instaText.append(std::to_string((int)(instaCool/GetFrameTime())), 0, 5);    // intakill countdown
            DrawText(instaText.c_str(), winWidth - 300.f, winHeight - 50.f, 40, WHITE); // instakill countdown text
            instaCool--;    // tick instakill cooldown
        }
        else
        {
            player.setDamage(player.getBaseDamage());   // reset damage after instakill
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
            player.undoMovementX(); // undo x movement to prevent passing boundary
        }
        // Checking Map Y Boundaries
        else if (player.getWorldPos().y < worldBorder.y - winHeight/2 - player.getTexHeight() ||
            player.getWorldPos().y > worldBorder.y - winHeight/2 - player.getTexHeight() + worldBorder.height)
        {
            player.undoMovementY(); // undo y movement to prevent passing boundary
        }

        // Checking Prop Collision
        for (auto prop : props)
        {
            // Collision X
            if (CheckCollisionRecs(prop.getCollisionRec(player.getWorldPos()), player.getCollisionRec()))
            {
                player.undoMovementX(); // undo x movement to prevent passing boundary
            }
            // Collision Y
            if (CheckCollisionRecs(prop.getCollisionRec(player.getWorldPos()), player.getCollisionRec()))
            {
                player.redoMovementX(); // redo x movement to prevent passing boundary
                player.undoMovementY(); // undo y movement to prevent passing boundary
            }
            // Collision X + Y
            if (CheckCollisionRecs(prop.getCollisionRec(player.getWorldPos()), player.getCollisionRec()))
            {
                player.undoMovementX(); // undo x movement to prevent passing boundary
            }
        }

        // Starting Wave
        if (waveStarted)
        {
            // Initializing Wave Settings
            if (wave == 0)  // Wave 0
            {
                // Enemies
                totalGoblins = 0;
                totalSuperGoblins = 0;
                totalGoblinBodyguards = 0;
                totalGodblins = 0;
                totalSlimes = 0;
                totalSuperSlimes = 0;
                totalNickelodeons = 0;
                totalGobbies = 0;

                // Settings
                spawnRate = 0;
                difficulty = 0;
            }
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

            // Applying Gamemode Modifiers
            totalGoblins = (int)((totalGoblins)*(gamemode/5.f + 0.6));
            totalSuperGoblins = (int)((totalSuperGoblins)*(gamemode/5.f + 0.6));
            totalGoblinBodyguards = (int)((totalGoblinBodyguards)*(gamemode/5.f + 0.6));
            totalGodblins = (int)((totalGodblins)*(gamemode/5.f + 0.6));
            totalSlimes = (int)((totalSlimes)*(gamemode/5.f + 0.6));
            totalSuperSlimes = (int)((totalSuperSlimes)*(gamemode/5.f + 0.6));
            spawnRate *= gamemode/6.f + 0.8;

            // Initializing Other Variables
            totalBosses = totalNickelodeons + totalGobbies;
            totalEnemies = totalGoblins + totalSuperGoblins + totalGoblinBodyguards + totalGodblins + totalSlimes + totalSuperSlimes + totalBosses;
            enemiesDefeated = 0;
            enemiesSpawned = 0;
            enemiesLeft = totalEnemies;
            waveStarted = false;
            enemies.clear();    // delete enemies from array
            enemies.reserve(totalEnemies);  // reserve memory for enemy array at start of round
            
            // Initializing Vector Array for Wave and Different Enemy Types
            for(int i = 0; i < totalGoblins; i++)   // Goblins
            {
                // Spawning
                enemies.push_back(goblin);  // add enemy type to array
                enemies[i].setWorldPos(Vector2Add(Vector2Scale(randPos(), 
                    worldBorder.height - enemies[i].getTexHeight()), {190, 190}));  // set random spawnpoint
                enemies[i].setTarget(&player);  // target player
                // Stats
                if (gamemode == 1) enemies[i].setMaxHealth(2.f * (difficulty + 0.8));
                if (gamemode == 2) enemies[i].setMaxHealth(3.f * (difficulty + 0.8));
                if (gamemode == 3) enemies[i].setMaxHealth(4.f * (difficulty + 0.8));
                if (gamemode == 4) enemies[i].setMaxHealth(6.f * (difficulty + 0.8));
                enemies[i].setHealth(enemies[i].getMaxHealth());    // set health to max
                if (gamemode == 1) enemies[i].setDamage(0.8 * (difficulty + 0.8));
                if (gamemode == 2) enemies[i].setDamage(1.f * (difficulty + 0.8));
                if (gamemode == 3) enemies[i].setDamage(1.5 * (difficulty + 0.8));
                if (gamemode == 4) enemies[i].setDamage(2.f * (difficulty + 0.8));
                if (gamemode == 1) enemies[i].setCritChance(0.f);
                if (gamemode == 2) enemies[i].setCritChance(0.f + (difficulty - 0.2)*15.f);
                if (gamemode == 3) enemies[i].setCritChance(1.f + (difficulty - 0.2)*25.f);
                if (gamemode == 4) enemies[i].setCritChance(2.f + (difficulty - 0.2)*50.f);
                if (gamemode == 1) enemies[i].setCritMult(0.f);
                if (gamemode == 2) enemies[i].setCritMult(1.f * (difficulty + 0.8));
                if (gamemode == 3) enemies[i].setCritMult(1.2 * (difficulty + 0.8));
                if (gamemode == 4) enemies[i].setCritMult(1.5 * (difficulty + 0.8));
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
                if (gamemode == 1) enemies[i].setMaxHealth(4.f * (difficulty + 0.8));
                if (gamemode == 2) enemies[i].setMaxHealth(6.f * (difficulty + 0.8));
                if (gamemode == 3) enemies[i].setMaxHealth(8.f * (difficulty + 0.8));
                if (gamemode == 4) enemies[i].setMaxHealth(12.f * (difficulty + 0.8));
                enemies[i].setHealth(enemies[i].getMaxHealth());
                if (gamemode == 1) enemies[i].setDamage(1.6 * (difficulty + 0.8));
                if (gamemode == 2) enemies[i].setDamage(2.f * (difficulty + 0.8));
                if (gamemode == 3) enemies[i].setDamage(3.f * (difficulty + 0.8));
                if (gamemode == 4) enemies[i].setDamage(4.f * (difficulty + 0.8));
                if (gamemode == 1) enemies[i].setCritChance(0.f);
                if (gamemode == 2) enemies[i].setCritChance(0.f + (difficulty - 0.2)*15.f);
                if (gamemode == 3) enemies[i].setCritChance(1.f + (difficulty - 0.2)*25.f);
                if (gamemode == 4) enemies[i].setCritChance(2.f + (difficulty - 0.2)*50.f);
                if (gamemode == 1) enemies[i].setCritMult(0.f);
                if (gamemode == 2) enemies[i].setCritMult(1.f * (difficulty + 0.8));
                if (gamemode == 3) enemies[i].setCritMult(1.2 * (difficulty + 0.8));
                if (gamemode == 4) enemies[i].setCritMult(1.5 * (difficulty + 0.8));
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
                if (gamemode == 1) enemies[i].setMaxHealth(8.f * (difficulty + 0.8));
                if (gamemode == 2) enemies[i].setMaxHealth(12.f * (difficulty + 0.8));
                if (gamemode == 3) enemies[i].setMaxHealth(16.f * (difficulty + 0.8));
                if (gamemode == 4) enemies[i].setMaxHealth(24.f * (difficulty + 0.8));
                enemies[i].setHealth(enemies[i].getMaxHealth());
                if (gamemode == 1) enemies[i].setDamage(2.4 * (difficulty + 0.8));
                if (gamemode == 2) enemies[i].setDamage(3.f * (difficulty + 0.8));
                if (gamemode == 3) enemies[i].setDamage(4.5 * (difficulty + 0.8));
                if (gamemode == 4) enemies[i].setDamage(6.f * (difficulty + 0.8));
                if (gamemode == 1) enemies[i].setCritChance(0.f);
                if (gamemode == 2) enemies[i].setCritChance(0.f + (difficulty - 0.2)*15.f);
                if (gamemode == 3) enemies[i].setCritChance(1.f + (difficulty - 0.2)*25.f);
                if (gamemode == 4) enemies[i].setCritChance(2.f + (difficulty - 0.2)*50.f);
                if (gamemode == 1) enemies[i].setCritMult(0.f);
                if (gamemode == 2) enemies[i].setCritMult(1.f * (difficulty + 0.8));
                if (gamemode == 3) enemies[i].setCritMult(1.2 * (difficulty + 0.8));
                if (gamemode == 4) enemies[i].setCritMult(1.5 * (difficulty + 0.8));
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
                if (gamemode == 1) enemies[i].setMaxHealth(40.f * (difficulty + 0.8));
                if (gamemode == 2) enemies[i].setMaxHealth(60.f * (difficulty + 0.8));
                if (gamemode == 3) enemies[i].setMaxHealth(80.f * (difficulty + 0.8));
                if (gamemode == 4) enemies[i].setMaxHealth(120.f * (difficulty + 0.8));
                enemies[i].setHealth(enemies[i].getMaxHealth());
                if (gamemode == 1) enemies[i].setDamage(4.f * (difficulty + 0.8));
                if (gamemode == 2) enemies[i].setDamage(5.f * (difficulty + 0.8));
                if (gamemode == 3) enemies[i].setDamage(7.f * (difficulty + 0.8));
                if (gamemode == 4) enemies[i].setDamage(10.f * (difficulty + 0.8));
                if (gamemode == 1) enemies[i].setCritChance(0.f);
                if (gamemode == 2) enemies[i].setCritChance(0.f + (difficulty - 0.2)*15.f);
                if (gamemode == 3) enemies[i].setCritChance(1.f + (difficulty - 0.2)*25.f);
                if (gamemode == 4) enemies[i].setCritChance(2.f + (difficulty - 0.2)*50.f);
                if (gamemode == 1) enemies[i].setCritMult(0.f);
                if (gamemode == 2) enemies[i].setCritMult(1.f * (difficulty + 0.8));
                if (gamemode == 3) enemies[i].setCritMult(1.2 * (difficulty + 0.8));
                if (gamemode == 4) enemies[i].setCritMult(1.5 * (difficulty + 0.8));
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
                if (gamemode == 1) enemies[i].setMaxHealth(3.f * (difficulty + 0.8));
                if (gamemode == 2) enemies[i].setMaxHealth(4.f * (difficulty + 0.8));
                if (gamemode == 3) enemies[i].setMaxHealth(5.f * (difficulty + 0.8));
                if (gamemode == 4) enemies[i].setMaxHealth(8.f * (difficulty + 0.8));
                enemies[i].setHealth(enemies[i].getMaxHealth());
                if (gamemode == 1) enemies[i].setDamage(1.6 * (difficulty + 0.8));
                if (gamemode == 2) enemies[i].setDamage(2.f * (difficulty + 0.8));
                if (gamemode == 3) enemies[i].setDamage(3.f * (difficulty + 0.8));
                if (gamemode == 4) enemies[i].setDamage(4.f * (difficulty + 0.8));
                if (gamemode == 1) enemies[i].setCritChance(0.f);
                if (gamemode == 2) enemies[i].setCritChance(1.f + (difficulty - 0.2)*15.f);
                if (gamemode == 3) enemies[i].setCritChance(2.f + (difficulty - 0.2)*30.f);
                if (gamemode == 4) enemies[i].setCritChance(3.f + (difficulty - 0.2)*60.f);
                if (gamemode == 1) enemies[i].setCritMult(0.f);
                if (gamemode == 2) enemies[i].setCritMult(1.1 * (difficulty + 0.8));
                if (gamemode == 3) enemies[i].setCritMult(1.3 * (difficulty + 0.8));
                if (gamemode == 4) enemies[i].setCritMult(1.6 * (difficulty + 0.8));
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
                if (gamemode == 1) enemies[i].setMaxHealth(6.f * (difficulty + 0.8));
                if (gamemode == 2) enemies[i].setMaxHealth(8.f * (difficulty + 0.8));
                if (gamemode == 3) enemies[i].setMaxHealth(10.f * (difficulty + 0.8));
                if (gamemode == 4) enemies[i].setMaxHealth(16.f * (difficulty + 0.8));
                enemies[i].setHealth(enemies[i].getMaxHealth());
                if (gamemode == 1) enemies[i].setDamage(3.2 * (difficulty + 0.8));
                if (gamemode == 2) enemies[i].setDamage(4.f * (difficulty + 0.8));
                if (gamemode == 3) enemies[i].setDamage(6.f * (difficulty + 0.8));
                if (gamemode == 4) enemies[i].setDamage(8.f * (difficulty + 0.8));
                if (gamemode == 1) enemies[i].setCritChance(0.f);
                if (gamemode == 2) enemies[i].setCritChance(1.f + (difficulty - 0.2)*15.f);
                if (gamemode == 3) enemies[i].setCritChance(2.f + (difficulty - 0.2)*30.f);
                if (gamemode == 4) enemies[i].setCritChance(3.f + (difficulty - 0.2)*60.f);
                if (gamemode == 1) enemies[i].setCritMult(0.f);
                if (gamemode == 2) enemies[i].setCritMult(1.1 * (difficulty + 0.8));
                if (gamemode == 3) enemies[i].setCritMult(1.3 * (difficulty + 0.8));
                if (gamemode == 4) enemies[i].setCritMult(1.6 * (difficulty + 0.8));
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
                if (gamemode == 1) enemies[i].setMaxHealth(16.f);
                if (gamemode == 2) enemies[i].setMaxHealth(20.f);
                if (gamemode == 3) enemies[i].setMaxHealth(25.f);
                if (gamemode == 4) enemies[i].setMaxHealth(40.f);
                enemies[i].setHealth(enemies[i].getMaxHealth());
                if (gamemode == 1) enemies[i].setDamage(3.f);
                if (gamemode == 2) enemies[i].setDamage(4.f);
                if (gamemode == 3) enemies[i].setDamage(5.f);
                if (gamemode == 4) enemies[i].setDamage(8.f);
                if (gamemode == 1) enemies[i].setCritChance(0.f);
                if (gamemode == 2) enemies[i].setCritChance(1.f + (difficulty - 0.2)*15.f);
                if (gamemode == 3) enemies[i].setCritChance(2.f + (difficulty - 0.2)*30.f);
                if (gamemode == 4) enemies[i].setCritChance(3.f + (difficulty - 0.2)*60.f);
                if (gamemode == 1) enemies[i].setCritMult(0.f);
                if (gamemode == 2) enemies[i].setCritMult(1.1 * (difficulty + 0.8));
                if (gamemode == 3) enemies[i].setCritMult(1.3 * (difficulty + 0.8));
                if (gamemode == 4) enemies[i].setCritMult(1.6 * (difficulty + 0.8));
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
                if (gamemode == 1) enemies[i].setMaxHealth(48.f);
                if (gamemode == 2) enemies[i].setMaxHealth(60.f);
                if (gamemode == 3) enemies[i].setMaxHealth(75.f);
                if (gamemode == 4) enemies[i].setMaxHealth(120.f);
                enemies[i].setHealth(enemies[i].getMaxHealth());
                if (gamemode == 1) enemies[i].setDamage(3.f);
                if (gamemode == 2) enemies[i].setDamage(5.f);
                if (gamemode == 3) enemies[i].setDamage(7.f);
                if (gamemode == 4) enemies[i].setDamage(10.f);
                if (gamemode == 1) enemies[i].setCritChance(0.f);
                if (gamemode == 2) enemies[i].setCritChance(0.f + (difficulty - 0.2)*15.f);
                if (gamemode == 3) enemies[i].setCritChance(1.f + (difficulty - 0.2)*25.f);
                if (gamemode == 4) enemies[i].setCritChance(2.f + (difficulty - 0.2)*50.f);
                if (gamemode == 1) enemies[i].setCritMult(0.f);
                if (gamemode == 2) enemies[i].setCritMult(1.f * (difficulty + 0.8));
                if (gamemode == 3) enemies[i].setCritMult(1.2 * (difficulty + 0.8));
                if (gamemode == 4) enemies[i].setCritMult(1.5 * (difficulty + 0.8));
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

            // Universal Gamemode Modifiers
            for(int i = 0; i < static_cast<int>(enemies.size()); i++)
            {
                enemies[i].setSpeed(enemies[i].getSpeed() * (gamemode/10.f + 0.8)); // speed modifier
                // Attack knockback modifier
                if (gamemode == 1) enemies[i].setForce(enemies[i].getForce() * 0.9);
                if (gamemode == 2) enemies[i].setForce(enemies[i].getForce() * 1.f);
                if (gamemode == 3) enemies[i].setForce(enemies[i].getForce() * 1.2);
                if (gamemode == 4) enemies[i].setForce(enemies[i].getForce() * 1.8);
                // Knockback resistance modifier
                if (gamemode == 1) enemies[i].setWeight(enemies[i].getWeight() * 0.85);
                if (gamemode == 2) enemies[i].setWeight(enemies[i].getWeight() * 1.f);
                if (gamemode == 3) enemies[i].setWeight(enemies[i].getWeight() * 1.2);
                if (gamemode == 4) enemies[i].setWeight(enemies[i].getWeight() * 1.8);
            }
        }

        // Spawning Enemies
        if (totalEnemies > 0 && enemiesSpawned < totalEnemies)  // If there are still enemies left to spawn
        {
            if (spawnCooldown < 0.f)    // Spawn cooldown passes
            {
                if (totalBosses > 0 && enemiesSpawned == totalEnemies - totalBosses) // Spawning Boss
                {
                    while (CheckCollisionRecs(enemies[totalEnemies - 1].getCollisionRec(), spawnProt))   // Spawn protection (600p radius)
                    {
                        enemies[totalEnemies - 1].setWorldPos(Vector2Add(Vector2Scale(randPos(), 
                            worldBorder.height - enemies[totalEnemies - 1].getTexHeight()), {190, 190}));    // Reset random spawn until works
                    }
                    enemies[totalEnemies - 1].spawn();  // Spawn in enemy
                }
                else
                {
                    int choice = rand() % (enemies.size() - totalBosses);   // Selecting random enemy to spawn
                    while (enemies[choice].isSpawned()) choice = rand() % (enemies.size() - totalBosses);   // Select new enemy
                    while (CheckCollisionRecs(enemies[choice].getCollisionRec(), spawnProt))   // Spawn protection (600p radius)
                    {
                        enemies[choice].setWorldPos(Vector2Add(Vector2Scale(randPos(), 
                            worldBorder.height - enemies[choice].getTexHeight()), {190, 190}));    // Reset random spawn until works
                    }
                    enemies[choice].spawn();    // Spawning enemies
                }
                enemiesSpawned++;   // One more enemy is recorded to have spawned
                spawnCooldown = (float)(rand() % (int)(40.f / spawnRate)) + 40.f / spawnRate;   // Randomness to cooldown
            }
            else
            {
                spawnCooldown--;    // Ticking spawn cooldown
            }
        }

        // Enemy Tick
        for(int i = 0; i < static_cast<int>(enemies.size()); i++)
        {
            enemies[i].tick(GetFrameTime());    // Enemy tick

            //  Item Drops (maybe just change to a loot table system in v0.3.1.0 or later versions)
            if (!enemies[i].getAlive() && !enemies[i].getDropped()) // If enemy died and has not yet rolled loot table
            {
                double loot = 0;    // Loot value

                // Loot values based on enemy type
                if (enemies[i].getTag() == "Goblin") loot = 1;
                if (enemies[i].getTag() == "Super Goblin") loot = 3;
                if (enemies[i].getTag() == "Goblin Bodyguard") loot = 5;
                if (enemies[i].getTag() == "Godblin") loot = 25;
                if (enemies[i].getTag() == "Slime") loot = 2;
                if (enemies[i].getTag() == "Super Slime") loot = 5;
                if (enemies[i].getTag() == "Nickelodeon the Slime") loot = 9;
                if (enemies[i].getTag() == "Gobby the Goblin") loot = 25;

                loot = sqrt(2) * loot / sqrt(gamemode); // Gamemode modifier

                // Coins
                for (int e = 0; e < loot; e++)  // 1 coin for every loot value
                {
                    // *Consider changing this to just 60% (or something else), then higher loot values give more rolls for possible coin(s)
                    if (rand() % 100 < 60 * sqrt(loot))     // 60% chance multiplied by sqrt of loot value
                    {
                        items.push_back(gold);  // Add gold to array
                        items[items.size() - 1].setWorldPos({enemies[i].getWorldPos().x + enemies[i].getTexWidth()/2, 
                            enemies[i].getWorldPos().y + enemies[i].getTexHeight()/2});     // Spawning in item
                    }
                }

                // Other items:
                // Drop chances dependent on loot value and are only dropped by certain enemies

                // Fire Bomb
                if (rand() % 100 < 1 * (1.f + log10(loot)) && enemies[i].getTag() != "Goblin" && enemies[i].getTag() != "Slime")    
                { 
                    items.push_back(fireBomb);  // Add item to array
                    items[items.size() - 1].setWorldPos(enemies[i].getWorldPos());  // Spawning in item
                }
                else if (rand() % 100 < 1 * (1.f + log10(loot)) && enemies[i].getTag() != "Goblin" && enemies[i].getTag() != "Slime")
                // Insta Kill
                {
                    items.push_back(instaKill);
                    items[items.size() - 1].setWorldPos(enemies[i].getWorldPos());
                }
                else if (rand() % 100 < 2 * (1.f + log10(loot)) && enemies[i].getTag() != "Goblin")    
                // Super Hearts
                {
                    items.push_back(superHeart);
                    items[items.size() - 1].setWorldPos(enemies[i].getWorldPos());
                }
                else if (rand() % 100 < 18 * (1.f + log10(loot)) && enemies[i].getTag() != "Goblin")    
                // Hearts
                {
                    items.push_back(heart);
                    items[items.size() - 1].setWorldPos(enemies[i].getWorldPos());
                }

                enemies[i].setDropped(true);    // Loot table of this enemy has been rolled
            }
        }

        player.displayHP();

        // Displaying Wave Number
        std::string waveText = "Wave: ";
        waveText.append(std::to_string(wave), 0, 5); // wave number
        DrawText(waveText.c_str(), winWidth - 200.f, 150.f, 40, WHITE); // drawing text
        if ((wave + 3) % 6 == 0)    // Boss wave
        {
            DrawText("Boss Wave", winWidth - 357.f, 90.f, 40, {195, 8, 2, 255});
        }
        else if (wave % 6 == 0 && wave != 0)    // Challenge wave
        {
            DrawText("Challenge Wave", winWidth - 357.f, 90.f, 40, {195, 8, 2, 255});
        }
        else if (wave > 0)  // Normal wave
        {
            DrawText("Normal Wave", winWidth - 357.f, 90.f, 40, WHITE);
        }

        // Displaying Difficulty
        if (gamemode == 1)  // Easy
        {
            DrawText("Easy", winWidth - 200.f, 210.f, 40, {0, 225, 32, 255});
        }
        if (gamemode == 2)  // Normal
        {
            DrawText("Normal", winWidth - 200.f, 210.f, 40, WHITE);
        }
        if (gamemode == 3)  // Hard
        {
            DrawText("Hard", winWidth - 200.f, 210.f, 40, YELLOW);
        }
        if (gamemode == 4)  // Joaquin
        {
            DrawText("Joaquin", winWidth - 200.f, 210.f, 40, {195, 8, 2, 255});
        }

        // Are Any Enemies Still Alive?
        enemiesDefeated = 0;
        for(int i = 0; i < static_cast<int>(enemies.size()); i++)
        {
            if (!enemies[i].getAlive()) {enemiesDefeated++;} // counts the number of enemies defeated
        }

        // Endings
        if (!player.getAlive()) // Character Dies
        {
            DrawText("GAME OVER!", winWidth/3.8, winHeight/2.5, 100, RED);
            if (IsKeyPressed(KEY_SPACE))    // Restart game by pressing space and clearing stats
            {
                player.subtractGold(player.getGold());
                waveStarted = false;
                wave = 0;
                enemies.clear();
                items.clear();
                player.setAlive(true);
                player.setHealth(player.getMaxHealth());
                // player spawns back in center of map
                player.setWorldPos({worldBorder.width/2 - winWidth/2, worldBorder.height/2 - winHeight/2});
                enemiesDefeated = 0;
                totalEnemies = 0;
            }
        }
        else if (enemiesDefeated == totalEnemies)    // Enemies are dead; Next Wave
        {
            if (wave == 12) // If won
            {
                DrawText("VICTORY!", winWidth/3.3, winHeight/2.5, 100, GREEN);
            }
            else if (gamemodeSelected)  // Next wave
            {
                waveActive = false; // pause waves
                DrawText("NEXT WAVE", winWidth/2.6, winHeight/1.11, 50, WHITE);
                // Starts next wave
                if (IsKeyPressed(KEY_SPACE))
                {
                    player.addGold(2 * wave);   // Increasing gold reward per wave
                    waveStarted = true;
                    waveActive = true;
                    wave++;
                }
            }
            else    // If just started game
            {
                // Difficulty Options
                DrawText("CHOOSE DIFFICULTY", 150.f, winHeight/2.6, 90, WHITE);
                DrawText("Easy: 1       Normal: 2       Hard: 3       Joaquin: 4", 320.f, winHeight/2.6 + 85.f, 27, WHITE);
                if (IsKeyPressed(KEY_ONE))      // Easy
                {
                    gamemode = 1;
                    gamemodeSelected = true;
                }
                if (IsKeyPressed(KEY_TWO))      // Normal
                {
                    gamemode = 2;
                    gamemodeSelected = true;
                }
                if (IsKeyPressed(KEY_THREE))    // Hard
                {
                    gamemode = 3;
                    gamemodeSelected = true;
                }
                if (IsKeyPressed(KEY_FOUR))     // Joaquin
                {
                    gamemode = 4;
                    gamemodeSelected = true;
                }
            }
        }
        else    // Character and Enemy Still Alive
        {
            enemiesLeft = enemiesSpawned - enemiesDefeated; // Enemies left counter
            std::string enemiesLeftStr = "Enemies Left: ";
            enemiesLeftStr.append(std::to_string(enemiesLeft), 0, 5); // Displaying said counter
            DrawText(enemiesLeftStr.c_str(), winWidth - 357.f, 30.f, 40, WHITE);
        }

        // Shop GUI
        if (!waveActive && gamemodeSelected && CheckCollisionRecs(props[0].getInteractiveRec(player.getWorldPos()), 
                player.getCollisionRec()))
        {
            // Shop GUI Textures
            DrawTextureEx(shop, {0, 0}, 0.f, 4.f / 3.f, WHITE); // background
            DrawText(std::to_string(player.getLevel()).c_str(), 145.f, 28.f, 32, {183, 183, 183, 255}); // display level
            DrawText(std::to_string(player.getGold()).c_str(), 145.f, 68.f, 32, {183, 183, 183, 255});  // display gold
            
            // Upgrade Ticks
            for (auto upgrade : swords)
            {
                upgrade.tick();
            }
            
            // Sword Upgrades
            for(int i = 0; i < swordCount; i++)
            {
                // Buying Upgrades
                if (swords.at(i).getBought())   // if bought
                {
                    player.setSword(i); // give sword
                    if (i + 1 < swordCount) swords.at(i + 1).setUnknown(false);  // Reveal unknowns
                }

                // Upgrade Stats
                if (!swords.at(i).getUnknown() && CheckCollisionPointRec(GetMousePosition(), swords.at(i).getCollisionRec()))
                {
                    // Only if we can see the upgrade (i.e. not unknown)
                    DrawTextureEx(swordStats.at(i), {GetMousePosition().x + 5.f, GetMousePosition().y + 5.f}, 0.f, 0.9, WHITE);
                }

                // Is Purchasable
                if (!swords.at(i).getBought() && player.getLevel() >= swords.at(i).getLevelReq() && player.getGold() >= swords.at(i).getGoldCost() && 
                        player.getSkillPoints() >= swords.at(i).getPointCost())
                {
                    // If requirements are met, sword can be purchased
                    swords.at(i).setPurchasable(true);
                }

                // Purchasing
                if (swords.at(i).getPurchasable() && CheckCollisionPointRec(GetMousePosition(), swords.at(i).getCollisionRec()) && 
                        IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                {
                    swords.at(i).setBought(true);
                    swords.at(i).setPurchasable(false);
                    player.subtractGold(swords.at(i).getGoldCost());
                    player.subtractSkillPoints(swords.at(i).getPointCost());
                }
            }
        }

        // Attacking Enemy
        if (player.isSwinging() && player.getLastHit() > player.getCooldown())
        {
            for(int i = 0; i < static_cast<int>(enemies.size()); i++)   // Splitting Damage
            {
                if (CheckCollisionRecs(enemies[i].getCollisionRec(), player.getWeaponCollisionRec()) && enemies[i].isSpawned() && 
                        enemies[i].getAlive())
                {
                    enemiesHit++;   // counting number of enemies hit
                }
            }
            if (enemiesHit > 0)
            {
                for(int i = 0; i < static_cast<int>(enemies.size()); i++)   // Applying Damage
                {
                    if (CheckCollisionRecs(enemies[i].getCollisionRec(), player.getWeaponCollisionRec()) && enemies[i].isSpawned() && 
                            enemies[i].getAlive()) // if collision + enemies are alive
                    {
                        // Swing Attacks
                        if (player.getLastMove() == "Swing")
                        {
                            enemies[i].setKnockback(2.f);   // applying knockback
                            (enemies[i].getTag() == "Nickelodeon the Slime" || enemies[i].getTag() == "Gobby the Goblin") ? 
                                damageDelt = player.getBaseDamage() : damageDelt = player.getDamage();  // Boss instakill immunity
                        }

                        // Stab Attacks
                        if (player.getLastMove() == "Stab")
                        {
                            enemies[i].setKnockback(3.f);   // applying knockback
                            (enemies[i].getTag() == "Nickelodeon the Slime" || enemies[i].getTag() == "Gobby the Goblin") ? 
                                damageDelt = 0.75 * player.getBaseDamage() : damageDelt = 0.75 * player.getDamage(); // Boss instakill immunity
                        }

                        // Knockback vector
                        enemies[i].setKnockVector(Vector2Add(Vector2Scale(enemies[i].getVelocity(), -1), player.getVelocity()));
                        // Knockback modifier
                        enemies[i].setKnockback(enemies[i].getSpeed() * enemies[i].getKnockback() * player.getForce() / enemiesHit / 
                                (sqrt(enemies[i].getWeight()) / 2.f));
                        
                        // Critical Hits
                        if (rand() % 100 < player.getCritChance())  // percent chance
                        {
                            // Critical hit effect on knockback
                            enemies[i].setKnockback(enemies[i].getKnockback() * (0.67 + player.getCritMult() / 3.f));
                            // Critical hit effect on damage
                            damageDelt *= player.getCritMult();
                        }

                        // Applying Damage
                        enemies[i].takeDamage(damageDelt/enemiesHit);

                        // Damage Indicators
                        markerDelay = 60.f;
                        markerYMod = 0.f;
                        markerPos = {(player.getRightLeft() == -1.f) ? player.getWeaponCollisionRec().x : player.getWeaponCollisionRec().x + 
                                player.getWeaponCollisionRec().width, player.getWeaponCollisionRec().y};
                        dWorldPos = player.getWorldPos();
                    }
                }
                markerSplit = enemiesHit;
                enemiesHit = 0;
            }
            player.setLastHit(0.f);
        }

        // Drawing Damage Indicators
        if (markerDelay > 0.f)
        {
            markerDelay--;
            markerYMod++;
            std::string hitMarker = std::to_string((int)(damageDelt/markerSplit)); 
            Vector2 markerPos2 = {markerPos.x - (player.getWorldPos().x - dWorldPos.x), markerPos.y - 
                    (player.getWorldPos().y - dWorldPos.y)};  // Maintaining marker's world pos
            if ((int)((damageDelt/markerSplit)*10.f) % 10 != 0) // Adding one decimal place
            {
                hitMarker.append(".", 0, 5);
                hitMarker.append(std::to_string((int)((damageDelt/markerSplit - (int)(damageDelt/markerSplit))*10.f)), 0, 5);
            }
            if (damageDelt > player.getDamage())    // Critical Hit
            {
                DrawText("CRITICAL", markerPos2.x - 4.f, markerPos2.y - markerYMod - 25.f, 20, { 255, 55, 5, 255+(markerDelay*3.f-180.f) });
                DrawText(hitMarker.c_str(), markerPos2.x, markerPos2.y - markerYMod, 20, { 255, 55, 5, 255+(markerDelay*3.f-180.f) });
            }
            else    // Normal hit
            {
                DrawText(hitMarker.c_str(), markerPos2.x, markerPos2.y - markerYMod, 20, { 255, 255, 255, 255+(markerDelay*3.f-180.f) });
            }
        }

        // End Program
        EndDrawing();
    }
    CloseWindow();
}