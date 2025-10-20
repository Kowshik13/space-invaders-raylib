# Space Invaders

Classic space invaders game built on C++ using the Raylib library.

- Left and right arrow keys to move the spaceship.
- Spacebar to shoot lasers.
- You have three lives. 
- Press Enter to restart. 

I followed closely the [tutorial video](https://youtu.be/TGo3Oxdpr5o?si=xQBBFQfa_mX7JzX-) by [Programming With Nick](#https://www.youtube.com/@programmingwithnick).

## Technologies
* **Raylib** for game loop

## How to set it up for developement

The logic of the game is all coded in one file `src/main.cpp`. Raylib is available on most modern OS including Windows, MacOS, and Linux. You can find the setup instruction for each in the [Raylib website](#https://www.raylib.com/). For MacOS (not sure about for others), it directs me to the [`Raylib-Quickstart` GitHub repository](#https://github.com/raylib-extras/raylib-quickstart). For the VS code users, there is a special instruction. 
It might be worth pointing out that even following the instruction, I could not set it up as instructued. It seemed to me that there is an issue finding the Raylib library by the VS Code. So I followed the instruction written for the `MacOS Users`.  

## How to run the game

This GitHub repo alreay has the executable compiled so the first two steps will not be necessary unless you are developping and making changes in the source file (`src/main.cpp`).

1. In the terminal, navigate to the project folder. 
2. Run `make` compile the project. This create an executable. 
3. Navigate to `bin/Debug`. This is where the executable is stored in.
4. Run `./space-invaders-raylib`. 

## Future improvements

- Construct datamodel: Right now all logic is stored in one file. It is much readable and manageable to construct a data model. That way the files are separated out in a more logical manner. I tried doing this, however, I came across issues in the compilation. I think there is an issue with how the Raylib library was set up. 
- Set up more levels so that the aliens move increasingly faster as we progress.
- Set up scores (e.g., gain scores when killing aliens).
- Set up leaderboards. 

## Resources
* Raylib - https://www.raylib.com/
