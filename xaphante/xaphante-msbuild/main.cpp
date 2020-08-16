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

void GlDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
	std::cout << "[Gl Error(" << severity << ")]: " << message << std::endl;
}

int main(int argc, char** argv) {


	auto* windW = new WindowWrapper();
	windW->Init();

#ifdef _DEBUG
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(GLDEBUGPROC(GlDebugCallback), 0);
#endif


	auto* scene = new SceneHandler();
	scene->Init("");

	do {
		windW->BeginLoop();

		scene->Draw();
	}
	while (windW->EndLoop());

	std::cout << "Exiting!\n";
}
