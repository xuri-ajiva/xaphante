#pragma once
#include <SDL.h>
#include <string>

#include "_defines.h"
class SceneHandler;
class WindowWrapper;

class GameHandler {
	SceneHandler*  scene {};

	bool RESET = true;
	bool EXIT  = false;

	SDL_GLContext gl_context {};
	SDL_version   compiled {};
	SDL_version   linked {};

	static void PrintMenAndIndex(int i);

	static void GlDebugCallback(GLenum      source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message,
	                            const void* userParam);

public:	
	WindowWrapper* WINDOW_W_ {};
	
	void Init();

	void LoadScene(std::string* scenePath);

	void Run();

	void Loop(SceneHandler* handler);

	void Tests();

	void MouseMove(Sint32 xRel, Sint32 yRel) const;

	bool RUNNING = false;

	bool KeyEvent(SDL_Keycode key, UInt16 mod, bool down) const;
};
