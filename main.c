#include "raylib.h"

typedef enum {
    GROUND,
    ENTITY_ENEMY,
    HAZARD_SPIKE
} EntityType;

typedef struct hairryblueprint {
    Vector2 pos;
    int size;
    int health;
    int damage;
    int attackspeed;
    EntityType type;
} hairry;

typedef struct playerstuff {
    Vector2 pos;
    int size;
    int health;
    int damage;
    int attackspeed;
    int stompdamage;
} player;

typedef struct spike {
    Vector2 pos;
    Vector2 size;
    Texture2D texture;
    EntityType type;
} spike;

void DrawHairry(hairry h) {
    DrawCircleV(h.pos, h.size, BLACK);
}

void DrawPlayer(player p) {
    DrawCircleV(p.pos, p.size, BEIGE);
}

void DrawSpike(spike rect) {
    Rectangle source = { 0, 0, rect.texture.width, rect.texture.height };
    Rectangle dest = { rect.pos.x, rect.pos.y, rect.size.x, rect.size.y };
    Vector2 origin = { 0, 0 };
    DrawTexturePro(rect.texture, source, dest, origin, 0.0f, WHITE);
}

int main(void) {
    hairry firsthairry;
    firsthairry.pos.x = 600;
    firsthairry.pos.y = 300;
    firsthairry.size = 30;
    firsthairry.health = 15;
    firsthairry.damage = 3;
    firsthairry.attackspeed = 1;
    firsthairry.type = ENTITY_ENEMY;

    player plr;
    plr.pos.x = 400;
    plr.pos.y = 300;
    plr.health = 100;
    plr.size = 30;
    plr.damage = 5;
    plr.stompdamage = 10;
    plr.attackspeed = 0.5;

    InitWindow(800, 600, "Dogeia");
    MaximizeWindow();
    SetTargetFPS(60);

    spike spik;
    spik.pos.x = 600;
    spik.pos.y = 500;
    spik.size.x = 80;
    spik.size.y = 80;
    spik.type = HAZARD_SPIKE;
    spik.texture = LoadTexture("Spikeunf.png");

    // initialize player pos
    Vector2 playerPos = {400, 300};

    while (!WindowShouldClose()) {
        // update player movement
        if (IsKeyDown(KEY_W)) plr.pos.y -= 4;
        if (IsKeyDown(KEY_S)) plr.pos.y += 4;
        if (IsKeyDown(KEY_A)) plr.pos.x -= 4;
        if (IsKeyDown(KEY_D)) plr.pos.x += 4;

        // Draw
        BeginDrawing();
        ClearBackground(DARKBLUE);

        DrawText("WASD to move the doge", 10, 10, 20, RAYWHITE);
        DrawPlayer(plr);
        DrawHairry(firsthairry);
        DrawSpike(spik);

        EndDrawing();
    }

    UnloadTexture(spik.texture);
    CloseWindow();
    return 0;
}