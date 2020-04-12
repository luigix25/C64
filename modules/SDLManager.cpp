#include "SDLManager.h"


SDLManager::SDLManager(){

	video_memory = new pixel_type[SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(pixel_type)];

	video_thread = new thread(&SDLManager::initialize_SDL,this);
	video_thread->detach();

	//funzione();
}

SDLManager::~SDLManager(){

	delete[] video_memory;

}


void SDLManager::initialize_SDL(){

	cout<<"sono un altro thread"<<endl;

	if (SDL_Init(SDL_INIT_VIDEO) != 0){
		cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
		return;
	}


	window = SDL_CreateWindow("Hello World!", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	
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

	SDL_RenderSetScale(renderer, 2, 2);
	texture = SDL_CreateTexture(renderer,SDL_PIXELFORMAT_RGB332,SDL_TEXTUREACCESS_STREAMING,SCREEN_WIDTH,SCREEN_HEIGHT);


	keyboard_loop();

}

void SDLManager::render_frame(){

	if(texture == nullptr || renderer == nullptr)
		return;

	SDL_UpdateTexture(texture,NULL,video_memory,SCREEN_WIDTH * sizeof(pixel_type));
	SDL_RenderCopy( renderer, texture, NULL, NULL );
	SDL_RenderPresent( renderer );

}


pixel_type* SDLManager::getVideoMemoryPtr(){

	return video_memory;

}



void SDLManager::keyboard_loop(){

	while(true){

		SDL_Event event;
		while( SDL_WaitEvent( &event ) ){

		// We are only worried about SDL_KEYDOWN and SDL_KEYUP events
			switch( event.type ){
				case SDL_KEYDOWN:
				cout<<"Key press detected: ";
				cout<<hex<<unsigned(event.key.keysym.scancode)<<endl;
				if(event.key.keysym.scancode == SDL_SCANCODE_A)
					cout<<"premuto A"<<endl;
				break;

				case SDL_KEYUP:
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