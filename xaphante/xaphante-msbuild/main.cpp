#define  __XURI__INIT
#define GLEW_STATIC   
#define SDL_MAIN_HANDLED

#define STB_IMAGE_IMPLEMENTATION
#include "../libs/stb_image.h"
#undef  STB_IMAGE_IMPLEMENTATION

#pragma comment(lib, "SDL2.lib")
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "opengl32.lib")

#include "Scene.h"
#include "_defines.h"

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


	auto* scene = new SceneHandler(windW);
	scene->Init(new std::string("scene/"));

	do {
		windW->BeginLoop();

		scene->Draw(windW);
	}
	while (windW->EndLoop());

	std::cout << "Exiting!\n";
}
