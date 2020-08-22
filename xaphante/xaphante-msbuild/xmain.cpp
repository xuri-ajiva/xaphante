#define  __XURI__INIT
//#define GLEW_STATIC   
#define SDL_MAIN_HANDLED

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#undef  STB_IMAGE_IMPLEMENTATION

#pragma comment(lib, "SDL2.lib")
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "opengl32.lib")
					 
#include "_defines.h"	
#include "GameHandler.h"


int main(int argc, char** argv) {
	auto game = new GameHandler();
	game->Init();

	game->LoadScene(new std::string("scene/"));
	game->Run();

	std::cout << "Exiting!\n";
}
