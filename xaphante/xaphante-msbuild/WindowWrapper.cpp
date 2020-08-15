#include "WindowWrapper.h"

int WindowWrapper::Init() {
	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	window = SDL_CreateWindow("xaphante-game-engin", SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED, 1200, 800,
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

	return 0;
}

void WindowWrapper::BeginLoop() {

	glClearColor(.1f, .1f, .1f, .1f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

bool WindowWrapper::EndLoop() const {
	SDL_GL_SwapWindow(window);

	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			return false;
		}
	}

	return true;
}
