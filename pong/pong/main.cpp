#include "raylib.h"
#include <stdlib.h>
#include <iostream>
#include <sstream>


using namespace std;
int left_score = 0;
int right_score = 0; 
int screen_width = GetScreenWidth();
Color Red = Color{ 220,20,60 };
Color Blue = Color{ 0, 71, 171 };
Color lightBlue = Color{ 93, 63, 211 };
Color lightRed = Color{ 240,128,128 };


struct Ball {
	float x, y;
	float speedX, speedY;
	float radius;
	void resetball() {
		x = GetScreenWidth() / 2;
		y = GetScreenHeight() / 2;

	}

	void Draw() {
		DrawCircle((int)x, (int)y, radius, WHITE);
	}
};


struct Paddle{
	float x, y;
	float speed;
	float width, height;
	float score; 
	float sfx;
	Rectangle GetRect() {
		return Rectangle{ x - width / 2, y - height / 2, 10, 100 };
	}
		 
	void draw() {
		DrawRectangleRec(GetRect(), WHITE);
	}
};


int main() {
	InitWindow(800, 600, "Pong");
	SetWindowState(FLAG_VSYNC_HINT);
	InitAudioDevice();
	
	Sound paddle_sfx =  LoadSound("./assets/paddle.wav");
	Sound wall_sfx = LoadSound("./assets/wall.wav");
	Sound score_sfx = LoadSound("./assets/score.wav");

	Texture2D background = LoadTexture("./assets/pong_background_2.png");                                                       // Load texture from file into GPU memory (VRAM)

	

	Ball ball;
	ball.x = GetScreenWidth() / 2.0f;
	ball.y = GetScreenHeight() / 2.0f;
	ball.radius = 5;
	ball.speedX = 250;
	ball.speedY = 250;

	Paddle leftPaddle;
	leftPaddle.x = 50;
	leftPaddle.y = GetScreenHeight() / 2;
	leftPaddle.width = 10;
	leftPaddle.height = 100;
	leftPaddle.speed = 500;
	leftPaddle.score = 0;

	Paddle rightPaddle; 
	rightPaddle.x = GetScreenWidth() - 50;
	rightPaddle.y = GetScreenHeight() / 2 - 50;
	rightPaddle.width = 10;
	rightPaddle.height = 100;
	rightPaddle.speed = 500;
	rightPaddle.score = 0;

	const char* winnerText = nullptr;


	while (!WindowShouldClose()) {

		ball.x += ball.speedX * GetFrameTime();
		ball.y += ball.speedY * GetFrameTime();

		if (ball.y < 0) {
			ball.y = 0;
			ball.speedY*= -1;
		}

		if (ball.y > GetScreenHeight()) {
			ball.y = GetScreenHeight();
			ball.speedY*= -1;
		}

		if (IsKeyDown(KEY_W)) {
			leftPaddle.y -= leftPaddle.speed * GetFrameTime();
		}

		if (IsKeyDown(KEY_S)) {
			leftPaddle.y += leftPaddle.speed * GetFrameTime();
		}
		if (leftPaddle.y < 0) {
			leftPaddle.y = 1;
		}

		if (leftPaddle.y > GetScreenHeight()) {
			leftPaddle.y = GetScreenHeight();
		}
		if (rightPaddle.y < 0) {
			rightPaddle.y = 1;
		}

		if (rightPaddle.y > GetScreenHeight()) {
			rightPaddle.y = GetScreenHeight();
		}



		if (IsKeyDown(KEY_UP)) {
			rightPaddle.y -= leftPaddle.speed * GetFrameTime();
		}

		if (IsKeyDown(KEY_DOWN)) {
			rightPaddle.y += leftPaddle.speed * GetFrameTime();
		}


		if (CheckCollisionCircleRec(Vector2{ ball.x, ball.y }, ball.radius,
			leftPaddle.GetRect())) {
			if (ball.speedX < 0) {
				ball.speedX *= -1.1f;
				ball.speedY = (ball.y - leftPaddle.y) / (leftPaddle.height / 2) * ball.speedX;
				PlaySound(paddle_sfx);
			}
			
		}


		if (CheckCollisionCircleRec(Vector2{ ball.x, ball.y }, ball.radius,
			rightPaddle.GetRect())) {
			if (ball.speedX > 0) {
				ball.speedX *= -1.1f;
				ball.speedY = (ball.y - rightPaddle.y) / (rightPaddle.height / 2) * - ball.speedX;
				PlaySound(paddle_sfx);
			}
		}


		if (ball.x < 0) {
			winnerText = "right player wins!";
			DrawText("Restart?     Press (R)", GetScreenWidth() / 2.0 - 150, GetScreenHeight() - 180, 23, GREEN);
			right_score ++;
			PlaySound(score_sfx);

		}
		if (ball.x > GetScreenWidth()) {
			winnerText = "left player wins!";
			DrawText("Restart?     Press (R)", GetScreenWidth() / 2.0 - 150, GetScreenHeight() - 180, 23,GREEN);
			left_score++;
			PlaySound(score_sfx);

	}
		if (winnerText) {
			ball.y = GetScreenHeight() / 2 + 80; 
			ball.x = GetScreenWidth() / 2;
			DrawText("Restart?     Press (R)", GetScreenWidth() / 2.0 - 150, GetScreenHeight() - 180, 23, GREEN);
		}
		if (winnerText && IsKeyPressed(KEY_R)) {
			ball.x = GetScreenWidth() / 2;
			ball.y = GetScreenHeight() / 2;
			ball.speedX = 300; 
			ball.speedY = 300; 
			winnerText = nullptr;
		}
		BeginDrawing();
		DrawRectangle(GetScreenWidth() / 2, 0, GetScreenWidth(), GetScreenHeight(), Red);
			ClearBackground(WHITE);
			DrawTexture(background, 0,0, WHITE);
			
		ball.Draw();
		leftPaddle.draw();
		rightPaddle.draw();
		DrawText(TextFormat("%i", right_score), GetScreenWidth() - 220, 20, 80, WHITE);
		DrawText(TextFormat("%i", left_score), GetScreenWidth() / 4 - 20, 20, 80, WHITE);
		if (winnerText) {
			int textWidth = MeasureText(winnerText, 60);
			DrawText(winnerText, GetScreenWidth() / 2 - textWidth / 2, GetScreenHeight() / 2 - 30, 60, YELLOW);
		}
			
		DrawFPS(10, 10);
		EndDrawing();
	
		

		

	}
	// unload

	UnloadTexture(background);
	UnloadSound(wall_sfx);
	UnloadSound(paddle_sfx);
	UnloadSound(score_sfx);
	CloseWindow();
	return 0;
}



