#pragma once	  
#include "_defines.h"

class GameHandler;

class WindowWrapper {
	SDL_Window* window                  = nullptr;
	UInt64      PERF_COUNTER_FREQUENCY_ = SDL_GetPerformanceFrequency();
	UInt64      lastCounter             = SDL_GetPerformanceCounter();
	std::string staticTitle             = "xaphante-game-engine";
	Float32     secondProcess           = 0.0f;

#define updateFps 0.3f
public:
	Float32 delta = 0.0f;
	Float64 time  = 0.0f;

	int Init();

	void Reset();

	SDL_GLContext GetGLContext() const;

	void MakeCurrent(const SDL_GLContext context) const;

	void Destroy() const;

	Float32 Aspect() const;

	static bool BeginRenderLoop(GameHandler* game);

	void EndRenderLoop();
};
