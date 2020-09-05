#include "WindowWrapper.h"
#include "_defines.h"

#include "GameHandler.h"

int WindowWrapper::Init() {
	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

#ifdef _DEBUG
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
#else

#endif

	window = SDL_CreateWindow(staticTitle.c_str(), SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED, 1200, 800,
	                          SDL_WINDOW_OPENGL);
	SDL_SetRelativeMouseMode(SDL_TRUE);
	return 0;
}

void WindowWrapper::Reset() {
	delta = 0.0f;
	time  = 0.0f;

	secondProcess = 0.0f;
}

SDL_GLContext WindowWrapper::GetGLContext() const {
	SDL_GLContext tmp = SDL_GL_CreateContext(window);
	SDL_GL_SetSwapInterval(1);
	return tmp;
}

void WindowWrapper::MakeCurrent(const SDL_GLContext context) const {
	SDL_GL_MakeCurrent(window, context);
}

void WindowWrapper::Destroy() const {
	SDL_DestroyWindow(window);
}

Float32 WindowWrapper::Aspect() const {
	int h, w;
	SDL_GetWindowSize(window, &w, &h);
	std::cout << "w: " << w << " h: " << h << " /: " << Float32(w) / Float32(h) << std::endl;
	return Float32(w) / Float32(h);
}

bool WindowWrapper::BeginRenderLoop(GameHandler* game) {
	SDL_Event event;
	auto status_continue = true;

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_QUIT:
				status_continue &= false;
				break;
			case SDL_KEYDOWN:
				if (KEY_DOWN_SET_.find(event.key.keysym.sym) == KEY_DOWN_SET_.end()) {
					KEY_DOWN_SET_.insert(event.key.keysym.sym);

					status_continue &= game->KeyEvent(event.key.keysym.sym, event.key.keysym.mod, true);
				}
				break;
			case SDL_KEYUP:
				if (KEY_DOWN_SET_.find(event.key.keysym.sym) != KEY_DOWN_SET_.end()) {
					KEY_DOWN_SET_.erase(event.key.keysym.sym);

					status_continue &= game->KeyEvent(event.key.keysym.sym, event.key.keysym.mod, false);
				}
				break;
			case SDL_MOUSEMOTION:
				static_cast<GameHandler*>(game)->MouseMove(event.motion.xrel, event.motion.yrel);
				break;
			default:
				break;
		}
	}

	glClearColor(.1f, .1f, .1f, .1f)GL_ERROR
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)GL_ERROR
	return status_continue;
}

void WindowWrapper::EndRenderLoop() {
	SDL_GL_SwapWindow(window);

	const auto endCounter     = SDL_GetPerformanceCounter();
	const auto counterElapsed = endCounter - lastCounter;

	delta = Float32(counterElapsed) / Float32(PERF_COUNTER_FREQUENCY_);

	secondProcess += delta;
	time += delta;

	if (secondProcess > updateFps) {
		secondProcess -= updateFps;
		auto FPS = UInt32(Float32(PERF_COUNTER_FREQUENCY_) / Float32(counterElapsed));

		SDL_SetWindowTitle(window, (staticTitle + " FPS: " + std::to_string(FPS)).c_str());
	}

	lastCounter = endCounter;
}
