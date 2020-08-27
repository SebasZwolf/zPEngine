#pragma once

#ifndef  __graphics__
#define  __graphics__

struct SDL_Renderer;
struct SDL_Window;

#include "fnc.h"

class Graphics {
private:
	static Graphics * sInstance;
	SDL_Renderer * const renderer;
public:
	static Graphics* Instance();
	static bool Start(SDL_Window*);

	static void End();

	void Render();

	SDL_Renderer* ask() const{
		return renderer;
	}
private:
	Graphics(SDL_Renderer*);
	~Graphics();
};



#endif // ! __graphics__
