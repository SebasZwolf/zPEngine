#include "Graphics.h"

#include <SDL_image.h>
#include <SDL_ttf.h>

#include <stdio.h>

#include "../Tools/AssetManager.h"
#include "Tools/Master.h"

Graphics* Graphics::sInstance = NULL;

Graphics * Graphics::Instance(){
	if (sInstance == nullptr) throw "Graphics have not been initialized yet";

	return sInstance;
}

bool Graphics::Start(SDL_Window* window){
	if (sInstance != nullptr) return true;

	SDL_Renderer* renderer = NULL;
	
	if(IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG | IMG_INIT_TIF) != (IMG_INIT_PNG | IMG_INIT_JPG | IMG_INIT_TIF)) {
		printf("could not initialize img: %s\n", IMG_GetError());
		goto ImgFail;
	}

	if (TTF_Init() == -1) {
		printf("could not initialize ttf: %s\n", TTF_GetError());
		goto TtfFail;
	}

	renderer = SDL_CreateRenderer(window, -1, 0);
	if (renderer == NULL) {
		printf("could not create renderer: %s\n", SDL_GetError());
		goto RendererFail;
	}

	sInstance = new Graphics(renderer);
	return true;

RendererFail:
	TTF_Quit();
TtfFail:
	IMG_Quit();
ImgFail:
	return false;
}

void Graphics::End(){
	if (sInstance == NULL) return;

	AssetManager::Release();

	delete sInstance;
}

void Graphics::Render(){
	SDL_RenderPresent(renderer);
}

Graphics::Graphics(SDL_Renderer* r) : renderer(r) {
	printf("Graphics %s\n","initialized");

	sngAManager.SetUp(renderer);
	MasterTool::Instance()->draw.setUp(renderer);

	/*sngDraw.SetUp(renderer);

	sngDraw.draw.set.backgroudColour	= { 0xaa,0xff,0xaa };
	sngDraw.draw.set.colour				= { 0xaa,0xaa,0xff };*/

}

Graphics::~Graphics(){
	TTF_Quit();
	IMG_Quit();
	SDL_DestroyRenderer(renderer);
	
	printf("Graphics %s\n","destroyed");
}
