#include "SDLManager.h"


SDLManager::SDLManager(){

	video_memory = new pixel_type[SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(pixel_type)];

	video_thread = new thread(&SDLManager::initialize_SDL,this);
	video_thread->detach();

	total_redraws = 0;
	start_time = chrono::steady_clock::now();

	format = SDL_AllocFormat(SDL_PIXELFORMAT_RGB332);

}

SDLManager::~SDLManager(){

	delete[] video_memory;

}

void SDLManager::checkFPS(){
	auto end_time = chrono::steady_clock::now();

	auto totale = chrono::duration_cast<chrono::seconds>(end_time - start_time).count();

	double frames = total_redraws;

	cout<<"Redraws "<<dec<<unsigned(total_redraws)<<endl;

	cout<<"Time "<<dec<<unsigned(totale)<<" s"<<endl;

	cout<<"FPS "<<dec<<frames/totale<<endl;

}

void SDLManager::initialize_SDL(){

	if (SDL_Init(SDL_INIT_VIDEO) != 0){
		cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
		return;
	}


	window = SDL_CreateWindow("Hello World!", 100, 100, SCREEN_WIDTH*2, SCREEN_HEIGHT*2, SDL_WINDOW_SHOWN);
	
	if (window == nullptr){
		cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	
	if (renderer == nullptr){
		SDL_DestroyWindow(window);
		cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return;
	}

	texture = SDL_CreateTexture(renderer,SDL_PIXELFORMAT_RGB332,SDL_TEXTUREACCESS_STREAMING,SCREEN_WIDTH,SCREEN_HEIGHT);

	keyboard_loop();

}

void SDLManager::render_frame(){

	total_redraws++;

	if(texture == nullptr || renderer == nullptr)
		return;

	SDL_UpdateTexture(texture,NULL,video_memory,SCREEN_WIDTH * sizeof(pixel_type));
	SDL_RenderCopy( renderer, texture, NULL, NULL );
	SDL_RenderPresent( renderer );

}


pixel_type* SDLManager::getVideoMemoryPtr(){

	return video_memory;

}

void SDLManager::setCIA1(CIA1* cia1){
	this->cia1 = cia1;
}

SDL_PixelFormat* SDLManager::getPixelFormat(){

	return format;

}


void SDLManager::keyboard_loop(){

	while(true){

		SDL_Event event;
		while( SDL_WaitEvent( &event ) ){

			KeyboardMatrix matrix;

		// We are only worried about SDL_KEYDOWN and SDL_KEYUP events
			switch( event.type ){
				case SDL_KEYDOWN:

				cout<<"Key press detected: ";
				matrix = RowColFromScancode(event.key.keysym.scancode);
				cia1->setKeyPressed(matrix);
				break;

				case SDL_KEYUP:
				cia1->resetKeyPressed();

				printf( "Key release detected\n" );
				break;

				case SDL_QUIT:
				printf("vorrei terminare");
				//loop = false;

				default:
		    	break;
			}
		}

	}

}