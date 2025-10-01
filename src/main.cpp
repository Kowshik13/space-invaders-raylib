/*
Raylib example file.
This is an example main file for a simple raylib project.
Use this as a starting point or replace it with your code.

by Jeffery Myers is marked with CC0 1.0. To view a copy of this license, visit https://creativecommons.org/publicdomain/zero/1.0/

*/

#include "raylib.h"

#include "resource_dir.h"	// utility header for SearchAndSetResourceDir

class Spaceship {
	private:
		Texture2D image;
		Vector2 position;
	public:
		Spaceship (){
			image = LoadTexture("spaceship.png");
			position.x = 100;
			position.y = 100;
		}
		~Spaceship(){
			UnloadTexture(image);
		}
		void Draw(){
			DrawRectangleV(position, {50, 50}, BLUE); 
			DrawText("wowHello Raylib", 500,200,20,WHITE);
			DrawTextureV(image, position, WHITE);
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

	// Load a texture from the resources directory
	Texture wabbit = LoadTexture("wabbit_alpha.png");
	Spaceship spaceship;
	
	// game loop
	while (!WindowShouldClose())		// run the loop untill the user presses ESCAPE or presses the Close button on the window
	{
		// drawing
		BeginDrawing();

		// Setup the back buffer for drawing (clear color and depth buffers)
		ClearBackground(grey);

		// draw some text using the default font
		DrawText("Hello Raylib", 200,200,20,WHITE);

		// draw our texture to the screen
		DrawTexture(wabbit, 400, 200, WHITE);
		spaceship.Draw();
		
		// end the frame and get ready for the next one  (display frame, poll input, etc...)
		EndDrawing();
	}

	// cleanup
	// unload our texture so it can be cleaned up
	UnloadTexture(wabbit);

	// destroy the window and cleanup the OpenGL context
	CloseWindow();
	return 0;
}
