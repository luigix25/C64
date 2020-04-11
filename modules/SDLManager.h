#pragma once
class SDLManager;

#include "library.h"
#include "vic.h"
#include <SDL2/SDL.h>

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 200
#define pixel_type uint8_t

class SDLManager{

	public:
		SDLManager();
		~SDLManager();

		void render_frame();
		pixel_type* getVideoMemoryPtr();

	private:
		void initialize_SDL();
		void keyboard_loop();

		thread *video_thread;

		SDL_Window *window 		= nullptr;
		SDL_Texture *texture 	= nullptr;
		SDL_Renderer *renderer 	= nullptr;

		pixel_type *video_memory = nullptr;
};