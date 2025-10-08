#include "raylib.h"
#include "resource_dir.h"	// utility header for SearchAndSetResourceDir
#include <iostream>
#include <vector>

// Texture2D alienImages[3] = {
// 	LoadTexture("alien_1.png")
// 	, LoadTexture("alien_2.png")
// 	, LoadTexture("alien_3.png")
// };

class Alien {
	private:
		Texture2D image;
		int number;
		int type;
		Vector2 position;
	public:
		static Texture2D alienImages[3];
		Alien (int type, Vector2 position){
			this -> type = type;
			this -> position = position;
			switch (type){
				case 1: 
					alienImages[0] = LoadTexture("alien_1.png");
					break;
				case 2: 
					alienImages[1] = LoadTexture("alien_2.png");
					break;
				case 3: 
					alienImages[2] = LoadTexture("alien_3.png");
					break;
				default: 
					alienImages[0] = LoadTexture("alien_1.png");
					break;
			}
		};
		int GetType(){
			return type;
		};
		void Update (){};
		void Draw (){
			DrawTextureV(alienImages[type - 1], position, WHITE);
		};
		static void UnloadImages(){
			std::cout << "unloaded images" << std::endl;
			for (int i = 0; i < 4; i++){
				UnloadTexture(alienImages[i]);
			}
		};
};
Texture2D Alien::alienImages[3] = {};

class Block {
	private: 
		Vector2 position;
	public:
		Block(Vector2 position){
			this -> position = position;
		};
		void Draw(){
			DrawRectangle(position.x, position.y, 3, 3, {243, 216, 63, 255});
		};

};

class Obstacle {
	private:
		Vector2 position;
	public:
		std::vector<Block> blocks;
		static std::vector<std::vector<int>> grid;
		// static std::vector<std::vector<int>> grid = {
		// 		{0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0},
		// 		{0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0},
		// 		{0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
		// 		{0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
		// 		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		// 		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		// 		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		// 		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		// 		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		// 		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		// 		{1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1},
		// 		{1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1},
		// 		{1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1}
		// 	};
		Obstacle (Vector2 position){
			this -> position = position;
			for (unsigned int row = 0; row < grid.size(); ++row){
				for (unsigned int column = 0; column < grid[0].size(); ++column){
					if (grid[row][column] == 1){
						float pos_x = position.x + 3 * column;
						float pos_y = position.y + 3 * row;
						Block block = Block({pos_x, pos_y});
						blocks.push_back(block);
					}
				}
			}
		};

		void Draw(){
			for (auto& block: blocks){
				block.Draw();
			}
		};
};

std::vector<std::vector<int>> Obstacle::grid = {
		{0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0},
		{0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0},
		{0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
		{0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		{1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1},
		{1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1},
		{1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1}
	};


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
		std::vector<Obstacle> obstacles;
		std::vector<Obstacle> CreateObstacles(){
			int obstacleWidth = Obstacle::grid[0].size() * 3;
			float gap = (GetScreenWidth() - 4 * obstacleWidth) / 5;
			for (int i = 0; i < 4; i++){
				float offsetx = (i + 1) * gap + i * obstacleWidth;
				obstacles.push_back(Obstacle({offsetx, float(GetScreenHeight() - 100)}));
			};
			return obstacles;
		};
		std::vector<Alien> aliens;
		std::vector<Alien> CreateAliens(){
			std::vector<Alien> aliensVector;
			for (int row = 0; row < 5; row++){
				int alienType;
				if (row == 0){
					alienType = 1;
				} else if (row == 1 || row == 2){
					alienType = 2;
				} else {
					alienType = 3;
				};
				for (int column = 0; column < 11; column++){
					float x = 75 + column * 55;
					float y = 110 + row * 55;
					aliensVector.push_back(Alien(alienType, {x, y}));
				};
			};
			return aliensVector;
		};
	public:
		Game(){
			obstacles = CreateObstacles();
			aliens = CreateAliens();
		};

		~Game(){
			Alien::UnloadImages();
		};

		void Draw(){
			spaceship.Draw();
			for (auto& laser: spaceship.lasers){
				laser.Draw();
			}
			for (auto& obstacle: obstacles){
				obstacle.Draw();
			}
			for (auto& alien: aliens){
				alien.Draw();
			}
		};

		void Update(){
			for (auto& laser: spaceship.lasers){
				laser.Update();
			}
			DeleteInactiveLasers();
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

	Texture2D oneAlien = LoadTexture("alien_1.png");

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
