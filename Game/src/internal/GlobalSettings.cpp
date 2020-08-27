#include "GlobalSettings.h"
#include <SDL.h>

GlobalSettings * GlobalSettings::sInstance = NULL;

GlobalSettings * GlobalSettings::Instance(){
	SDL_WINDOWPOS_CENTERED;

	if (sInstance == NULL) throw "GlobalSettings have not been initialized yet";
	return sInstance;
}

bool GlobalSettings::Start(const char *t, unsigned int w, unsigned int h, int x, int y){
	if (sInstance != NULL) return true;

	auto* window = SDL_CreateWindow(t, x, y, w, h, SDL_WINDOW_SHOWN);

	if (window == NULL) {
		printf("could not create Window: %s\n", SDL_GetError());
		return false;
	}

	sInstance = new GlobalSettings(window);
	return true;
}


void GlobalSettings::End(){
	if (sInstance == NULL) return;

	delete sInstance;
	sInstance = NULL;
}

bool GlobalSettings::operator[](unsigned short index) const{
	bool pair = index % 2 == 0 ? true : false;

	switch (index) {
	case CAPTURE_WHEN_DRAG:
		return innervalues[0] && !innervalues[1];
		break;

	case NO_CAPTURE:
	case CAPTURE_ALLWAYS:
		return pair ^ innervalues[1];
		break;

	default:
	{
		unsigned short aindex = (index - 1) / 2 + 1;
		return !pair ^ innervalues[aindex];
	}
		break;
	}
}

void GlobalSettings::operator()(unsigned short index) {
	bool pair = index % 2 == 0 ? true : false;

	//0: capture_when_dragged
	//1: capture_in_all_situations
	//2: fullscreen
	//3: bordered_sceen
	switch (index)
	{
	case CAPTURE_WHEN_DRAG:
		innervalues[0] = true;
		innervalues[1] = false;
		break;
	case NO_CAPTURE:
		innervalues[0] = false;
		innervalues[1] = false;
		SDL_CaptureMouse(SDL_FALSE);
		break;
	case CAPTURE_ALLWAYS:
		innervalues[0] = false;
		innervalues[1] = true;
		SDL_CaptureMouse(SDL_TRUE);
		break;

	case FIXED_SCREEN:
	case FULL_SCREEN:
		innervalues[2] = pair;
		SDL_SetWindowBordered(window, pair ? SDL_TRUE : SDL_FALSE);
		break;

	case BORDERLESS_SCREEN:
	case BORDERED_SCREEN:
		innervalues[3] = pair;
		SDL_SetWindowBordered(window, pair ? SDL_TRUE : SDL_FALSE);
		break;

	case WINDOW_UNGRAB:
	case WINDOW_GRAB:
		innervalues[4] = pair;
		SDL_SetWindowGrab(window, pair ? SDL_TRUE : SDL_FALSE);
		break;
	default:
		break;
	}
}

GlobalSettings::GlobalSettings(SDL_Window* window) : window(window){
	printf("GlobalSettings %s\n", "enabled");
}

GlobalSettings::~GlobalSettings(){
	SDL_DestroyWindow(window);

	printf("GlobalSettings %s\n", "disabled");
}
