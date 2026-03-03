#include "raylib.h"
#include <vector>
#include <string>

enum GameState { PLAYING, GAME_OVER };

struct PipePair {
    Rectangle topPipe;
    Rectangle bottomPipe;
    bool passed;
};

int main() {
    const int screenWidth = 400;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "Flappy Bird - C++ Raylib");
    SetTargetFPS(60);

    Texture2D bgTex = LoadTexture("background.png");
    Texture2D baseTex = LoadTexture("base.png");
    Texture2D birdTex = LoadTexture("bird.png");
    Texture2D pipeTex = LoadTexture("pipe.png");

    float baseHeight = (float)baseTex.height;
    float base1X = 0.0f;
    float base2X = (float)baseTex.width;

    Vector2 birdPos = { 100.0f, 300.0f };
    float birdVelocity = 0.0f;
    const float gravity = 0.6f;
    const float jumpStrength = -8.0f;

    const float scrollSpeed = 3.0f;
    const float pipeGap = 150.0f;
    std::vector<PipePair> pipes;
    int spawnTimer = 0;
    GameState state = PLAYING;
    int score = 0;

    while (!WindowShouldClose()) {

        if (IsKeyPressed(KEY_SPACE)) {
            if (state == PLAYING) {
                birdVelocity = jumpStrength;
            }
            else if (state == GAME_OVER) {
                birdPos = { 100.0f, 300.0f };
                birdVelocity = 0.0f;
                pipes.clear();
                score = 0;
                spawnTimer = 0;
                state = PLAYING;
            }
        }

        if (state == PLAYING) {
            birdVelocity += gravity;
            birdPos.y += birdVelocity;

            if (birdPos.y + birdTex.height >= screenHeight - baseHeight) {
                state = GAME_OVER;
                birdPos.y = screenHeight - baseHeight - birdTex.height;
            }
            if (birdPos.y < 0) state = GAME_OVER;

            base1X -= scrollSpeed;
            base2X -= scrollSpeed;
            if (base1X + baseTex.width <= 0) base1X = base2X + baseTex.width;
            if (base2X + baseTex.width <= 0) base2X = base1X + baseTex.width;

            spawnTimer++;
            if (spawnTimer > 90) {
                spawnTimer = 0;
                float minHeight = 50.0f;
                float maxHeight = screenHeight - baseHeight - pipeGap - minHeight;
                float randomY = minHeight + GetRandomValue(0, (int)(maxHeight - minHeight));

                PipePair newPipe;
                newPipe.topPipe = { 450.0f, randomY - pipeTex.height, (float)pipeTex.width, (float)pipeTex.height };
                newPipe.bottomPipe = { 450.0f, randomY + pipeGap, (float)pipeTex.width, (float)pipeTex.height };
                newPipe.passed = false;
                pipes.push_back(newPipe);
            }

            Rectangle birdRec = { birdPos.x, birdPos.y, (float)birdTex.width, (float)birdTex.height };

            for (size_t i = 0; i < pipes.size(); i++) {
                pipes[i].topPipe.x -= scrollSpeed;
                pipes[i].bottomPipe.x -= scrollSpeed;

                if (CheckCollisionRecs(birdRec, pipes[i].topPipe) || CheckCollisionRecs(birdRec, pipes[i].bottomPipe)) {
                    state = GAME_OVER;
                }

                if (!pipes[i].passed && birdPos.x > pipes[i].bottomPipe.x + pipes[i].bottomPipe.width) {
                    score++;
                    pipes[i].passed = true;
                }
            }

            if (!pipes.empty() && pipes[0].bottomPipe.x + pipes[0].bottomPipe.width < 0) {
                pipes.erase(pipes.begin());
            }
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawTexture(bgTex, 0, 0, WHITE);

        for (const auto& p : pipes) {
            Rectangle sourceRec = { 0.0f, 0.0f, (float)pipeTex.width, -(float)pipeTex.height };
            Vector2 topPos = { p.topPipe.x, p.topPipe.y };
            DrawTextureRec(pipeTex, sourceRec, topPos, WHITE);

            DrawTexture(pipeTex, (int)p.bottomPipe.x, (int)p.bottomPipe.y, WHITE);
        }

        DrawTexture(baseTex, (int)base1X, (int)(screenHeight - baseHeight), WHITE);
        DrawTexture(baseTex, (int)base2X, (int)(screenHeight - baseHeight), WHITE);

        DrawTexture(birdTex, (int)birdPos.x, (int)birdPos.y, WHITE);

        DrawText(TextFormat("Skor: %d", score), 10, 10, 40, BLACK);

        if (state == GAME_OVER) {
            DrawText("OYUN BITTI!", 80, 250, 40, RED);
            DrawText("Tekrar icin SPACE", 60, 300, 30, BLACK);
        }

        EndDrawing();
    }

    UnloadTexture(bgTex);
    UnloadTexture(baseTex);
    UnloadTexture(birdTex);
    UnloadTexture(pipeTex);
    CloseWindow();

    return 0;
}