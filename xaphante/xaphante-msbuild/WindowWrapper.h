#pragma once
#include <iostream>
#include <SDL.h>
#include <string>
#include <GL/glew.h>

#include "_defines.h"
#include "../libs/stb_image.h"

class WindowWrapper {
	SDL_Window*   window;
	SDL_GLContext gl_context;
	SDL_version   compiled;
	SDL_version   linked;
	UInt64        PERF_COUNTER_FREQUENCY_ = SDL_GetPerformanceFrequency();
	UInt64        lastCounter             = SDL_GetPerformanceCounter();
	std::string   staticTitle             = "xaphante-game-engine";
	Float32       secondProcess           = 0.0f;

	Float32 updateFps = .1;
public:
	Float32 delta = 0.0f;
	Float64 time  = 0.0f;

	int Init() {
		SDL_Init(SDL_INIT_EVERYTHING);

		SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);


#ifdef _DEBUG
		std::cout << "[DEBUG] Enabled" << std::endl;
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
#else

#endif

		window = SDL_CreateWindow(staticTitle.c_str(), SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED, 1200, 800,
		                          SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

		gl_context = SDL_GL_CreateContext(window);

		const auto err = glewInit();
		if (err != GLEW_OK) {
			std::cout << "Error: " << glewGetErrorString(err) << std::endl;
			std::cin.get();
			return -1;
		}
		std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;


		SDL_VERSION(&compiled);
		SDL_GetVersion(&linked);
		if (compiled.major != linked.major || compiled.minor != linked.minor || compiled.patch != linked.patch) {

			printf("We compiled against SDL version %d.%d.%d ...\n",
			       compiled.major, compiled.minor, compiled.patch);
			printf("But we are linking against SDL version %d.%d.%d.\n",
			       linked.major, linked.minor, linked.patch);
		} else {
			printf("We compiled and linking against SDL version %d.%d.%d.\n",
			       linked.major, linked.minor, linked.patch);
		}

		SDL_GL_SetSwapInterval(1);

		stbi_set_flip_vertically_on_load(true);
		return 0;
	}

	static void BeginLoop() {
		glClearColor(.1f, .1f, .1f, .1f)GL_ERROR
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)GL_ERROR
	}

	bool EndLoop() {
		SDL_GL_SwapWindow(window);

		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				return false;
			}
		}

		const auto endCounter     = SDL_GetPerformanceCounter();
		const auto counterElapsed = endCounter - lastCounter;
		delta                     = Float32(counterElapsed) / Float32(PERF_COUNTER_FREQUENCY_);
		secondProcess += delta;
		time += delta;

		if (secondProcess > updateFps) {
			secondProcess -= updateFps;
			auto FPS = UInt32(Float32(PERF_COUNTER_FREQUENCY_) / Float32(counterElapsed));


			SDL_SetWindowTitle(window, (staticTitle + " FPS: " + std::to_string(FPS)).c_str());
		}

		lastCounter = endCounter;
		return true;
	}

};
