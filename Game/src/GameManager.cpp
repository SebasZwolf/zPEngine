#include "GameManager.h"
#include <SDL.h>
#include <cstdio>

#include "Tools/InputManager.h"
#include "Tools/Master.h"

GameManager* GameManager::sInstance = NULL;

GameManager * GameManager::Start(GraphicsInfo gI, AudioInfo aI) {

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)				goto SdlInitFail;

	MasterTool::Instance()->SetUp();

	if (!GlobalSettings::Start(gI.title, gI.w, gI.h, gI.x, gI.h))	goto windowFail;
	if (!Graphics::Start(GlobalSettings::Instance()->GetWindow()))	goto GraphicsFail;
	if (!Audio::Start(aI.stereo, aI.chunkSize, aI.channelNumber))	goto AudioFail;

	sInstance = new GameManager(GlobalSettings::Instance(), Graphics::Instance(), Audio::Instance());
	return sInstance;

AudioFail:
	Graphics::End();
GraphicsFail:
	GlobalSettings::End();
windowFail:
	SDL_Quit();
SdlInitFail:
	return nullptr;
}

void GameManager::End(){
	if (sInstance == NULL) return;

	printf("\n\n\n");

	Manager::Release();
	InputManager::Release();

	MasterTool::Release();

	Graphics::End();
	GlobalSettings::End();
	Audio::End();
	
	SDL_Quit();

	delete sInstance;
	sInstance = NULL;
}

void GameManager::Run(){
	const int fps = 60;
	const int frameDelay = 1000 / fps;

	Uint32 frameStart;

	manager->born();
	while (mRunning) {
		frameStart = SDL_GetTicks();

		handle();
		update();
		render();

		performance = SDL_GetTicks() - frameStart;

		if (frameDelay > performance)
			SDL_Delay(frameDelay - performance);
	}

	End();
}


GameManager::GameManager(GlobalSettings* s, Graphics* g, Audio* a) : ev(*new SDL_Event()){
	mRunning = true;
	this->graphics = g;
	this->global_settings = s;
	this->audio = a;

	this->manager = Manager::Instance();

	printf("GameMannager %s\n", "initialized");
}

GameManager::~GameManager(){
	delete &ev;
	printf("GameMannager %s\n", "destroyed");
}

void GameManager::handle(){
	InputManager::Instance()->postupdate();

	SDL_PollEvent(&ev);
	if (ev.type == SDL_QUIT) mRunning = false;

	InputManager::Instance()->update();
}

void GameManager::update(){
	manager->step();
}

void GameManager::render(){
	MasterTool::Instance()->draw.clear();

	manager->draw();
	graphics->Render();
}

void GameManager::onlate(){
	
}

bool GameManager::ManagerOverride(IManager * manager) {
	if (manager == nullptr) return false;

	this->manager = manager;
	return true;
}