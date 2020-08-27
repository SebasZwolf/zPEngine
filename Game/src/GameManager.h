#pragma once
#ifndef __game_manager__
#define __game_manager__

#ifdef __MOUSE_CAPTUREABLE__
#define __DO_CAPTURE_MOUSE__
#endif

#include "internal/GlobalSettings.h"
#include "internal/Graphics.h"
#include "internal/Audio.h"
#include "Tools/IManage.h"

struct GraphicsInfo{
	fnc::text title;
	fnc::uint w, h;
	int x, y;
	GraphicsInfo(fnc::text t = "zPaperEngine", fnc::uint w = 640, fnc::uint h = 480, int x = 0x2fff0000, int y = 0x2fff0000) : title(t), w(w), h(h), x(x), y(y) {}
};

struct AudioInfo{
	bool stereo;
	fnc::ushort chunkSize;
	fnc::ushort channelNumber;
	AudioInfo(bool s = false, fnc::ushort c = 1024, fnc::ushort n = 16) : stereo(s), chunkSize(c), channelNumber(n) {}
};

union SDL_Event;

class GameManager {
private:

	static GameManager* sInstance;
	bool mRunning;

	SDL_Event& ev;

	Graphics* graphics;
	GlobalSettings* global_settings;
	Audio* audio;

	IManager* manager;

	unsigned int performance;
public:
	
	unsigned int& fps = performance;

	//Set the initial conditions to the GameEngine
	static GameManager* Start(
		GraphicsInfo, AudioInfo
	);
	static void End();

	//THIS FUNC ALLOWS THE USEAGE OF A CUSTOM INSTANCE MANAGER
	bool ManagerOverride(IManager*);

	fnc::func<void, void> onCreate;

	//USE ONLY AFTHER MAKE ALL CONFIGURATIONS WERE MADE
	void Run();
private:
	GameManager(GlobalSettings*, Graphics*, Audio*);
	~GameManager();

	//FUNCIONES DE GAME
	void handle();
	void update();
	void render();

	void onlate();

	GameManager(const GameManager&) = delete;
};

struct GameInstance {
private:
	GameManager* const instance;
public:
	GameInstance(GameManager* s) : instance(s) {}
	operator bool() {
		return instance != nullptr;
	}

	void Run() {
		instance->Run();
	}
};

#endif // !__game_manager__