#include "raylib.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <math.h>

#define GAME_WIDTH  1920
#define GAME_HEIGHT 1080
#define PLAYER_JUMP_SPD -350.0f

// a million different structs and functions here lol

// entity types
typedef enum {
    GROUND,
    ENTITY_ENEMY_HAIRRY,
    HAZARD_SPIKE,
    SPRING
} EntityType;

// "blueprint" for hairry enemy
typedef struct hairryblueprint {
    Vector2 pos;
    Vector2 size;
    int health;
    int damage;
    int attackspeed;
    bool isfacingright;
    float vy;
    float vx;
    Texture2D* texture;
    Rectangle hitbox;
    Rectangle sighthitbox;
    int id;
    EntityType type;
    bool insight;
    bool canattack;
    float attacktime;
} hairry;

// player struct
typedef struct playerstuff {
    Vector2 pos;
    Vector2 size;
    int health;
    int damage;
    int attackspeed;
    int stompdamage;
    float jumptime;
    float maxheight;
    bool isfalling;
    bool iswalking;
    bool isjumping;
    bool iscolliding;
    float vy;
    float vx;
    float stamina;
    Rectangle hitbox;
    Rectangle dashhitbox; // only used for dash collision checking
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

typedef struct ground {
    Vector2 pos;
    Vector2 size;
    int id;
    Rectangle hitbox;
    Texture2D* texture;
    EntityType type;
} ground;

typedef struct spring {
    Vector2 pos;
    Vector2 size;
    int id;
    Rectangle hitbox;
    Texture2D* texture;
    EntityType type;
} spring;

typedef struct groundpillar {
	Vector2 pos;
	Vector2 size;
	int id;
	Rectangle hitbox;
	Texture2D* texture;
	EntityType type;
} groundpillar;

typedef struct groundpillartop {
	Vector2 pos;
	Vector2 size;
	int id;
	Rectangle hitbox;
	Texture2D* texture;
	EntityType type;
} groundpillartop;

void DrawHairry(hairry rect) {
    Rectangle source = { 0, 0, rect.texture->width, rect.texture->height };
    Rectangle dest = { rect.pos.x, rect.pos.y, rect.size.x, rect.size.y };
    Vector2 origin = { 0, 0 };
    DrawTexturePro(*rect.texture, source, dest, origin, 0.0f, WHITE);

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

void DrawGround(ground rect) {
    Rectangle source = { 0, 0, rect.texture->width, rect.texture->height };
    Rectangle dest = { rect.pos.x, rect.pos.y, rect.size.x, rect.size.y };
    Vector2 origin = { 0, 0 };
    DrawTexturePro(*rect.texture, source, dest, origin, 0.0f, WHITE);
}

void DrawSpring(spring rect) {
    Rectangle source = { 0, 0, rect.texture->width, rect.texture->height };
    Rectangle dest = { rect.pos.x, rect.pos.y, rect.size.x, rect.size.y };
    Vector2 origin = { 0, 0 };
    DrawTexturePro(*rect.texture, source, dest, origin, 0.0f, WHITE);
}

void DrawGroundPillar(groundpillar rect) {
    Rectangle source = { 0, 0, rect.texture->width, rect.texture->height };
    Rectangle dest = { rect.pos.x, rect.pos.y, rect.size.x, rect.size.y };
    Vector2 origin = { 0, 0 };
    DrawTexturePro(*rect.texture, source, dest, origin, 0.0f, WHITE);
}

void DrawGroundPillarTop(groundpillartop rect) {
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

hairry* CloneHairry(const hairry* blueprint, int new_id) {
	return &(hairry){
        .id = new_id,
        .texture = blueprint->texture,
        .pos = blueprint->pos,
	.sighthitbox = (Rectangle){blueprint->pos.x, blueprint->pos.y, 300, blueprint->hitbox.height},
	.isfacingright = blueprint->isfacingright,
	.insight = blueprint->insight,
	.canattack = true,
	.attackspeed = blueprint->attackspeed,
	.attacktime = 0,
        .hitbox = (Rectangle){
            blueprint->pos.x,
            blueprint->pos.y,
            blueprint->hitbox.width,
            blueprint->hitbox.height
        }
    };
}

hairry* NewHairry(const hairry* blueprint, int id) {
    hairry* s = malloc(sizeof(hairry));
    if (!s) return NULL;
    *s = *CloneHairry(blueprint, id);  // copy the clean temp struct
    return s;
}



groundpillar* CloneGroundPillar(const groundpillar* blueprint, int new_id) {
	return &(groundpillar){
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


groundpillar* NewGroundPillar(const groundpillar* blueprint, int id) {
    groundpillar* s = malloc(sizeof(groundpillar));
    if (!s) return NULL;
    *s = *CloneGroundPillar(blueprint, id);  // copy the clean temp struct
    return s;
}

groundpillartop* CloneGroundPillarTop(const groundpillartop* blueprint, int new_id) {
	return &(groundpillartop){
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

groundpillartop* NewGroundPillarTop(const groundpillartop* blueprint, int id) {
    groundpillartop* s = malloc(sizeof(groundpillartop));
    if (!s) return NULL;
    *s = *CloneGroundPillarTop(blueprint, id);  // copy the clean temp struct
    return s;
}


ground* CloneGround(const ground* blueprint, int new_id) {
    return &(ground){
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

spring* CloneSpring(const spring* blueprint, int new_id) {
    return &(spring){
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

spring* NewSpring(const spring* blueprint, int id) {
    spring* s = malloc(sizeof(spring));
    if (!s) return NULL;
    *s = *CloneSpring(blueprint, id);  // copy the clean temp struct
    return s;
}

ground* NewGround(const ground* blueprint, int id) {
    ground* s = malloc(sizeof(ground));
    if (!s) return NULL;
    *s = *CloneGround(blueprint, id);  // copy the clean temp struct
    return s;
}

float round60(float value) {
    float remainder = fmodf(value, 60.0);

    if (remainder >= 60.0) {
        value += (60.0 - remainder);
        return value;
    } else {
        value -= remainder;
        return value;
    }
}

spike* spikeList[40000]; // array of spike pointers
int spikeCount = 0;

ground* groundList[40000]; // array of ground pointers
int groundCount = 0;

spring* springList[40000]; // array of spring pointers
int springCount = 0;

groundpillar* groundpillarList[40000]; // array of groundpillar pointers
int groundpillarCount = 0;

groundpillartop* groundpillartopList[40000]; // array of groundpillartop pointers
int groundpillartopCount = 0;

hairry* hairryList[40000]; // array of hairry pointers
int hairryCount = 0;

float t = 0.5;

// update this whenever i add a new block that can be collided with (this function is for right-facing dashes)
bool rightDashCheck(player *plr) {
    bool shoulddash = false;
    plr->dashhitbox.x = plr->pos.x;
    plr->dashhitbox.y = plr->pos.y;
    for (int k = 0; k < 180; k++) {

        plr->dashhitbox.x += 1;

        for (int i = 0; i < spikeCount; i++) {
            if (CheckCollisionRecs(plr->dashhitbox, spikeList[i]->hitbox)) { 
                plr->dashhitbox.x = spikeList[i]->hitbox.x - plr->size.x;
                shoulddash = true;
                goto what;
            }
        }

        for (int i = 0; i < groundCount; i++) {
            if (CheckCollisionRecs(plr->dashhitbox, groundList[i]->hitbox)) {    
                plr->dashhitbox.x = groundList[i]->hitbox.x - plr->size.x;
                shoulddash = true; 
                goto what;
            }
        }

        for (int i = 0; i < springCount; i++) {
            if (CheckCollisionRecs(plr->dashhitbox, springList[i]->hitbox)) {    
                plr->dashhitbox.x = springList[i]->hitbox.x - plr->size.x;
                shoulddash = true; 
                goto what;
            }
        }

	for (int i = 0; i < groundpillarCount; i++) {
            if (CheckCollisionRecs(plr->dashhitbox, groundpillarList[i]->hitbox)) {    
                plr->dashhitbox.x = groundpillarList[i]->hitbox.x - plr->size.x;
                shoulddash = true; 
                goto what;
            }
        }

	for (int i = 0; i < groundpillartopCount; i++) {
            if (CheckCollisionRecs(plr->dashhitbox, groundpillartopList[i]->hitbox)) {    
                plr->dashhitbox.x = groundpillartopList[i]->hitbox.x - plr->size.x;
                shoulddash = true; 
                goto what;
            }
        }


	for (int i = 0; i < hairryCount; i++) {
            if (CheckCollisionRecs(plr->dashhitbox, hairryList[i]->hitbox)) { 
                shoulddash = true;
            }
        }

    }
    what:
        plr->pos.x = plr->dashhitbox.x;
    return shoulddash;
}

// pretty much the same as the right one but for the left
bool leftDashCheck(player *plr) {
    bool shoulddash = false;
    plr->dashhitbox.width = plr->size.x;
    plr->dashhitbox.height = plr->size.y;
    plr->dashhitbox.x = plr->pos.x;
    plr->dashhitbox.y = plr->pos.y;
    for (int k = 0; k < 180; k++) {

        plr->dashhitbox.x -= 1;

        for (int i = 0; i < spikeCount; i++) {
            if (CheckCollisionRecs(plr->dashhitbox, spikeList[i]->hitbox)) {
                plr->dashhitbox.x = spikeList[i]->hitbox.x + spikeList[i]->hitbox.width;
                shoulddash = true;
                goto what;
            }
        }

        for (int i = 0; i < groundCount; i++) {
            if (CheckCollisionRecs(plr->dashhitbox, groundList[i]->hitbox)) {
                plr->dashhitbox.x = groundList[i]->hitbox.x + groundList[i]->hitbox.width;
                shoulddash = true;
                goto what;
            }
        }

        for (int i = 0; i < springCount; i++) {
            if (CheckCollisionRecs(plr->dashhitbox, springList[i]->hitbox)) {
                plr->dashhitbox.x = springList[i]->hitbox.x + springList[i]->hitbox.width;
                shoulddash = true;
                goto what;
            }
        }

	for (int i = 0; i < groundpillarCount; i++) {
            if (CheckCollisionRecs(plr->dashhitbox, groundpillarList[i]->hitbox)) {    
                plr->dashhitbox.x = groundpillarList[i]->hitbox.x + plr->size.x;
                shoulddash = true; 
                goto what;
            }
        }

	for (int i = 0; i < groundpillartopCount; i++) {
            if (CheckCollisionRecs(plr->dashhitbox, groundpillartopList[i]->hitbox)) {    
                plr->dashhitbox.x = groundpillartopList[i]->hitbox.x + plr->size.x;
                shoulddash = true; 
                goto what;
            }
        }


	for (int i = 0; i < hairryCount; i++) {
            if (CheckCollisionRecs(plr->dashhitbox, hairryList[i]->hitbox)) {
                shoulddash = true;
            }
        }


    }
    what:
        plr->dashhitbox.x = plr->dashhitbox.x;

    return shoulddash;
}

void unloadlevel() {
    for (int i = 0; i < spikeCount; i++) {
        free(spikeList[i]);
    }
    for (int i = 0; i < groundCount; i++) {
        free(groundList[i]);
    }
    for (int i = 0; i < springCount; i++) {
        free(springList[i]);
    }
    for (int i = 0; i < groundpillarCount; i++) {
        free(groundpillarList[i]);
    }
    for (int i = 0; i < groundpillartopCount; i++) {
	free(groundpillartopList[i]);
    }
    for (int i = 0; i < hairryCount; i++) {
	free(hairryList[i]);
    }

}

// menu nav bools
bool inEditor = false;
bool inMainMenu = true;
bool inEditorPauseMenu = false;

// bools for if you are either play testing a level or in an actual one
bool isPlayTesting = false;
bool isplaying = false;

// object collision checking variables
bool isCollidingSpring = false;
bool isCollidingGround = false;
bool isCollidingGroundPillar = false;
bool isCollidingGroundPillarTop = false;

// im too lazy to put these player stats in the struct
float stamina = 100;

int objectCount = 0; // object count is all the sum of all the blocks that are placed down and is used to limit on how big levels can be (in a file sense and in a physical sense)

int objectPlaceID = 0; // just lets the game know what object you wanna place down, eg. 0 = a spike, 1 = ground

float gravity = 1000.0f; // GRAVITY

// variables used when you die
bool showDeathBox = false;
float deathBoxDuration = 2.5f;
float deathTime = 0.0f;
bool tele = false;

// variables used in dash mechanic
bool isfacingright = true;
float dashcd = 0.5f;
float dashtime = 0.0f;
bool candash = true;

float hairryspeed = 3.0;

float distancehp;

Rectangle editorEnterHitbox = (Rectangle){GAME_WIDTH / 2, 600, 50, 50}; // hitbox in main menu to open editor
Rectangle editorPauseMenuEnterHitbox = (Rectangle){70, 70, 60, 60}; // hitbox in editor to access pause menu

int main(void) {
    SetConfigFlags(FLAG_FULLSCREEN_MODE);
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
    spik.hitbox = (Rectangle){spik.pos.x, spik.pos.y, spik.size.x / 4, spik.size.y / 4};
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
    plr.jumptime = 0;
    plr.maxheight = 90;
    plr.stompdamage = 10;
    plr.attackspeed = 0.5;
    plr.isfalling = false;
    plr.iswalking = false;
    plr.isjumping = false;
    plr.vy = 0;
    plr.vx = 0;
    plr.stamina = 100;
    plr.hitbox = (Rectangle){ plr.pos.x, plr.pos.y, plr.size.x, plr.size.y };
    plr.dashhitbox = (Rectangle){ plr.pos.x, plr.pos.y, plr.size.x, plr.size.y };
    plr.texture = LoadTexture("SirAdamDogeingknight.png");

    ground groond;
    groond.pos.x = 0;
    groond.pos.y = 0;
    groond.size.x = 60;
    groond.size.y = 60;
    groond.hitbox = (Rectangle){groond.pos.x, groond.pos.y, groond.size.x, groond.size.y};
    Texture2D groundTex = LoadTexture("Ground.png");
    groond.texture = &groundTex;
    groond.type = GROUND;

    spring sprin;
    sprin.pos.x = 0;
    sprin.pos.y = 0;
    sprin.size.x = 60;
    sprin.size.y = 60;
    sprin.hitbox = (Rectangle){sprin.pos.x, sprin.pos.y, sprin.size.x, sprin.size.y};
    Texture2D springTex = LoadTexture("Spring.png");
    sprin.texture = &springTex;
    sprin.type = SPRING;

    groundpillar groondpillar;
    groondpillar.pos.x = 0;
    groondpillar.pos.y = 0;
    groondpillar.size.x = 60;
    groondpillar.size.y = 60;
    groondpillar.hitbox = (Rectangle){groondpillar.pos.x, groondpillar.pos.y, groondpillar.size.x, groondpillar.size.y};
    Texture2D groundPillarTex = LoadTexture("GroundPillar.png");
    groondpillar.texture = &groundPillarTex;
    groondpillar.type = GROUND;

    groundpillartop groondpillartop;
    groondpillartop.pos.x = 0;
    groondpillartop.pos.y = 0;
    groondpillartop.size.x = 60;
    groondpillartop.size.y = 60;
    groondpillartop.hitbox = (Rectangle){groondpillartop.pos.x, groondpillartop.pos.y, groondpillartop.size.x, groondpillartop.size.y};
    Texture2D groundPillarTopTex = LoadTexture("GroundPillarTop.png");
    groondpillartop.texture = &groundPillarTopTex;
    groondpillartop.type = GROUND;

    hairry hairy;
    hairy.pos.x = 0;
    hairy.pos.y = 0;
    hairy.size.x = 60;
    hairy.size.y = 60;
    hairy.health = 30;
    hairy.damage = 10;
    hairy.isfacingright = true;
    hairy.type = ENTITY_ENEMY_HAIRRY;
    hairy.vx = -1;
    hairy.vy = 0;
    hairy.attackspeed = 1.5;
    hairy.canattack = true;
    hairy.attacktime = 0;
    hairy.hitbox = (Rectangle){hairy.pos.x, hairy.pos.y, hairy.size.x, hairy.size.y};
    hairy.sighthitbox = (Rectangle){hairy.pos.x, hairy.pos.y, 300, 60};
    Texture2D hairryTex =  LoadTexture("Hairry.png");
    hairy.texture = &hairryTex;

    Texture2D staminaTex = LoadTexture("stamina.png");

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        float screenRadio = (float)GetScreenWidth() / (float)GetScreenHeight();
        float gameRadio   = (float)GAME_WIDTH / (float)GAME_HEIGHT;

        float skale = 1.0f;
        int offsetex = 0, offsetwhy = 0;

        if (screenRadio > gameRadio) {
            // Screen is wider — pillarbox
            skale = (float)GetScreenHeight() / GAME_HEIGHT;
            offsetex = (GetScreenWidth() - (int)(GAME_WIDTH * skale)) / 2;
        } else {
            // Screen is taller — letterbox
            skale = (float)GetScreenWidth() / GAME_WIDTH;
            offsetwhy = (GetScreenHeight() - (int)(GAME_HEIGHT * skale)) / 2;
        }
        Vector2 mouse = GetMousePosition();

        Vector2 worldMouse = {
            (mouse.x - offsetex) / skale,
            (mouse.y - offsetwhy) / skale
        };

        // update player movement
        if (isPlayTesting || isplaying) {
            if (IsKeyDown(KEY_A)) {
                plr.vx = -1;
                plr.pos.x -= 4;
                plr.hitbox.x = plr.pos.x;
                plr.hitbox.y = plr.pos.y;
                isfacingright = false;
            }
            if (IsKeyDown(KEY_D)) {
                plr.vx = 1;
                plr.pos.x += 4;
                plr.hitbox.x = plr.pos.x;
                plr.hitbox.y = plr.pos.y;
                isfacingright = true;
            }
            if (IsKeyUp(KEY_A) && IsKeyUp(KEY_D)) {
                plr.vx = 0;
                plr.hitbox.x = plr.pos.x;
                plr.hitbox.y = plr.pos.y;
            }
            if (IsKeyPressed(KEY_P)) {
                objectPlaceID = 0;
            }
            if (IsKeyPressed(KEY_O)) {
                objectPlaceID = 1;
            }
            if (IsKeyPressed(KEY_I)) {
                objectPlaceID = 2;
            }
	    if (IsKeyPressed(KEY_U)) {
	    	objectPlaceID = 3;
	    }
	    if (IsKeyPressed(KEY_Y)) {
	    	objectPlaceID = 4;
	    }
	    if (IsKeyPressed(KEY_T)) {
		objectPlaceID = 5;
	    }
            if (IsKeyPressed(KEY_SPACE) && isCollidingGround == true || IsKeyPressed(KEY_SPACE) && isCollidingSpring || IsKeyPressed(KEY_SPACE) && isCollidingGroundPillar || IsKeyPressed(KEY_SPACE) && isCollidingGroundPillarTop) {
                plr.iscolliding = false;
                plr.isjumping = true;
                plr.vy = -400.0;
            }
            if (!candash && (GetTime() - dashtime >= dashcd)) {
                candash = true;
            }
            if (IsKeyPressed(KEY_R) && stamina >= 30) {
                if (candash) {
		    stamina -= 30;
                    candash = false;
                    dashtime = GetTime();
                    if (isfacingright == true) {
                        if (rightDashCheck(&plr)) {
                            plr.pos.x = plr.dashhitbox.x;
                            plr.vy = 0;
                            plr.hitbox.y = plr.pos.y;
                            plr.hitbox.x = plr.pos.x;
                        } else {
                            plr.pos.x = plr.dashhitbox.x;
                            plr.vy = 0;
                            plr.hitbox.y = plr.pos.y;
                            plr.hitbox.x = plr.pos.x;
                        }
                    } else if (isfacingright == false) {
                        if (leftDashCheck(&plr)) {
                            plr.pos.x = plr.dashhitbox.x;
                            plr.vy = 0;
                            plr.hitbox.y = plr.pos.y;
                            plr.hitbox.x = plr.pos.x;
                        } else {
                            plr.pos.x = plr.dashhitbox.x;
                            plr.vy = 0;
                            plr.hitbox.y = plr.pos.y;
                            plr.hitbox.x = plr.pos.x;
                        }
                    }
                }
            }
        }
	for (int i = 0; i < hairryCount; i++) {
	    if (!hairryList[i]->canattack && (GetTime() - hairryList[i]->attacktime >= hairryList[i]->attackspeed)) {
		hairryList[i]->canattack = true;
	    }
	    if (CheckCollisionRecs(plr.hitbox, hairryList[i]->hitbox)) {
		if (hairryList[i]->canattack) {
		    hairryList[i]->canattack = false;
		    hairryList[i]->attacktime = GetTime();
		    plr.health -= 15;
		}
	    }
	}
        
        
        objectCount = spikeCount + groundCount + springCount + groundpillarCount + groundpillartopCount + hairryCount;
        if (inEditor) {
            isPlayTesting = true;
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && objectCount <= 40000) {
                Vector2 pos = GetMousePosition();
                pos.x = round60(pos.x);
                pos.y = round60(pos.y);
                if (objectPlaceID == 0) {
                
                    spike* newSpike = NewSpike(&spik, spikeCount);
                
                    // Customize new spike's position & size to default
                    newSpike->pos = pos;
                    newSpike->size = (Vector2){ 60, 60 };
                    newSpike->hitbox = (Rectangle){ pos.x + newSpike->size.x * 0.375, pos.y + newSpike->size.y * 0.375, newSpike->size.x / 4, newSpike->size.y / 4 };
                    newSpike->texture = &spikeTex;
            
                    spikeList[spikeCount++] = newSpike;
                } else if (objectPlaceID == 1) {
    
                    ground* newGround = NewGround(&groond, groundCount);
                
                    // Customize new ground's position & size to default
                    newGround->pos = pos;
                    newGround->size = (Vector2){ 60, 60 };
                    newGround->hitbox = (Rectangle){ pos.x, pos.y, newGround->size.x, newGround->size.y };
                    newGround->texture = &groundTex;

                    groundList[groundCount++] = newGround;
                } else if (objectPlaceID == 2) {
    
                    spring* newSpring = NewSpring(&sprin, springCount);
                
                    // Customize new ground's position & size to default
                    newSpring->pos = pos;
                    newSpring->size = (Vector2){ 60, 60 };
                    newSpring->hitbox = (Rectangle){ pos.x, pos.y, newSpring->size.x, newSpring->size.y };
                    newSpring->texture = &springTex;
            
                    springList[springCount++] = newSpring;
                } else if (objectPlaceID == 3) {
    
                    groundpillar* newGroundPillar = NewGroundPillar(&groondpillar, groundpillarCount);
                
                    // Customize new ground's position & size to default
                    newGroundPillar->pos = pos;
                    newGroundPillar->size = (Vector2){ 60, 60 };
                    newGroundPillar->hitbox = (Rectangle){ pos.x, pos.y, newGroundPillar->size.x, newGroundPillar->size.y };
                    newGroundPillar->texture = &groundPillarTex;
            
                    groundpillarList[groundpillarCount++] = newGroundPillar;
                } else if (objectPlaceID == 4) {
    
                    groundpillartop* newGroundPillarTop = NewGroundPillarTop(&groondpillartop, groundpillartopCount);
                
                    // Customize new ground's position & size to default
                    newGroundPillarTop->pos = pos;
                    newGroundPillarTop->size = (Vector2){ 60, 60 };
                    newGroundPillarTop->hitbox = (Rectangle){ pos.x, pos.y, newGroundPillarTop->size.x, newGroundPillarTop->size.y };
                    newGroundPillarTop->texture = &groundPillarTopTex;
            
                    groundpillartopList[groundpillartopCount++] = newGroundPillarTop;
                } else if (objectPlaceID == 5) {
    
                    hairry* newHairry = NewHairry(&hairy, hairryCount);
                
                    // Customize new ground's position & size to default
                    newHairry->pos = pos;
                    newHairry->size = (Vector2){ 60, 60 };
                    newHairry->hitbox = (Rectangle){ pos.x, pos.y, newHairry->size.x, newHairry->size.y };
                    newHairry->texture = &hairryTex;
            
                    hairryList[hairryCount++] = newHairry;
                }


            }
        }

        if (inEditor && !inMainMenu || isplaying && !inMainMenu || isPlayTesting && !inMainMenu) {
            if (IsKeyPressed(KEY_B)) {
                inEditor = false;
                isplaying = false;
                isPlayTesting = false;
                inMainMenu = true;
            }
        }

	t = 2.0 * dt;

	for (int i = 0; i < hairryCount; i++) {
	   if (!hairryList[i]->insight) {
	       if (hairryList[i]->isfacingright) {
		   hairryList[i]->isfacingright = false;
	       } else if (!hairryList[i]->isfacingright) {
		   hairryList[i]->isfacingright = true;
	       }
	   }
	    
	    if (hairryList[i]->isfacingright) {
		hairryList[i]->sighthitbox.x = hairryList[i]->pos.x;
		hairryList[i]->sighthitbox.y = hairryList[i]->pos.y;
		hairryList[i]->sighthitbox.width = 300;
		hairryList[i]->sighthitbox.height = hairryList[i]->size.y;
	    } else if (!hairryList[i]->isfacingright) {
		hairryList[i]->sighthitbox.x = hairryList[i]->pos.x - 240;
		hairryList[i]->sighthitbox.y = hairryList[i]->pos.y;
		hairryList[i]->sighthitbox.width = 300;
		hairryList[i]->sighthitbox.height = hairryList[i]->size.y;
	    }


	    if (CheckCollisionRecs(plr.hitbox, hairryList[i]->sighthitbox) && hairryList[i]->isfacingright == true) {
		hairryList[i]->pos.x += (plr.pos.x - hairryList[i]->pos.x) * t;
		hairryList[i]->insight = true;
	    } else if (CheckCollisionRecs(plr.hitbox, hairryList[i]->sighthitbox) && hairryList[i]->isfacingright == false) {
		hairryList[i]->pos.x += (plr.pos.x - hairryList[i]->pos.x) * t;
		hairryList[i]->insight = true;
	    } else {
		hairryList[i]->insight = false;
	    }
	}

        isCollidingGround = false;
        isCollidingSpring = false;
	isCollidingGroundPillar = false;
	isCollidingGroundPillarTop = false;

        // Update hitbox after horizontal move
        plr.hitbox.x = plr.pos.x;
        plr.hitbox.y = plr.pos.y;

        // horizontal collision with spike
        for (int i = 0; i < spikeCount; i++) {
            if (CheckCollisionRecs(plr.hitbox, spikeList[i]->hitbox)) {
                if (plr.vx > 0) {
                plr.pos.x = spikeList[i]->hitbox.x - plr.size.x;
                plr.health = 0;
                } else if (plr.vx < 0) {
                plr.pos.x = spikeList[i]->hitbox.x + spikeList[i]->hitbox.width;
                plr.health = 0;
                }
                plr.vx = 0;
                plr.hitbox.x = plr.pos.x;
            }
        }

        // horizontal collision with ground
        for (int i = 0; i < groundCount; i++) {
            if (CheckCollisionRecs(plr.hitbox, groundList[i]->hitbox)) {
                if (plr.vx > 0) {
                plr.pos.x = groundList[i]->hitbox.x - plr.size.x;
                } else if (plr.vx < 0) {
                plr.pos.x = groundList[i]->hitbox.x + groundList[i]->hitbox.width;
                }
                plr.vx = 0;
                plr.hitbox.x = plr.pos.x;
            }

	    for (int k = 0; k < hairryCount; k++) {
		if (CheckCollisionRecs(hairryList[k]->hitbox, groundList[i]->hitbox)) {
		    if (hairryList[k]->vx > 0) {
			// falling down
			hairryList[k]->pos.x = groundList[i]->hitbox.x - hairryList[k]->size.x;
		    } else if (hairryList[k]->vx < 0) {
			// hitting head
			hairryList[k]->pos.x = groundList[i]->hitbox.x + groundList[i]->hitbox.width;
		    }
		    hairryList[k]->vx = 0;
		    hairryList[k]->hitbox.x = hairryList[k]->pos.x;
		}
	    }

        }

        // horizontal collision with spring
        for (int i = 0; i < springCount; i++) {
            if (CheckCollisionRecs(plr.hitbox, springList[i]->hitbox)) {
                if (plr.vx > 0) {
                    plr.pos.x = springList[i]->hitbox.x - plr.size.x;
                } else if (plr.vx < 0) {
                    plr.pos.x = springList[i]->hitbox.x + springList[i]->hitbox.width;
                }
                plr.vx = 0;
                plr.hitbox.x = plr.pos.x;
            }

	    for (int k = 0; k < hairryCount; k++) {
		if (CheckCollisionRecs(hairryList[k]->hitbox, springList[i]->hitbox)) {
		    if (hairryList[k]->vx > 0) {
			// falling down
			hairryList[k]->pos.x = springList[i]->hitbox.x - hairryList[k]->size.x;
		    } else if (hairryList[k]->vx < 0) {
			// hitting head
			hairryList[k]->pos.x = springList[i]->hitbox.x + springList[i]->hitbox.width;
		    }
		    hairryList[k]->vx = 0;
		    hairryList[k]->hitbox.x = hairryList[k]->pos.x;
		}
	    }

        }

	// horizontal collision with groundpillar
        for (int i = 0; i < groundpillarCount; i++) {
            if (CheckCollisionRecs(plr.hitbox, groundpillarList[i]->hitbox)) {
                if (plr.vx > 0) {
                plr.pos.x = groundpillarList[i]->hitbox.x - plr.size.x;
                } else if (plr.vx < 0) {
                plr.pos.x = groundpillarList[i]->hitbox.x + groundpillarList[i]->hitbox.width;
                }
                plr.vx = 0;
                plr.hitbox.x = plr.pos.x;
            }

	    for (int k = 0; k < hairryCount; k++) {
		if (CheckCollisionRecs(hairryList[k]->hitbox, groundpillarList[i]->hitbox)) {
		    if (hairryList[k]->vx > 0) {
			// falling down
			hairryList[k]->pos.x = groundpillarList[i]->hitbox.x - hairryList[k]->size.x;
		    } else if (hairryList[k]->vx < 0) {
			// hitting head
			hairryList[k]->pos.x = groundpillarList[i]->hitbox.x + groundpillarList[i]->hitbox.width;
		    }
		    hairryList[k]->vx = 0;
		    hairryList[k]->hitbox.x = hairryList[k]->pos.x;
		}
	    }

        }

	// horizontal collision with groundpillar
        for (int i = 0; i < groundpillartopCount; i++) {
            if (CheckCollisionRecs(plr.hitbox, groundpillartopList[i]->hitbox)) {
                if (plr.vx > 0) {
                plr.pos.x = groundpillartopList[i]->hitbox.x - plr.size.x;
                } else if (plr.vx < 0) {
                plr.pos.x = groundpillartopList[i]->hitbox.x + groundpillartopList[i]->hitbox.width;
                }
                plr.vx = 0;
                plr.hitbox.x = plr.pos.x;
            }

	    for (int k = 0; k < hairryCount; k++) {
		if (CheckCollisionRecs(hairryList[k]->hitbox, groundpillartopList[i]->hitbox)) {
		    if (hairryList[k]->vx > 0) {
			// falling down
			hairryList[k]->pos.x = groundpillartopList[i]->hitbox.x - hairryList[k]->size.x;
		    } else if (hairryList[k]->vx < 0) {
			// hitting head
			hairryList[k]->pos.x = groundpillartopList[i]->hitbox.x + groundpillartopList[i]->hitbox.width;
		    }
		    hairryList[k]->vx = 0;
		    hairryList[k]->hitbox.x = hairryList[k]->pos.x;
		}
	    }

        }



        // ---- Vertical movement & collision & stamina regen
        if (isPlayTesting || isplaying) {
            plr.vy += gravity * dt;
            plr.pos.y += plr.vy * dt;
	    for (int i = 0; i < hairryCount; i++) {
	    	hairryList[i]->vy += gravity * dt;
		hairryList[i]->pos.y += hairryList[i]->vy * dt;
		hairryList[i]->hitbox.x = hairryList[i]->pos.x;
		hairryList[i]->hitbox.y = hairryList[i]->pos.y;
	    }
	    if (stamina <= 100) {
		stamina += 4.0f * dt;
	    }
        }

        // Update hitbox again after vertical move
        plr.hitbox.x = plr.pos.x;
        plr.hitbox.y = plr.pos.y;

        // vertical collision with spike
        for (int i = 0; i < spikeCount; i++) {
            if (CheckCollisionRecs(plr.hitbox, spikeList[i]->hitbox)) {
                if (plr.vy > 0) {
                // Falling down
                plr.pos.y = spikeList[i]->hitbox.y - plr.size.y;
                plr.isfalling = false;
                plr.health = 0;
            } else if (plr.vy < 0) {
                // Jumping up into spike
                plr.pos.y = spikeList[i]->hitbox.y + spikeList[i]->hitbox.height;
                plr.iscolliding = false;
                plr.health = 0;
            }
            plr.vy = 0;
            plr.hitbox.y = plr.pos.y;
            }
        }

        // vertical collision with ground
        for (int i = 0; i < groundCount; i++) {
            if (CheckCollisionRecs(plr.hitbox, groundList[i]->hitbox)) {
                if (plr.vy > 0) {
                // Falling down
                plr.pos.y = groundList[i]->hitbox.y - plr.size.y;
                plr.isfalling = false;
               isCollidingGround = true;
            } else if (plr.vy < 0) {
                // Jumping up into spike
                plr.pos.y = groundList[i]->hitbox.y + groundList[i]->hitbox.height;
                plr.iscolliding = false;
            }
            plr.vy = 0;
            plr.hitbox.y = plr.pos.y;
            }

	    for (int k = 0; k < hairryCount; k++) {
		if (CheckCollisionRecs(hairryList[k]->hitbox, groundList[i]->hitbox)) {
		    if (hairryList[k]->vy > 0) {
			// falling down
			hairryList[k]->pos.y = groundList[i]->hitbox.y - hairryList[k]->size.y;
		    } else if (hairryList[k]->vy < 0) {
			// hitting head
			hairryList[k]->pos.y = groundList[i]->hitbox.y + groundList[i]->hitbox.height;
		    }
		    hairryList[k]->vy = 0;
		    hairryList[k]->hitbox.y = hairryList[k]->pos.y;
		}
	    }

        }

        // vertical collision with spring
        for (int i = 0; i < springCount; i++) {
            if (CheckCollisionRecs(plr.hitbox, springList[i]->hitbox)) {
                if (plr.vy > 0) {
                // Falling down
                plr.pos.y = springList[i]->hitbox.y - plr.size.y;
                plr.isfalling = false;
                isCollidingSpring = true;
                plr.vy = -700.0;
            } else if (plr.vy < 0) {
                // Jumping up into spike
                plr.pos.y = springList[i]->hitbox.y + springList[i]->hitbox.height;
                plr.iscolliding = false;
                plr.vy = 700.0;
            }
            plr.hitbox.y = plr.pos.y;
            }

	    for (int k = 0; k < hairryCount; k++) {
		if (CheckCollisionRecs(hairryList[k]->hitbox, springList[i]->hitbox)) {
		    if (hairryList[k]->vy > 0) {
			// falling down
			hairryList[k]->pos.y = springList[i]->hitbox.y - hairryList[k]->size.y;
			hairryList[k]->vy = -700.0;
		    } else if (hairryList[k]->vy < 0) {
			// hitting head
			hairryList[k]->pos.y = springList[i]->hitbox.y + springList[i]->hitbox.height;
			hairryList[k]->vy = 700.0;
		    }
		    hairryList[k]->hitbox.y = hairryList[k]->pos.y;
		}
	    }

        }

	// vertical collision with groundpillar
        for (int i = 0; i < groundpillarCount; i++) {
            if (CheckCollisionRecs(plr.hitbox, groundpillarList[i]->hitbox)) {
                if (plr.vy > 0) {
                // Falling down
                plr.pos.y = groundpillarList[i]->hitbox.y - plr.size.y;
                plr.isfalling = false;
               isCollidingGroundPillar = true;
            } else if (plr.vy < 0) {
                // Jumping up into spike
                plr.pos.y = groundpillarList[i]->hitbox.y + groundpillarList[i]->hitbox.height;
                plr.iscolliding = false;
            }
            plr.vy = 0;
            plr.hitbox.y = plr.pos.y;
            }
	    for (int k = 0; k < hairryCount; k++) {
		if (CheckCollisionRecs(hairryList[k]->hitbox, groundpillarList[i]->hitbox)) {
		    if (hairryList[k]->vy > 0) {
			// falling down
			hairryList[k]->pos.y = groundpillarList[i]->hitbox.y - hairryList[k]->size.y;
		    } else if (hairryList[k]->vy < 0) {
			// hitting head
			hairryList[k]->pos.y = groundpillarList[i]->hitbox.y + groundpillarList[i]->hitbox.height;
		    }
		    hairryList[k]->vy = 0;
		    hairryList[k]->hitbox.y = hairryList[k]->pos.y;
		}
	    }

        }

	// vertical collision with groundpillartop
        for (int i = 0; i < groundpillartopCount; i++) {
            if (CheckCollisionRecs(plr.hitbox, groundpillartopList[i]->hitbox)) {
                if (plr.vy > 0) {
                // Falling down
                plr.pos.y = groundpillartopList[i]->hitbox.y - plr.size.y;
                plr.isfalling = false;
               isCollidingGroundPillarTop = true;
            } else if (plr.vy < 0) {
                // Jumping up into spike
                plr.pos.y = groundpillartopList[i]->hitbox.y + groundpillartopList[i]->hitbox.height;
                plr.iscolliding = false;
            }
            plr.vy = 0;
            plr.hitbox.y = plr.pos.y;
            }
	    for (int k = 0; k < hairryCount; k++) {
		if (CheckCollisionRecs(hairryList[k]->hitbox, groundpillartopList[i]->hitbox)) {
		    if (hairryList[k]->vy > 0) {
			// falling down
			hairryList[k]->pos.y = groundpillartopList[i]->hitbox.y - hairryList[k]->size.y;
		    } else if (hairryList[k]->vy < 0) {
			// hitting head
			hairryList[k]->pos.y = groundpillartopList[i]->hitbox.y + groundpillartopList[i]->hitbox.height;
		    }
		    hairryList[k]->vy = 0;
		    hairryList[k]->hitbox.y = hairryList[k]->pos.y;
		}
	    }

        }


        // check if it's time to get rid of death message
        if (showDeathBox && (GetTime() - deathTime >= deathBoxDuration)) {
            showDeathBox = false;
            plr.health = 100;
            if (tele == false) {
                plr.pos.x = 400;
                plr.pos.y = 100;
                tele = true;
                plr.vy = 0;
            }
        }

        // Draw
        BeginDrawing();

        // actually begin drawing with good resolution
        BeginTextureMode(target);
        ClearBackground(BLUE);

        for (int i = 0; i < spikeCount; i++) {
            DrawSpike(*spikeList[i]);
        }
        for (int i = 0; i < groundCount; i++) {
            DrawGround(*groundList[i]);
        }
        for (int i = 0; i < springCount; i++) {
            DrawSpring(*springList[i]);
        }
	for (int i = 0; i < groundpillarCount; i++) {
	    DrawGroundPillar(*groundpillarList[i]);
	}
	for (int i = 0; i < groundpillartopCount; i++) {
	    DrawGroundPillarTop(*groundpillartopList[i]);
	}
	for (int i = 0; i < hairryCount; i++) {
	    DrawHairry(*hairryList[i]);
	}
        if (showDeathBox) {
            DrawText("You died...", 960, 540, 20, RAYWHITE);
        }

        if (isplaying || isPlayTesting) {
            DrawPlayer(plr);
	    DrawRectangle(20, 20, 300, 50, DARKPURPLE);
	    DrawRectangle(20, 20, stamina * 3, 50, PURPLE);
	    DrawTexture(staminaTex, 340, 20, PURPLE);
	    DrawRectangle(390, 20, 300, 50, DARKGREEN);
	    DrawRectangle(390, 20, plr.health * 3, 50, GREEN);
        }

        if (inMainMenu && !inEditor && !inEditorPauseMenu) {
            DrawText("Dogeia", GAME_WIDTH / 2, 300, 100, RAYWHITE);
            DrawText(">", GAME_WIDTH / 2, 450, 50, RAYWHITE);
            DrawText("#", GAME_WIDTH / 2, 600, 50, RAYWHITE);
            plr.pos.y = 100;
            plr.pos.x = 400;
            plr.vy = 0;
	    stamina = 100;
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                if (CheckCollisionPointRec(worldMouse, editorEnterHitbox)) {
                    inMainMenu = false;
                    inEditor = true;
                }
            }
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

        // check delete spikes
        for (int i = 0; i < spikeCount; i++) {
            if (spikeList[i] && CheckCollisionPointRec(worldMouse, spikeList[i]->hitbox)) {
                // if right mousebutton
                if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
                    int spikeid = spikeList[i]->id;
                    free(spikeList[i]);
                    for (int j = spikeid; j < spikeCount - 1; j++) {
                        spikeList[j] = spikeList[j + 1];
                    }
                    spikeList[spikeCount - 1] = NULL;
                    spikeCount--;
                    for (int k = spikeid; k < spikeCount; k++) {
                        spikeList[k]->id = k;
                    }
                }
            }
        }

        // check delete ground
        for (int i = 0; i < groundCount; i++) {
            if (groundList[i] && CheckCollisionPointRec(worldMouse, groundList[i]->hitbox)) {
                // if right mousebutton
                if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
                    int groundid = groundList[i]->id;
                    free(groundList[i]);
                    for (int j = groundid; j < groundCount - 1; j++) {
                        groundList[j] = groundList[j + 1];
                    }
                    groundList[groundCount - 1] = NULL;
                    groundCount--;
                    for (int k = groundid; k < groundCount; k++) {
                        groundList[k]->id = k;
                    }
                }
            }
        }

	// check delete groundpillar
        for (int i = 0; i < groundpillarCount; i++) {
            if (groundpillarList[i] && CheckCollisionPointRec(worldMouse, groundpillarList[i]->hitbox)) {
                // if right mousebutton
                if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
                    int groundpillarid = groundpillarList[i]->id;
                    free(groundpillarList[i]);
                    for (int j = groundpillarid; j < groundpillarCount - 1; j++) {
                        groundpillarList[j] = groundpillarList[j + 1];
                    }
                    groundpillarList[groundpillarCount - 1] = NULL;
                    groundpillarCount--;
                    for (int k = groundpillarid; k < groundpillarCount; k++) {
                        groundpillarList[k]->id = k;
                    }
                }
            }
        }

	// check delete groundpillartop
        for (int i = 0; i < groundpillartopCount; i++) {
            if (groundpillartopList[i] && CheckCollisionPointRec(worldMouse, groundpillartopList[i]->hitbox)) {
                // if right mousebutton
                if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
                    int groundpillartopid = groundpillartopList[i]->id;
                    free(groundpillartopList[i]);
                    for (int j = groundpillartopid; j < groundpillartopCount - 1; j++) {
                        groundpillartopList[j] = groundpillartopList[j + 1];
                    }
                    groundpillartopList[groundpillartopCount - 1] = NULL;
                    groundpillartopCount--;
                    for (int k = groundpillartopid; k < groundpillartopCount; k++) {
                        groundpillartopList[k]->id = k;
                    }
                }
            }
        }


        // check delete spring
        for (int i = 0; i < springCount; i++) {
            if (springList[i] && CheckCollisionPointRec(worldMouse, springList[i]->hitbox)) {
                // if right mousebutton
                if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
                    int springid = springList[i]->id;
                    free(springList[i]);
                    for (int j = springid; j < springCount - 1; j++) {
                        springList[j] = springList[j + 1];
                    }
                    springList[springCount - 1] = NULL;
                    springCount--;
                    for (int k = springid; k < springCount; k++) {
                        springList[k]->id = k;
                    }
                }
            }
        }


	// check delete hairry
        for (int i = 0; i < hairryCount; i++) {
            if (hairryList[i] && CheckCollisionPointRec(worldMouse, hairryList[i]->hitbox)) {
                // if right mousebutton
                if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
                    int hairryid = hairryList[i]->id;
                    free(hairryList[i]);
                    for (int j = hairryid; j < hairryCount - 1; j++) {
                        hairryList[j] = hairryList[j + 1];
                    }
                    hairryList[hairryCount - 1] = NULL;
                    hairryCount--;
                    for (int k = hairryid; k < hairryCount; k++) {
                        hairryList[k]->id = k;
                    }
                }
            }
        }


        if (plr.health <= 0 && !showDeathBox) {
            showDeathBox = true;
            deathTime = GetTime();
            plr.pos.x = -100000;
            plr.pos.y = -100000;
            tele = false;
        }
        
    }
    unloadlevel();
    UnloadTexture(staminaTex);
    UnloadTexture(groundTex);
    UnloadTexture(spikeTex);
    UnloadTexture(plr.texture);
    UnloadRenderTexture(target);
    UnloadTexture(*spik.texture);
    CloseWindow();
    return 0;
}
