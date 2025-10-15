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
		Rectangle getRect(){
			return {position.x, position.y, float(alienImages[type - 1].width), float(alienImages[type - 1].height)};
		};
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
		float GetPositionX(){
			return position.x;
		};
		float GetPositionY(){
			return position.y;
		};
		void SetPosition(Vector2 newPosition){
			position = newPosition;
		}
		void Update (int x_direction, int y_direction){
			position.x += x_direction;
			position.y += y_direction;
		};
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
		Rectangle getRect(){
			return {position.x, position.y, 3, 3};
		};
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
		Rectangle getRect(){
			return {position.x, position.y, 4, 15};
		};
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

		void SetActive(bool activeStatus){
			active = activeStatus;
		};
};

class Spaceship {
	private:
		Texture2D image;
		Vector2 position;
		double lastFireTime;
	public:
		Rectangle getRect(){
			return {position.x, position.y, float(image.width), float(image.height)};
		};
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

		void Reset(){
			// image = LoadTexture("spaceship.png");
			position.x = (GetScreenWidth() - image.width) / 2;
			position.y = (GetScreenHeight() - image.height);
			// lastFireTime = 0.0;
			lasers.clear();
		};
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
		void MoveAliens(){
			for (auto& alien : aliens){
				if (alien.GetPositionX() + alien.alienImages[alien.GetType() - 1].width > GetScreenWidth()){
					aliensDirection = -1;
					MoveDownAliens(4);
				}
				if (alien.GetPositionX() < 0){
					aliensDirection = 1;
					MoveDownAliens(4);
				}
				alien.Update(aliensDirection, 0);	
			}
		};
		void MoveDownAliens(int distance){
			for (auto& alien : aliens){
				alien.Update(0, distance);
			}
		};
		int aliensDirection;
		std::vector<Laser> alienLasers;
		constexpr static float alienLaserShootInterval = 0.55;
		float timeLastAlienFired;
		void AlienShootLaser(){
			double currentTime = GetTime();
			if (currentTime - timeLastAlienFired > alienLaserShootInterval && !aliens.empty()){
				int randomIndex = GetRandomValue(0, aliens.size() - 1);
				Alien& alien = aliens[randomIndex];
				alienLasers.push_back(Laser({alien.GetPositionX() + alien.alienImages[alien.GetType() - 1].width / 2, 
					alien.GetPositionY() + alien.alienImages[alien.GetType() - 1].height}, 5));
				timeLastAlienFired = GetTime();
			}
		};
		void CheckForCollisions(){
			// spaceship lasers
			for (auto& laser: spaceship.lasers){
				auto it = aliens.begin();
				while (it != aliens.end()){
					if (CheckCollisionRecs(it -> getRect(), laser.getRect())){
						it = aliens.erase(it);
						laser.SetActive(false);
					} else {
						++it;
					}
				}

				for (auto& obstacle : obstacles){
					auto itobs = obstacle.blocks.begin();
					while (itobs != obstacle.blocks.end()){
						if (CheckCollisionRecs(itobs -> getRect(), laser.getRect())){
							itobs = obstacle.blocks.erase(itobs);
							laser.SetActive(false);
						} else {
							++itobs;
						}
					}
				}

			}

			// Alien lasers
			for (auto& laser : alienLasers){
				if (CheckCollisionRecs(spaceship.getRect(), laser.getRect())){
					laser.SetActive(false);
					lives -= 1;
					std::cout << lives << std::endl;
					if (lives == 0){
						GameOver();
					}
				}
				for (auto& obstacle : obstacles){
					auto itobs = obstacle.blocks.begin();
					while (itobs != obstacle.blocks.end()){
						if (CheckCollisionRecs(itobs -> getRect(), laser.getRect())){
							itobs = obstacle.blocks.erase(itobs);
							laser.SetActive(false);
						} else {
							++itobs;
						}
					}
				}

			}

			// alien collision with obstacle
			for (auto& alien : aliens){
				for (auto& obstacle : obstacles){
					auto italobs = obstacle.blocks.begin();
					while (italobs != obstacle.blocks.end()){
						if (CheckCollisionRecs(italobs -> getRect(), alien.getRect())){
							italobs = obstacle.blocks.erase(italobs);
						} else {
							++italobs;
						}
					}
				}

				if (CheckCollisionRecs(alien.getRect(), spaceship.getRect())){
					std::cout << "spaceship hittt nooooooo" << std::endl;
					lives = 0;
					GameOver();
				}
			}
		};
		int lives = 3;
		void GameOver(){
			std::cout << "deaddd" << std::endl;
			run = false;
		};
		void Reset(){
			std::cout << "resetting!!" << std::endl;
			spaceship.Reset();
			aliens.clear();
			alienLasers.clear();
			obstacles.clear();
		};
		void InitGame(){
			obstacles = CreateObstacles();
			aliens = CreateAliens();
			aliensDirection = 1;
			timeLastAlienFired = 0.0;
			run = true;
			lives = 3;
		};
	public:
		Game(){
			obstacles = CreateObstacles();
			aliens = CreateAliens();
			aliensDirection = 1;
			timeLastAlienFired = 0.0;
		};

		~Game(){
			Alien::UnloadImages();
		};

		int GetLives(){
			return lives;
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
			for (auto& alienLaser: alienLasers){
				alienLaser.Draw();
			}
		};

		void Update(){
			if (run){
				for (auto& laser: spaceship.lasers){
					laser.Update();
				}
				DeleteInactiveLasers();
				MoveAliens();
				AlienShootLaser();
				for (auto& alienLaser: alienLasers){
					alienLaser.Update();
				}
				CheckForCollisions();
			} else {
				if (IsKeyDown(KEY_ENTER)){
					Reset();
					InitGame();
				}
			}
		};

		void HandleInput(){
			if (run){
				if (IsKeyDown(KEY_LEFT)){
					spaceship.MoveLeft();
				} else if (IsKeyDown(KEY_RIGHT)){
					spaceship.MoveRight();
				} else if (IsKeyDown(KEY_SPACE)){
					spaceship.FireLaser();
				}
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
			for (auto it = alienLasers.begin(); it != alienLasers.end();){
				if (!it -> GetActive()){
					it = alienLasers.erase(it);
				} else {
					++ it;
				}
			}
		}

		bool run = true;

};

int main ()
{
	int offset = 50;
	int windowWidth = 750;
	int windowHeight = 700;

	Color grey = {29, 29, 27, 255};

	// Tell the window to use vsync and work on high DPI displays
	// SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	// Create the window and OpenGL context
	InitWindow(windowWidth + offset, windowHeight + 2 * offset, "Space Invaders!!!");

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

		if (!game.run){
			DrawText("Game over. Press ENTER to restart.", 20, 10, 20, YELLOW);
		};

		for (int i = 0; i < game.GetLives(); i++){
			DrawTextureV(LoadTexture("spaceship.png"), {float(20 + i * 50), 10}, WHITE);
		}

		// update the game
		game.Update();

		// end the frame and get ready for the next one  (display frame, poll input, etc...)
		EndDrawing();
	}

	// destroy the window and cleanup the OpenGL context
	CloseWindow();
	return 0;
}
