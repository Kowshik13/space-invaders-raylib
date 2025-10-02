#include "raylib.h"
#include "resource_dir.h"	// utility header for SearchAndSetResourceDir

class Spaceship {
	private:
		Texture2D image;
		Vector2 position;
	public:
		Spaceship (){
			image = LoadTexture("spaceship.png");
			position.x = (GetScreenWidth() - image.width) / 2;
			position.y = (GetScreenHeight() - image.height);
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
};

class Game {
	private:
		Spaceship spaceship;
	public:
		Game(){};
		~Game(){};
		void Draw(){
			spaceship.Draw();
		};
		void Update(){};
		void HandleInput(){
			if (IsKeyDown(KEY_LEFT)){
				spaceship.MoveLeft();
			}
			if (IsKeyDown(KEY_RIGHT)){
				spaceship.MoveRight();
			}
		};

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
		
		// end the frame and get ready for the next one  (display frame, poll input, etc...)
		EndDrawing();
	}

	// destroy the window and cleanup the OpenGL context
	CloseWindow();
	return 0;
}
