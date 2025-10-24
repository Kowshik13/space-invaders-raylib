# Space Invaders

Classic space invaders game built on C++ using the Raylib library.

- Left and right arrow keys to move the spaceship.
- Spacebar to shoot lasers.
- You have three lives. 
- Press Enter to restart. 

I followed closely the [tutorial video](https://youtu.be/TGo3Oxdpr5o?si=xQBBFQfa_mX7JzX-) by [Programming With Nick](https://www.youtube.com/@programmingwithnick).

## Technologies
* **Raylib** for game loop

## Building and running

The repository bundles a [Premake](https://premake.github.io/) setup that will download Raylib and generate the native project files for you. The process differs slightly between Windows and macOS – follow the platform specific guide below.

### Windows

You can use either Visual Studio or MinGW-w64. Both paths will build the game into `bin/Debug/space-invaders-raylib.exe` (or the `Release` folder if you choose a release configuration).

#### Option A – Visual Studio 2022 (recommended)

1. Install **Visual Studio 2022** with the *Desktop development with C++* workload.
2. Open a *Developer PowerShell* or *Developer Command Prompt for VS 2022* and `cd` into the project directory.
3. Run `build-VisualStudio2022.bat`. This calls Premake to generate a `space-invaders-raylib.sln` file in the repository root.
4. Open the generated solution in Visual Studio, choose the `Debug` (or `Release`) configuration you want, and build the solution (Build ▸ Build Solution).
5. Press *Local Windows Debugger* (or `Ctrl+F5`) to launch the game, or run the built executable from `bin/Debug/space-invaders-raylib.exe`.

#### Option B – MinGW-w64 (MSYS2)

1. Install [MSYS2](https://www.msys2.org/) and open the *MSYS2 MinGW 64-bit* shell.
2. Install the toolchain and make by running `pacman -S mingw-w64-x86_64-toolchain mingw-w64-x86_64-make`.
3. Inside that same shell, `cd` into the project directory and execute `build-MinGW-W64.bat`. The script downloads Raylib (if needed) and generates GNU Makefiles.
4. Still in the MinGW shell, build the project with `mingw32-make config=debug_x64` (or `release_x64`).
5. Launch the game with `bin/Debug/space-invaders-raylib.exe` (or the release variant).

> **Tip:** If Windows blocks the `.bat` scripts, right-click the file ▸ *Properties* ▸ *Unblock* before running it.

### macOS

1. Install the Xcode Command Line Tools (`xcode-select --install`).
2. Ensure you have a recent version of make (macOS ships one by default) and optionally install Homebrew's `make` if you prefer (`brew install make`).
3. Grant execute permission to the bundled Premake binary once: `chmod +x build/premake5.osx`.
4. Generate the GNU Make project files by running `./build/premake5.osx gmake2` from the repository root (the command will download Raylib sources on the first run).
5. Build the game with `make config=debug_x64` (or `config=release_x64`) from the project root.
6. Run the app with `./bin/Debug/space-invaders-raylib` (or the release executable).

If you want to build a `.app` bundle later, you can open the generated Xcode project under `build/build_files` or integrate with your own toolchain.

## Future improvements

- Construct datamodel: Right now all logic is stored in one file. It is much readable and manageable to construct a data model. That way the files are separated out in a more logical manner. I tried doing this, however, I came across issues in the compilation. I think there is an issue with how the Raylib library was set up. 
- Set up more levels so that the aliens move increasingly faster as we progress.
- Set up scores (e.g., gain scores when killing aliens).
- Set up leaderboards. 
- Display game beat message.

## Resources
* Raylib - https://www.raylib.com/
