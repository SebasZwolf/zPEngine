#pragma once

#ifndef __global_config__
#define __base_inner_config__

#include <bitset>

struct SDL_Window;

struct GlobalSettings{
private:
	static GlobalSettings* sInstance;

	std::bitset<16> innervalues;
public:
	SDL_Window* const window;
	static GlobalSettings* Instance();

	static bool Start(const char * ="zPaperEngine", unsigned int =640, unsigned int =480, int = 0x2fff0000, int = 0x2fff0000);
	static void End();

	bool operator[](unsigned short index) const;
	void operator()(unsigned short index);

	enum : unsigned short {
		//captures mouse events OUTISIDE WINDOW only when a button was down
		//RECOMENDADO
		CAPTURE_WHEN_DRAG,
		//captures mouse events OUTISIDE WINDOW always
		//NO RECOMENDADO
		CAPTURE_ALLWAYS,
		//captures NOT mouse events OUTISIDE WINDOW
		NO_CAPTURE,

		FIXED_SCREEN,
		FULL_SCREEN,

		BORDERLESS_SCREEN,
		BORDERED_SCREEN,

		WINDOW_UNGRAB,
		WINDOW_GRAB
	};

private:
	SDL_Window* const & GetWindow() const {
		return window;
	};

	GlobalSettings(SDL_Window*);
	~GlobalSettings();
	
	friend class GameManager;
};

#define sngGlobalSettings (*GlobalSettings::Instance())

#endif // !__base_inner_config__
