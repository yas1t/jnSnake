#include <stdlib.h>
#include <raylib.h>
#include <time.h>

#define WIN_WIDTH 1280
#define WIN_HEIGHT 720

#define SNAKE_SEG_SIZE 32

#define SNAKE_DIR_UP    1
#define SNAKE_DIR_DOWN  2
#define SNAKE_DIR_LEFT  3
#define SNAKE_DIR_RIGHT 4

typedef struct {
    Rectangle rect;
    Vector2 lastPositions[(WIN_WIDTH * WIN_HEIGHT) / SNAKE_SEG_SIZE];
    int score;
    int direction;
} snakeHead;

void initSnake(snakeHead* head) {
    head->score = 1;

    head->rect.width = SNAKE_SEG_SIZE - 2;
    head->rect.height = SNAKE_SEG_SIZE - 2;

    head->rect.x = (float) SNAKE_SEG_SIZE * 10 + 1;
    head->rect.y = (float) SNAKE_SEG_SIZE * 6 + 1;
}

void drawSnake(snakeHead* head, int direction, int* count) {
    DrawRectangleRec(head->rect, RAYWHITE);
    head->lastPositions[*count] = (Vector2){ head->rect.x, head->rect.y };

    for (int i = 0; i < head->score; i++) {
        DrawRectangle(
            head->lastPositions[i].x,
            head->lastPositions[i].y,
            SNAKE_SEG_SIZE,
            SNAKE_SEG_SIZE,
            RAYWHITE
        );
    }
    if (*count >= head->score) *count = 0;
}

void drawEndScreen() {
    ClearBackground(GRAY);
    DrawText("Game Over", WIN_WIDTH / 2 - 190, WIN_HEIGHT / 2 - 100, 64, SKYBLUE);
}

int main(int argc, char* argv[]) {
    InitWindow(WIN_WIDTH, WIN_HEIGHT, "jnSnake");
    int framecount = 0;
    int moveCount = 0;
    bool winState = true;

    time_t t;
    srand((unsigned)time(&t));

    int foodX = rand() % WIN_WIDTH / SNAKE_SEG_SIZE;
    int foodY = rand() % WIN_HEIGHT / SNAKE_SEG_SIZE;

    foodX *= SNAKE_SEG_SIZE;
    foodY *= SNAKE_SEG_SIZE;

    snakeHead head;
    initSnake(&head);

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        framecount++;

        switch (GetKeyPressed()) {
            case KEY_W: head.direction = SNAKE_DIR_UP; break;
            case KEY_S: head.direction = SNAKE_DIR_DOWN; break;
            case KEY_A: head.direction = SNAKE_DIR_LEFT; break;
            case KEY_D: head.direction = SNAKE_DIR_RIGHT; break;
            default: break;
        }
        if (framecount % 9 == 0) {
            moveCount++;
            switch (head.direction) {
            case SNAKE_DIR_UP:
                head.rect.y -= SNAKE_SEG_SIZE;
                break;

            case SNAKE_DIR_DOWN:
                head.rect.y += SNAKE_SEG_SIZE;
                break;

            case SNAKE_DIR_LEFT:
                head.rect.x -= SNAKE_SEG_SIZE;
                break;

            case SNAKE_DIR_RIGHT:
                head.rect.x += SNAKE_SEG_SIZE;
                break;

            default: break;
            }
        }

        if (head.rect.x > 1280 || head.rect.x < 0 || head.rect.y > 736 || head.rect.y < 0)
            winState = false;

        BeginDrawing();

        if (head.direction != 0) {
            drawSnake(&head, head.direction, &moveCount);
            DrawRectangle(foodX, foodY, SNAKE_SEG_SIZE, SNAKE_SEG_SIZE, SKYBLUE);
            if (CheckCollisionRecs((Rectangle) { foodX, foodY, SNAKE_SEG_SIZE, SNAKE_SEG_SIZE }, head.rect)) {
                foodX = (rand() % 40) * SNAKE_SEG_SIZE;
                foodY = (rand() % 23) * SNAKE_SEG_SIZE;
                head.score++;
            }

            ClearBackground(BLACK);
        }

        if (winState == false) {
            drawEndScreen();
            if (framecount % 300 == 0) break;
        }
        EndDrawing();
    }

    return 0;
}
