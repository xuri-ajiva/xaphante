#include "_defines.h"
#include <iostream>

#define GLEW_STATIC   
#define SDL_MAIN_HANDLED
#include <GL/glew.h>
#include <SDL.h>

#include "SceneHandler.h"
#include "WindowWrapper.h"

#pragma comment(lib, "SDL2.lib")
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "opengl32.lib")

int main(int argc, char** argv) {
	auto* windoW = new WindowWrapper();
	windoW->Init();


	auto* scene = new SceneHandler();
	scene->Init("");

	do {
		windoW->BeginLoop();

		scene->Draw();
	}
	while (windoW->EndLoop());

	std::cout << "Exiting!\n";
}
