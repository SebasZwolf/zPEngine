#include "Audio.h"
#include <stdio.h>

//#include "Play.h"
#include "../Tools/Master.h"
#include <SDL_mixer.h>

Audio* Audio::sInstance = NULL;

Audio * Audio::Instance(){
	if (sInstance == nullptr) throw "Audio have not been initialized yet";

	return sInstance;
}

bool Audio::Start(bool stereo, unsigned short chunkSize, unsigned short channelNumber){
	if (Mix_Init(MIX_INIT_FLAC | MIX_INIT_MOD | MIX_INIT_MP3 | MIX_INIT_OGG) != (MIX_INIT_FLAC | MIX_INIT_MOD | MIX_INIT_MP3 | MIX_INIT_OGG)) {
		printf("could not initialize Audio: %s\n", Mix_GetError());
		goto MixInitFail;
	}
	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, stereo ? 2 : 1, chunkSize) != 0) {
		printf("Mixer initialization error: %s\n", Mix_GetError());
		goto OpenAdudioFail;
	}

	sInstance = new Audio(channelNumber);
	return true;

OpenAdudioFail:
	Mix_Quit();
MixInitFail:
	return false;
}

void Audio::End(){
	if (sInstance == nullptr) return;

	delete sInstance;
}

Audio::Audio(unsigned short& channelNumber){	
	MasterTool::Instance()->play.setUp(channelNumber);
	printf("Audio %s\n", "initialized");
}

Audio::~Audio(){
	Mix_CloseAudio();
	Mix_Quit();

	printf("Audio %s\n", "destroyed");
}
