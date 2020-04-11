#define DEBUG_MODE 1

#if defined DEBUG_MODE && DEBUG_MODE == 1
	#define DEBUG_PRINT(x) cout<<x;
#else
	#define DEBUG_PRINT(x) ;
#endif