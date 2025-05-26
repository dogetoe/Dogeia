#include "raylib.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#define GAME_WIDTH  1920
#define GAME_HEIGHT 1080

// a million different structs and functions here lol

// entity types
typedef enum {
    GROUND,
    ENTITY_ENEMY_HAIRRY,
    HAZARD_SPIKE
} EntityType;

// "blueprint" for hairry enemy
typedef struct hairryblueprint {
    Vector2 pos;
    int size;
    int health;
    int damage;
    int attackspeed;
    EntityType type;
} hairry;

// player struct
typedef struct playerstuff {
    Vector2 pos;
    Vector2 size;
    int health;
    int damage;
    int attackspeed;
    int stompdamage;
    bool isfalling;
    bool iswalking;
    float vy;
    float vx;
    Rectangle hitbox;
    Texture2D texture;
} player;

// spike struct
typedef struct spike {
    Vector2 pos;
    Vector2 size;
    int id;
    Rectangle hitbox;
    Texture2D* texture;
    EntityType type;
} spike;

void DrawHairry(hairry h) {
    DrawCircleV(h.pos, h.size, BLACK);
}

void DrawPlayer(player rect) {
    Rectangle source = { 0, 0, rect.texture.width, rect.texture.height };
    Rectangle dest = { rect.pos.x, rect.pos.y, rect.size.x, rect.size.y };
    Vector2 origin = { 0, 0 };
    DrawTexturePro(rect.texture, source, dest, origin, 0.0f, WHITE);
}

void DrawSpike(spike rect) {
    Rectangle source = { 0, 0, rect.texture->width, rect.texture->height };
    Rectangle dest = { rect.pos.x, rect.pos.y, rect.size.x, rect.size.y };
    Vector2 origin = { 0, 0 };
    DrawTexturePro(*rect.texture, source, dest, origin, 0.0f, WHITE);
}

spike* CloneSpike(const spike* blueprint, int new_id) {
    return &(spike){
        .id = new_id,
        .texture = blueprint->texture,
        .pos = blueprint->pos,
        .hitbox = (Rectangle){
            blueprint->pos.x,
            blueprint->pos.y,
            blueprint->hitbox.width,
            blueprint->hitbox.height
        }
    };
}

spike* NewSpike(const spike* blueprint, int id) {
    spike* s = malloc(sizeof(spike));
    if (!s) return NULL;
    *s = *CloneSpike(blueprint, id);  // copy the clean temp struct
    return s;
}

spike* spikeList[40000]; // array of spike pointers
int spikeCount = 0;

int main(void) {
    hairry firsthairry;
    firsthairry.pos.x = 600;
    firsthairry.pos.y = 300;
    firsthairry.size = 30;
    firsthairry.health = 15;
    firsthairry.damage = 3;
    firsthairry.attackspeed = 1;
    firsthairry.type = ENTITY_ENEMY_HAIRRY;

    InitWindow(GAME_WIDTH, GAME_HEIGHT, "Dogeia");
    MaximizeWindow();
    SetTargetFPS(60);

    RenderTexture2D target = LoadRenderTexture(GAME_WIDTH, GAME_HEIGHT);
    SetTextureFilter(target.texture, TEXTURE_FILTER_POINT);


    spike spik;
    spik.pos.x = 0;
    spik.pos.y = 1000;
    spik.size.x = GAME_WIDTH;
    spik.size.y = 80;
    spik.hitbox = (Rectangle){spik.pos.x, spik.pos.y, spik.size.x, spik.size.y};
    spik.type = HAZARD_SPIKE;
    Texture2D spikeTex = LoadTexture("Spikeunf.png");
    spik.texture = &spikeTex;

    player plr;
    plr.pos.x = 400;
    plr.pos.y = 100;
    plr.health = 100;
    plr.size.x = 60;
    plr.size.y = 60;
    plr.damage = 5;
    plr.stompdamage = 10;
    plr.attackspeed = 0.5;
    plr.isfalling = false;
    plr.iswalking = false;
    plr.vy = 0;
    plr.vx = 0;
    plr.hitbox = (Rectangle){ plr.pos.x, plr.pos.y, plr.size.x, plr.size.y };
    plr.texture = LoadTexture("SirAdamDogeingknight.png");

    while (!WindowShouldClose()) {
        // update player movement
        if (IsKeyDown(KEY_A)) {
            plr.vx = -1;
            plr.pos.x -= 4;
            plr.hitbox.x = plr.pos.x;
            plr.hitbox.y = plr.pos.y;

        }
        if (IsKeyDown(KEY_D)) {
            plr.vx = 1;
            plr.pos.x += 4;
            plr.hitbox.x = plr.pos.x;
            plr.hitbox.y = plr.pos.y;

        }
        if (IsKeyUp(KEY_A) && IsKeyUp(KEY_D)) {
            plr.vx = 0;
            plr.hitbox.x = plr.pos.x;
            plr.hitbox.y = plr.pos.y;
        }
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && spikeCount < 40000) {
            Vector2 pos = GetMousePosition();
            spike* newSpike = NewSpike(&spik, spikeCount);
            
            // Customize new spike's position & size
            newSpike->pos = pos;
            newSpike->size = (Vector2){ 60, 60 };
            newSpike->hitbox = (Rectangle){ pos.x, pos.y, newSpike->size.x, newSpike->size.y };
            newSpike->texture = &spikeTex;
        
            spikeList[spikeCount++] = newSpike;
        }

        // Update hitbox after horizontal move
        plr.hitbox.x = plr.pos.x;
        plr.hitbox.y = plr.pos.y;

        // Horizontal collision with spike
        for (int i = 0; i < spikeCount; i++) {
            if (CheckCollisionRecs(plr.hitbox, spikeList[i]->hitbox)) {
                if (plr.vx > 0) {
                plr.pos.x = spikeList[i]->hitbox.x - plr.size.x;
                } else if (plr.vx < 0) {
                plr.pos.x = spikeList[i]->hitbox.x + spikeList[i]->hitbox.width;
                }
                plr.vx = 0;
                plr.hitbox.x = plr.pos.x;
            }
        }

        // ---- Vertical movement & collision
        plr.vy += 0.09f; // gravity
        plr.pos.y += plr.vy;

        // Update hitbox again after vertical move
        plr.hitbox.x = plr.pos.x;
        plr.hitbox.y = plr.pos.y;

        // Vertical collision with spike
        for (int i = 0; i < spikeCount; i++) {
            if (CheckCollisionRecs(plr.hitbox, spikeList[i]->hitbox)) {
                if (plr.vy > 0) {
                // Falling down
                plr.pos.y = spikeList[i]->hitbox.y - plr.size.y;
                plr.isfalling = false;
            } else if (plr.vy < 0) {
                // Jumping up into spike
                plr.pos.y = spikeList[i]->hitbox.y + spikeList[i]->hitbox.height;
            }
            plr.vy = 0;
            plr.hitbox.y = plr.pos.y;
            }
        }

        // Draw
        BeginDrawing();

        // actually begin drawing with good resolution
        BeginTextureMode(target);
        ClearBackground(DARKBLUE);

        DrawText("WASD to move the doge", 10, 10, 20, RAYWHITE);
        DrawPlayer(plr);
        DrawHairry(firsthairry);
        DrawSpike(spik);
        for (int i = 0; i < spikeCount; i++) {
            DrawSpike(*spikeList[i]);
        }

        EndTextureMode();

        // Calculate scaling
        float screenRatio = (float)GetScreenWidth() / (float)GetScreenHeight();
        float gameRatio   = (float)GAME_WIDTH / (float)GAME_HEIGHT;

        float scale = 1.0f;
        int offsetX = 0, offsetY = 0;

        if (screenRatio > gameRatio) {
            // Screen is wider — pillarbox
            scale = (float)GetScreenHeight() / GAME_HEIGHT;
            offsetX = (GetScreenWidth() - (int)(GAME_WIDTH * scale)) / 2;
        } else {
            // Screen is taller — letterbox
            scale = (float)GetScreenWidth() / GAME_WIDTH;
            offsetY = (GetScreenHeight() - (int)(GAME_HEIGHT * scale)) / 2;
        }

        // Draw the scaled render texture
        DrawTexturePro(
            target.texture,
            (Rectangle){ 0, 0, (float)GAME_WIDTH, -(float)GAME_HEIGHT }, // flip vertically
            (Rectangle){ offsetX, offsetY, GAME_WIDTH * scale, GAME_HEIGHT * scale },
            (Vector2){ 0, 0 },
            0.0f,
            WHITE
        );

        EndDrawing();
    }

    for (int i = 0; i < spikeCount; i++) {
        free(spikeList[i]);
    }
    UnloadTexture(spikeTex);
    UnloadTexture(plr.texture);
    UnloadRenderTexture(target);
    UnloadTexture(*spik.texture);
    CloseWindow();
    return 0;
}