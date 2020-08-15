#pragma once
#include <cstdio>
#include <iostream>
#include <SDL.h>
#include <GL/glew.h>

class WindowWrapper {
	SDL_Window*   window;
	SDL_GLContext gl_context;
	SDL_version   compiled;
	SDL_version   linked;
public:
	int Init();

	static void BeginLoop();

	bool EndLoop() const;
};
