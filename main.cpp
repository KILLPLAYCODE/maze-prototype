#include "raylib.h"
#include <vector>
#include <iostream>

Rectangle PlayerRect(Vector2 playerPos, float size) //player hitbox
{
    return {playerPos.x + 1.5f, playerPos.y + 0.5f,size - 2,size - 2};
}
Rectangle Key(Vector2 keyPos, float keyScale) //key hitbox
{
    return {keyPos.x,keyPos.y,keyScale,keyScale};
}
Rectangle ExitDoor(Vector2 doorPos, float doorScale) //door hitbox
{
    return {doorPos.x, doorPos.y, doorScale, doorScale};
}
Rectangle Exit(Vector2 exitPos, float exitScale) //exit game hitbox
{
    return {exitPos.x, exitPos.y, exitScale, exitScale};
}

int main(){
Vector2 playerPos = {32,32};
Vector2 keyPos = {205,94};
Vector2 doorPos = {733,450};
Vector2 exitPos = {734, 460};
    float size = 32;
    float keyScale = 32;
    float doorScale = 32;
    float exitScale = 32;
    float speed = 200;
    bool keyOpen = false;
    bool win = false;

    std::vector<Rectangle> walls = { //walls hitboxes
        {0,0,800,32},
        {0,0,32,600},
        {768,0,32,600},
        {0,450,730,32}, //up,left,right and down

        {64,0,16,200}, //all other
        {64,200,200,16},
        {64,258,204,60},
        {64,258,16,160},
        {64,402,200,16},
        {118,66,150,16},
        {118,66,16,100},
        {184,66,16,100},
        {252,66,16,150},
        {298,350,16,100},
        {298,350,150,16},
        {440,350,16,68},
        {350,402,90,16},
        {118,350,200,16},
        {305,258,16,60},
        {490,258,16,60},
        {490,202,16,60},
        {489,0,16,80},
        {305,70,16,145},
        {305,70,200,16},
        {305,200,200,16},
        {305,140,200,16},
        {305,258,200,16},
        {305,302,200,16},
        {545,0,16,400},
        {545,400,50,16},
        {635,400,95,16},
        {579,0,16,400},
        {635,66,16,334},
        {714,66,16,334},
        {714,122,90,16}
    };
    
    int wallCount = walls.size();

    const int screenWidth = 800;
    const int screenHeight = 480;

    InitWindow(screenWidth, screenHeight, "Maze prototype");

    InitAudioDevice();

    Music music = LoadMusicStream("resources/music.mp3");
    Sound sound = LoadSound("resources/get.mp3");

    PlayMusicStream(music);

    SetMusicVolume(music, 0.8f);
    SetSoundVolume(sound, 1.8f);

    Texture2D key = LoadTexture("resources/key.png");

    SetTargetFPS(60);
    while (!WindowShouldClose() && win == false)
    {
        UpdateMusicStream(music);
        float dt = GetFrameTime();
        Vector2 dir = {0,0};

        if(IsKeyDown(KEY_W)){
            dir.y -= 1;
        }
        if(IsKeyDown(KEY_S)){
            dir.y += 1;
        }
        if(IsKeyDown(KEY_A)){
            dir.x -= 1;
        }
        if(IsKeyDown(KEY_D)){
            dir.x += 1;
        }
        Vector2 move = {
            dir.x * speed * dt,
            dir.y * speed * dt
        };

        playerPos.x += move.x;

        Rectangle pr = PlayerRect(playerPos, size);
        Rectangle dr = ExitDoor(doorPos, doorScale);

        for (int i = 0; i < wallCount; i++){
            if (CheckCollisionRecs(pr, walls[i]) || CheckCollisionRecs(pr, dr)){
                playerPos.x -= move.x;
                break;
            }
        }
        playerPos.y += move.y;

        pr = PlayerRect(playerPos, size);
        dr = ExitDoor(doorPos, doorScale);


        for (int i = 0; i < wallCount; i++){
            if (CheckCollisionRecs(pr, walls[i]) || CheckCollisionRecs(pr, dr)){
                playerPos.y -= move.y;
                break;
            }
        }
        Rectangle k = Key(keyPos,keyScale);

        if (CheckCollisionRecs(pr, k)){
            keyOpen = true;
            keyPos.y = -100; 
            doorPos.x = 800;
            PlaySound(sound);
        }
        Rectangle ex = Exit(exitPos, exitScale);

        if (CheckCollisionRecs(pr, ex)){
            win = true;
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);
        for(int i = 0; i < wallCount; i++){   //walls
            DrawRectangleRec(walls[i], GRAY);
        }

        DrawRectangleV(playerPos, {size, size}, BLUE); //player
        DrawRectangle(714,400,16,60, GRAY); //fake wall

        if (keyOpen == false){
        DrawTextureEx(key, (Vector2){205,94}, 0.0f, 0.08f, WHITE); //key
        DrawTextureEx(key, (Vector2){key.width*2 + 205.0f, 94}, 0.0f, 0.08f, WHITE); //key

        DrawRectangleV(doorPos, {doorScale, doorScale}, RED); //exit
        }

        EndDrawing();
    }
    UnloadMusicStream(music);
    UnloadSound(sound);
    CloseAudioDevice();
    CloseWindow();

    return 0;
}
