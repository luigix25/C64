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


struct KeyboardMatrix{

	uint8_t row;
	uint8_t col;
	bool shift = false;

};

class SDLManager{

	public:
		SDLManager();
		~SDLManager();

		pixel_type* getVideoMemoryPtr();
		SDL_PixelFormat* getPixelFormat();

		void render_frame();

		void checkFPS();
		uint8_t getRowForCol(uint8_t);

	private:
		void initialize_SDL();
		void keyboard_loop();

		void terminate();
		
		KeyboardMatrix RowColFromScancode(uint16_t);

		thread *video_thread;

		SDL_Window *window 		= nullptr;
		SDL_Texture *texture 	= nullptr;
		SDL_Renderer *renderer 	= nullptr;
		SDL_PixelFormat *format = nullptr;

		uint8_t keyboard_matrix[8][8];
		pixel_type *video_memory = nullptr;

		//DEBUG

		uint64_t total_redraws;
		chrono::time_point<chrono::steady_clock> start_time;

};