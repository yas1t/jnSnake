#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>
#include <time.h>

#define WIN_WIDTH 1280
#define WIN_HEIGHT 736

#define SNAKE_SEG_SIZE 32

#define SNAKE_DIR_UP    1
#define SNAKE_DIR_DOWN  2
#define SNAKE_DIR_LEFT  3
#define SNAKE_DIR_RIGHT 4

typedef struct {
    Rectangle rect;
    Vector2 lastPositions[(1280*720)/32];
    int score;
    int direction;
} snakeHead;

void initSnake(snakeHead *head) {
    head->score = 600;

    head->rect.width = SNAKE_SEG_SIZE;
    head->rect.height = SNAKE_SEG_SIZE;
    
    head->rect.x = WIN_WIDTH/2;
    head->rect.y = WIN_HEIGHT/2;

    
}

void drawSnake(snakeHead *head, int direction, int *count) {
    DrawRectangleRec(head->rect, RAYWHITE);
    head->lastPositions[*count] = (Vector2){head->rect.x, head->rect.y};

    for(int i = 0; i < head->score; i++) {
        DrawRectangle(head->lastPositions[i].x, head->lastPositions[i].y, SNAKE_SEG_SIZE, SNAKE_SEG_SIZE, (i%2==0?RAYWHITE:GREEN));
    }
    if(*count >= head->score) *count = 0;
}

void drawEndScreen() {
    ClearBackground(GRAY);
    DrawText("Game Over", WIN_WIDTH/2, WIN_HEIGHT/2, 64, SKYBLUE);
}

int main(int argc, char *argv[]) {
    InitWindow(WIN_WIDTH, WIN_HEIGHT, "jnSnake");
    int framecount = 0;
    int moveCount = 0;
    bool winState = true;

    time_t t;
    srand((unsigned) time(&t));

    int foodX = rand()%1280;
    int foodY =  rand()%720;

    snakeHead head;
    initSnake(&head);

    SetTargetFPS(60);

    while(!WindowShouldClose()) {
        framecount++;

        switch(GetKeyPressed()) {
            case KEY_W: head.direction = SNAKE_DIR_UP; break;
            case KEY_S: head.direction = SNAKE_DIR_DOWN; break;
            case KEY_A: head.direction = SNAKE_DIR_LEFT; break;
            case KEY_D: head.direction = SNAKE_DIR_RIGHT; break;
            default: break;
        }
        if(framecount % 9 == 0) {
            moveCount++;
            switch(head.direction) {
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

        if(head.rect.x > 1280 || head.rect.x < 0 || head.rect.y > 736 || head.rect.y < 0)
            winState = false;

        BeginDrawing();

        if(head.direction != NULL) {
            drawSnake(&head, head.direction, &moveCount);
            DrawRectangle(foodX, foodY, 32, 32, SKYBLUE);
            if(CheckCollisionRecs((Rectangle){foodX, foodY, 32, 32}, head.rect)) {
                foodX = (rand()%40) * 32;
                foodY = (rand()%23) * 32;
                head.score++;   
            }
            
            ClearBackground(BLACK);
        }

        if(winState == false) {
            drawEndScreen();
            if(framecount % 120 == 0)
                break;
        }
        EndDrawing();
    }

    return 0;
}
