#pragma once
class SDLManager;

#include "library.h"
#include "vic.h"
#include "cia1.h"

#include <SDL2/SDL.h>

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 200
#define pixel_type uint8_t

#define KEYBOARD_COL_ADDR 0xDC00
#define KEYBOARD_ROW_ADDR 0xDC01

class SDLManager{

	public:
		SDLManager();
		~SDLManager();


		void setCIA1(CIA1*);
		pixel_type* getVideoMemoryPtr();
		SDL_PixelFormat* getPixelFormat();

		void render_frame();

		void checkFPS();

	private:
		void initialize_SDL();
		void keyboard_loop();


		CIA1 *cia1;

		thread *video_thread;

		SDL_Window *window 		= nullptr;
		SDL_Texture *texture 	= nullptr;
		SDL_Renderer *renderer 	= nullptr;
		SDL_PixelFormat *format = nullptr;

		pixel_type *video_memory = nullptr;

		//DEBUG

		uint64_t total_redraws;
		chrono::time_point<chrono::steady_clock> start_time;

};