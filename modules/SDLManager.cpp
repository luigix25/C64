#include "SDLManager.h"


SDLManager::SDLManager(){

	video_memory = new pixel_type[SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(pixel_type)];

	video_thread = new thread(&SDLManager::initialize_SDL,this);
	video_thread->detach();

	total_redraws = 0;
	start_time = chrono::steady_clock::now();

	format = SDL_AllocFormat(SDL_PIXELFORMAT_RGB332);

	memset(&keyboard_matrix[0],0xFF,64);

	getRowForCol(0);

}

SDLManager::~SDLManager(){

	delete[] video_memory;
	SDL_FreeFormat(format);

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

SDL_PixelFormat* SDLManager::getPixelFormat(){

	return format;

}


void SDLManager::terminate(){

	SDL_DestroyRenderer(renderer);
	SDL_DestroyTexture(texture);
	SDL_DestroyWindow(window);

	SDL_Quit();

	exit(-1);

}


void SDLManager::keyboard_loop(){

	while(true){

		KeyboardMatrix matrix;

		SDL_Event event;
		while( SDL_WaitEvent( &event ) ){

		// We are only worried about SDL_KEYDOWN and SDL_KEYUP events
			switch( event.type ){
				case SDL_KEYDOWN:
					cout<<"Key press detected: \n";
					matrix = RowColFromScancode(event.key.keysym.scancode);
					keyboard_matrix[matrix.row][matrix.col] = 0x00;
			
					break;

				case SDL_KEYUP:
					cout<<"Key release detected\n";
					matrix = RowColFromScancode(event.key.keysym.scancode);
					keyboard_matrix[matrix.row][matrix.col] = 0xFF;

					break;

				case SDL_QUIT:
					cout<<"Terminating\n";
					terminate();
					//loop = false;
					break;

				default:
		    		break;
			}



		}

	}

}


uint8_t SDLManager::getRowForCol(uint8_t col){

	uint8_t result = 0xFF;

	for(int i=0;i<8;i++){
		for(int j=0;j<8;j++){
			if(GET_I_BIT(col,j) == 1)
				continue;
			
			if(GET_I_BIT(result,i) == 1 and keyboard_matrix[i][j] == 0)
				RESET_I_BIT(result,i);
		}
	}

	return result;

}

KeyboardMatrix SDLManager::RowColFromScancode(uint16_t code){

	KeyboardMatrix matrix;

	switch(code){
		case SDL_SCANCODE_A:
			matrix.col = 1;
			matrix.row = 2;
			break;

		case SDL_SCANCODE_B:
			matrix.col = 3;
			matrix.row = 4;
			break;

		case SDL_SCANCODE_C:
			matrix.col = 2;
			matrix.row = 4;
			break;

		case SDL_SCANCODE_D:	
			matrix.col = 2;
			matrix.row = 2;
			break;
		case SDL_SCANCODE_E:
			matrix.col = 1;
			matrix.row = 6;
			break;
		case SDL_SCANCODE_F:
			matrix.col = 2;
			matrix.row = 5;
			break;

		case SDL_SCANCODE_G:
			matrix.col = 3;
			matrix.row = 2;
			break;

		case SDL_SCANCODE_H:
			matrix.col = 3;
			matrix.row = 5;
			break;
		case SDL_SCANCODE_I:
			matrix.col = 4;
			matrix.row = 1;
			break;

		case SDL_SCANCODE_J:
			matrix.col = 4;
			matrix.row = 2;
			break;		
		case SDL_SCANCODE_K:
			matrix.col = 4;
			matrix.row = 5;
			break;		
		case SDL_SCANCODE_L:
			matrix.col = 5;
			matrix.row = 2;
			break;		
		case SDL_SCANCODE_M:
			matrix.col = 4;
			matrix.row = 4;
			break;		
		case SDL_SCANCODE_N:
			matrix.col = 4;
			matrix.row = 7;
			break;			
		case SDL_SCANCODE_O:
			matrix.col = 4;
			matrix.row = 6;
			break;		
		case SDL_SCANCODE_P:
			matrix.col = 5;
			matrix.row = 1;
			break;

		case SDL_SCANCODE_Q:
			matrix.col = 7;
			matrix.row = 6;
			break;
					
		case SDL_SCANCODE_R:
			matrix.col = 2;
			matrix.row = 1;
			break;
		
		case SDL_SCANCODE_S:
			matrix.col = 1;
			matrix.row = 5;
			break;
		
		case SDL_SCANCODE_T:
			matrix.col = 2;
			matrix.row = 6;
			break;		

		case SDL_SCANCODE_U:
			matrix.col = 3;
			matrix.row = 6;
			break;		

		case SDL_SCANCODE_V:
			matrix.col = 3;
			matrix.row = 7;
			break;	

		case SDL_SCANCODE_W:
			matrix.col = 1;
			matrix.row = 1;
			break;

		case SDL_SCANCODE_X:
			matrix.col = 2;
			matrix.row = 7;
			break;	

		case SDL_SCANCODE_Y:
			matrix.col = 3;
			matrix.row = 1;
			break;	

		case SDL_SCANCODE_Z:
			matrix.col = 1;
			matrix.row = 4;
			break;

		case SDL_SCANCODE_0:
			matrix.col = 4;
			matrix.row = 3;
			break;

		case SDL_SCANCODE_1:
			matrix.col = 7;
			matrix.row = 0;
			break;
		
		case SDL_SCANCODE_2:
			matrix.col = 7;
			matrix.row = 3;
			break;

		case SDL_SCANCODE_3:
			matrix.col = 1;
			matrix.row = 0;
			break;
		
		case SDL_SCANCODE_4:
			matrix.col = 1;
			matrix.row = 3;
			break;

		case SDL_SCANCODE_5:
			matrix.col = 2;
			matrix.row = 0;
			break;

		case SDL_SCANCODE_6:
			matrix.col = 2;
			matrix.row = 3;
			break;

		case SDL_SCANCODE_7:
			matrix.col = 3;
			matrix.row = 0;
			break;

		case SDL_SCANCODE_8:
			matrix.col = 3;
			matrix.row = 3;
			break;

		case SDL_SCANCODE_9:
			matrix.col = 4;
			matrix.row = 0;
			break;

		case SDL_SCANCODE_RETURN:
			matrix.col = 0;
			matrix.row = 1;
			break;

		case SDL_SCANCODE_COMMA:
			matrix.col = 5;
			matrix.row = 7;
			break;

		case SDL_SCANCODE_SPACE:
			matrix.col = 7;
			matrix.row = 4;
			break;

		case SDL_SCANCODE_LSHIFT:
			matrix.col = 1;
			matrix.row = 7;
			break;

		case SDL_SCANCODE_F1:	//Equals
			matrix.col = 6;
			matrix.row = 5;
			break;

		case SDL_SCANCODE_F2:	//Shift + 2
			cout<<"f2"<<endl;
			matrix.col = 0xFD;
			matrix.row = 0x7E;

			return matrix;
			break;

		case SDL_SCANCODE_BACKSPACE:
			matrix.col = 0;
			matrix.row = 0;
			break;
	}

	/*matrix.col =  (1 << matrix.col); 
	matrix.row = (1 << matrix.row); 

	if(pressed){
		matrix.col =  ~matrix.col; 
		matrix.row = ~matrix.row; 
	}*/

	return matrix;

}