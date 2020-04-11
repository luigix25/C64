#pragma once
class SDLManager;

#include "library.h"
#include "vic.h"
#include <SDL2/SDL.h>
#include <thread>

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 200
#define pixel_type uint8_t

class SDLManager{

	public:
		SDLManager();
		void render_frame();
		pixel_type* getVideoMemoryPtr();

	private:
		void initialize_SDL();
		void keyboard_loop();

		thread *video_thread;

		SDL_Window *window;
		SDL_Texture *texture;
		SDL_Renderer *renderer;

		pixel_type *video_memory;
};