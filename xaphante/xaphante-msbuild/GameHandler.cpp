#include "GameHandler.h"
#include "_defines.h"

#include <string>
#include <thread>
#include <chrono>
#include <ctime>

#include <windows.h>
#include <psapi.h>

#include "Scene.h"
#include "WindowWrapper.h"
#include <GL/glew.h>
#include <SDL.h>

void GameHandler::GlDebugCallback(GLenum      source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message,
                                  const void* userParam) {
	std::cout << "[Gl Error(" << severity << ")]: " << message << std::endl;
}

void GameHandler::Init() {
	WINDOW_W_ = new WindowWrapper();
	if (WINDOW_W_->Init() != 0) return;

	gl_context = WINDOW_W_->GetGLContext();

	const auto err = glewInit();
	if (err != GLEW_OK) {
		std::cout << "Error: " << glewGetErrorString(err) << std::endl;
		std::cin.get();
		return;
	}
	std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

#ifdef _DEBUG
	glEnable(GL_DEBUG_OUTPUT) GL_ERROR
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS)GL_ERROR
	glDebugMessageCallback(GLDEBUGPROC(GlDebugCallback), nullptr);
#endif

	SDL_VERSION(&compiled);
	SDL_GetVersion(&linked);
	if (compiled.major != linked.major || compiled.minor != linked.minor || compiled.patch != linked.patch) {
		printf("We compiled against SDL version %d.%d.%d ...\n", compiled.major, compiled.minor, compiled.patch);
		printf("But we are linking against SDL version %d.%d.%d.\n", linked.major, linked.minor, linked.patch);
	} else {
		printf("We compiled and linking against SDL version %d.%d.%d.\n", linked.major, linked.minor, linked.patch);
	}

	stbi_set_flip_vertically_on_load(true);
}

void GameHandler::LoadScene(std::string* scenePath) {
	RUNNING = false;
	while (!RESET) {
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
	if (scene != nullptr) {
		scene->CleanUp();
	}
	if (WINDOW_W_ != nullptr) {
		WINDOW_W_->Reset();
	}

	scene = new SceneHandler(this);
	scene->Init(scenePath);
}

void GameHandler::Run() {
	while (!EXIT) {
		LoadScene(new std::string("scene/"));
		Loop(scene);
	}
}

void GameHandler::Loop(SceneHandler* handler) {
	RUNNING = true;
	do {
		if (!WINDOW_W_->BeginRenderLoop(this)) {
			EXIT = true;
			return;
		}

		handler->GameLoop(WINDOW_W_->delta);
		handler->Draw();

		WINDOW_W_->EndRenderLoop();
	}
	while (RUNNING);
	RESET = true;
}

void GameHandler::PrintMenAndIndex(int i) {
	PROCESS_MEMORY_COUNTERS_EX pmc;
	GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
	SIZE_T virtualMemUsedByMe = pmc.PrivateUsage;
	std::cout << "[" << i << "]" << " Memory Usage: " << humanSize(virtualMemUsedByMe) << std::endl;
}

void GameHandler::Tests() {
#define num_test 10000

	auto testScene = new std::string("scene/");

	std::cout << "Reset Test" << std::endl;
	for (int i = 0; i < num_test; ++i) {
		if (i % 20 == 0) PrintMenAndIndex(i);
		WINDOW_W_ = new WindowWrapper();
		WINDOW_W_->Init();

		gl_context = WINDOW_W_->GetGLContext();

		WINDOW_W_->BeginRenderLoop(this);
		WINDOW_W_->EndRenderLoop();
		WINDOW_W_->Reset();
		WINDOW_W_->Destroy();
		SDL_GL_DeleteContext(gl_context);

		delete WINDOW_W_;
	}

	std::cout << "Scene Load Test" << std::endl;
	for (int i = 0; i < num_test; ++i) {
		if (i % 20 == 0) PrintMenAndIndex(i);

		scene = new SceneHandler(this);
		scene->Init(testScene);
		WINDOW_W_->BeginRenderLoop(this);
		scene->GameLoop(WINDOW_W_->delta);
		scene->Draw();
		WINDOW_W_->EndRenderLoop();
		scene->CleanUp();
		delete scene;
	}

	PrintMenAndIndex(0);
	std::cout << "End" << std::endl;
	system("pause");
}

void GameHandler::MouseMove(Sint32 xRel, Sint32 yRel) const {
	scene->MouseMove(xRel, yRel);
}

bool GameHandler::KeyEvent(SDL_Keycode key, UInt16 mod, bool down) const {
	return scene->KeyEvent(key, mod, down);
}
