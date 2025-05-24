#include "raylib.h"

typedef struct hairryblueprint {
    Vector2 pos;
    int size;
    int health;
    int damage;
    int attackspeed;
} hairry;

void DrawHairry(hairry h) {
    DrawCircleV(h.pos, h.size, BLACK);
}

int main(void) {
    hairry firsthairry;
    firsthairry.pos.x = 600;
    firsthairry.pos.y = 300;
    firsthairry.size = 30;
    firsthairry.health = 15;
    firsthairry.damage = 3;
    firsthairry.attackspeed = 1;

    InitWindow(800, 600, "Dogeia");
    MaximizeWindow();
    SetTargetFPS(60);

    // initialize player pos
    Vector2 playerPos = {400, 300};

    while (!WindowShouldClose()) {
        // update player movement
        if (IsKeyDown(KEY_W)) playerPos.y -= 4;
        if (IsKeyDown(KEY_S)) playerPos.y += 4;
        if (IsKeyDown(KEY_A)) playerPos.x -= 4;
        if (IsKeyDown(KEY_D)) playerPos.x += 4;

        // Draw
        BeginDrawing();
        ClearBackground(DARKBLUE);

        DrawText("WASD to move the doge", 10, 10, 20, RAYWHITE);
        DrawCircleV(playerPos, 30, BEIGE); // THE DOGE
        DrawHairry(firsthairry);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}