#define  __XURI__INIT
#define GLEW_STATIC   
#define SDL_MAIN_HANDLED 
#pragma comment(lib, "SDL2.lib")
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "opengl32.lib")

#include <GL/glew.h>
#include <SDL.h>

#include "_defines.h"

#include "SceneHandler.h"
#include "WindowWrapper.h"


void GlDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
	std::cout << "[Gl Error(" << severity << ")]: " << message << std::endl;
}

int main(int argc, char** argv) {

	auto* windW = new WindowWrapper();
	windW->Init();

#ifdef _DEBUG
	glEnable(GL_DEBUG_OUTPUT) GL_ERROR
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS)GL_ERROR
	glDebugMessageCallback(GLDEBUGPROC(GlDebugCallback), nullptr);
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
