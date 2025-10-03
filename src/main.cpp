#include "raylib.h"
#include "resource_dir.h"	// utility header for SearchAndSetResourceDir
#include <iostream>
#include <vector>

class Laser {
	private:
		Vector2 position;
		int speed;	
		bool active;
	public:
		Laser(Vector2 position, int speed){
			this -> position = position;
			this -> speed = speed;
			active = true;
		};
		void Draw(){
			if (active){
				DrawRectangle(position.x, position.y, 4, 15, {243, 216, 63, 255});
			}
		};
		void Update(){
			position.y += speed;
			if (position.y > GetScreenHeight() || position.y < 0){
				active = false;
			}
		};
		bool GetActive(){
			return active;
		};
};

class Spaceship {
	private:
		Texture2D image;
		Vector2 position;
		double lastFireTime;
	public:
		std::vector<Laser> lasers;
		Spaceship (){
			image = LoadTexture("spaceship.png");
			position.x = (GetScreenWidth() - image.width) / 2;
			position.y = (GetScreenHeight() - image.height);
			lastFireTime = 0.0;
		}

		~Spaceship(){
			UnloadTexture(image);
		}

		void Draw(){
			DrawTextureV(image, position, WHITE);
		}

		void MoveLeft(){
			if (position.x > 0){
				position.x -= 7;
			}
		}
		
		void MoveRight(){
			if (position.x < GetScreenWidth() - image.width){
				position.x += 7;
			}
		}

		void FireLaser(){
			if (GetTime() - lastFireTime > 0.35){
				Laser laser = Laser({position.x + image.width / 2 - 2, position.y - image.height}, -6);
				lasers.push_back(laser);
				lastFireTime = GetTime();
			}
		}
};

class Game {
	private:
		Spaceship spaceship;
	public:
		Game(){};
		~Game(){};
		void Draw(){
			spaceship.Draw();
			for (auto& laser: spaceship.lasers){
				laser.Draw();
			}
		};
		void Update(){
			for (auto& laser: spaceship.lasers){
				laser.Update();
			}
			DeleteInactiveLasers();
			std::cout << spaceship.lasers.size() << std::endl;
		};
		void HandleInput(){
			if (IsKeyDown(KEY_LEFT)){
				spaceship.MoveLeft();
			} else if (IsKeyDown(KEY_RIGHT)){
				spaceship.MoveRight();
			} else if (IsKeyDown(KEY_SPACE)){
				spaceship.FireLaser();
			}
		};
		void DeleteInactiveLasers(){
			for (auto it = spaceship.lasers.begin(); it != spaceship.lasers.end();){
				if (!it -> GetActive()){
					it = spaceship.lasers.erase(it);
				} else {
					++ it;
				}
			}
		}
};

int main ()
{
	int windowWidth = 750;
	int windowHeight = 700;

	Color grey = {29, 29, 27, 255};

	// Tell the window to use vsync and work on high DPI displays
	// SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	// Create the window and OpenGL context
	InitWindow(windowWidth, windowHeight, "Space Invaders!!!");

	// Specify target FPS - if undefined, the computer will run the game as fast as it can so the speed will depend on the computer
	SetTargetFPS(60);

	// Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	SearchAndSetResourceDir("resources");

	// Initialise the instance of the game
	Game game;

	// game loop
	while (!WindowShouldClose())		// run the loop untill the user presses ESCAPE or presses the Close button on the window
	{

		game.HandleInput();
		
		// drawing
		BeginDrawing();

		// Setup the back buffer for drawing (clear color and depth buffers)
		ClearBackground(grey);

		// draw our texture to the screen
		game.Draw();

		// update the game
		game.Update();

		// end the frame and get ready for the next one  (display frame, poll input, etc...)
		EndDrawing();
	}

	// destroy the window and cleanup the OpenGL context
	CloseWindow();
	return 0;
}
