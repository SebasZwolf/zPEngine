#include "AssetManager.h"
#include "internal/Graphics.h"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

fncTexture::fncTexture(SDL_Texture * t) : _texture(t) {
	SDL_QueryTexture(t, NULL, NULL, (int*)&_size.x, (int*)&_size.y);
}
fncTexture::~fncTexture() {
	if (_texture != nullptr)
		SDL_DestroyTexture(_texture);
}

fncSprite::fncSprite(fncTexture & texture, fig::Point<fnc::ushort>& sprite_offset, fnc::ushort & image_number, fig::Point<fnc::ushort> sprite_size) :
	texture(texture),
	offset(sprite_offset),
	image_number(image_number),
	size(sprite_size) {}
fncSprite::~fncSprite() {
}
void fncSprite::config(fig::Point<fnc::ushort>(*func)(fnc::ushort, fncSprite &texture)) {
	this->subImage = func;
}

fncFont::fncFont(SDL_Texture * texture, char * inner, Detail detail) : texture(texture), data(inner), detail(detail) {
	int i;
	SDL_QueryTexture(texture, NULL, NULL, NULL, &i);
	height = i;
}
fncFont::~fncFont() {
	delete data;
	SDL_DestroyTexture(texture);
}

fncMusic::fncMusic(Mix_Music * s) : source(s) {
}
fncMusic::~fncMusic() {
	Mix_FreeMusic(source);
}

fncSFX::fncSFX(Mix_Chunk * s) : source(s) {
}
fncSFX::~fncSFX() {
	Mix_FreeChunk(source);
}

AssetManager* AssetManager::sInstance = NULL;

AssetManager * AssetManager::Instance(){
	if (sInstance == NULL) sInstance = new AssetManager();	
	
	return sInstance;
}
void AssetManager::Release(){
	if (sInstance == NULL) return;

	delete sInstance;
	sInstance = NULL;
}
AssetManager::AssetManager() {
	this->renderer = Graphics::Instance()->ask();
	printf("AssetMannager initiailzated!\n");
}
AssetManager::~AssetManager(){
	printf("AssetMannager destroyed!\n");
}

//TEXTURE---														--------->
AssetManager::_textures::_textures(SDL_Renderer *& renderer) : renderer(renderer) {}
AssetManager::_textures::~_textures() {
	for (auto tex : textureSheet)
		if (tex.second != NULL)
			delete tex.second;
	textureSheet.clear();
}
fncTexture* AssetManager::_textures::operator[](const char* key){
	if (textureSheet[key] == nullptr) {
		std::string path = SDL_GetBasePath();
		path.append(TextureFolder).append(key);

		SDL_Surface* tmpimage = IMG_Load(path.c_str());
		textureSheet[key] = new fncTexture(SDL_CreateTextureFromSurface(renderer, tmpimage));;
		SDL_FreeSurface(tmpimage);
#ifdef Testing
		printf("loaded thexture at %s\n", path);
#endif
	}

	return textureSheet[key];
}
fncTexture* AssetManager::_textures::load(const char* file) {
	if (textureSheet[file] == nullptr) {
	
	std::string path = SDL_GetBasePath();
	path.append(TextureFolder).append(file);

	SDL_Surface* tmpimage = IMG_Load(path.c_str());
	textureSheet[file] = new fncTexture(SDL_CreateTextureFromSurface(renderer, tmpimage));;
	SDL_FreeSurface(tmpimage);
#ifdef Testing
	printf("loaded thexture at %s\n", path);
#endif
	}else
		printf("texture already loaded!\n");
	return textureSheet[file];
}

//SPRITE---															--------->
AssetManager::_sprites::_sprites(SDL_Renderer *& renderer) : renderer(renderer) {}
AssetManager::_sprites::~_sprites() {
	for (auto& c : spriteSheet)
		if (c.second != nullptr)
			delete c.second;
}
fncSprite* AssetManager::_sprites::operator[](const char* key) {
	if (spriteSheet[key] == nullptr) {
		return nullptr;
	}

	return spriteSheet[key];
}
fncSprite * AssetManager::_sprites::load(fncTexture* texture, std::string key, fnc::ushort image_number, fig::Point<fnc::ushort> size, fig::Point<fnc::ushort> offset, fig::Point<fnc::ushort>(*subImage)(fnc::ushort, fncSprite&)){
	if (spriteSheet[key] != nullptr) {
		printf("sprite already loaded!\n");
		return spriteSheet[key];
	}

	fncSprite* sprite = new fncSprite(*texture, offset, image_number, size);
	sprite->config(subImage);

	printf("sprite loaded!\n");
	spriteSheet[key] = sprite;
	return spriteSheet[key];
}

//FONT---															--------->
AssetManager::_font::_font(SDL_Renderer *& renderer) : renderer(renderer) {}
AssetManager::_font::~_font() {
	for (auto& c : fontSheet)
		if (c.second != nullptr)
			delete c.second;
}
fncFont * AssetManager::_font::operator[](const char * fileName) {
	return fontSheet[fileName];
};
fncFont * AssetManager::_font::load(fnc::text file, fnc::usmall char_size, fnc::text key, fncFont::Detail detail){
	if (fontSheet[key] == nullptr) {
		std::string path = SDL_GetBasePath();
		path.append(FontFolder).append(file);

		TTF_Font * openfont = TTF_OpenFont(path.c_str(), char_size);

		if (openfont == nullptr) {
			printf("error loading this font: %s\n", TTF_GetError());
			return nullptr;
		}

		char* ascii = new char[96];
		memset(ascii, 0, 96);

		for (char c = 32; c != 127; c++)
			ascii[c - 32] = c;

		std::map<char, std::pair<fnc::uint, fnc::ushort>> glyph_atlas;

		SDL_Texture * text;
		{
			long unsigned int width = 0;
			fnc::ushort height = TTF_FontHeight(openfont);
			
			for (char* it = ascii; *it != '\0'; it++) {
				int minx, maxx, adv;

				TTF_GlyphMetrics(openfont, *it, &minx, &maxx, NULL, NULL, &adv);
#ifdef debug
				printf("%c: x<%i,%i>, a<%i>\n", *it, maxx, minx, adv);
#endif			
				unsigned char this_width = std::fmaxf(adv, maxx) - std::fminf(0, minx);

				glyph_atlas.emplace(std::pair<char, std::pair<fnc::uint, fnc::ushort>>(*it, {width, this_width}));
				width += this_width;
			}

			text = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height);
		}
		if (text == nullptr) {
			delete[] ascii;
			printf("error loading this font: %s\n", TTF_GetError());
			return nullptr;
		}
		{					
			SDL_SetRenderTarget(renderer, text);

			SDL_SetTextureBlendMode(text, SDL_BLENDMODE_BLEND);
			SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0);
			SDL_RenderClear(renderer);

			SDL_BlendMode ax;
			SDL_GetRenderDrawBlendMode(renderer, &ax);
			SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
			SDL_Rect dst{ 0,0,0,0 };
			for (char* it = ascii; *it != '\0'; it++) {
					SDL_Surface* glyp_surface;

					switch (detail) {
					default:
					case fncFont::Solid: 
						glyp_surface = TTF_RenderGlyph_Solid(openfont, *it, { 0xff, 0xff, 0xff });
						break;
					case fncFont::Shadow: 
						glyp_surface = TTF_RenderGlyph_Shaded(openfont, *it, { 0xff, 0xff, 0xff }, {0x00,0x00,0x00, 0x00});
						break;
					case fncFont::Blended: 
						glyp_surface = TTF_RenderGlyph_Blended(openfont, *it, { 0xff, 0xff, 0xff });
						break;
					}

				dst.w = glyp_surface->w;
				dst.h = glyp_surface->h;

#ifdef debug
					printf("%c: A<%u,%u> R<%u,%u>\n", *it, glyph_atlas[*it].first, glyph_atlas[*it].second, dst.w, dst.h);
#endif
					SDL_Texture * temp_texture = SDL_CreateTextureFromSurface(renderer, glyp_surface);
					SDL_SetTextureBlendMode(temp_texture, SDL_BLENDMODE_BLEND);

					SDL_FreeSurface(glyp_surface);
#ifdef debug
					if(!SDL_RenderCopy(renderer, temp_texture, NULL, &dst)) printf("ERROR: not able to render glyph!\n");
#else
					SDL_RenderCopy(renderer, temp_texture, NULL, &dst);
#endif

					SDL_DestroyTexture(temp_texture);
				dst.x += dst.w;
			}

			SDL_SetRenderTarget(renderer, NULL);
			SDL_SetRenderDrawBlendMode(renderer, ax);
		}
		TTF_CloseFont(openfont);

		fncFont* tmpFont = new fncFont(text, ascii, detail);
		tmpFont->glyph_atlas = std::move(glyph_atlas);

		fontSheet[key] = tmpFont;
	}
	current = fontSheet[key];
	return current;
}

//MUSIC---															--------->
AssetManager::_music::_music() {}
AssetManager::_music::~_music() {
	for (auto& c : musicSheet)
		delete c.second;
}
fncMusic * AssetManager::_music::operator[](const char * key){
	if (musicSheet[key] == nullptr) {
		std::string path = SDL_GetBasePath();
		path.append(MusicFolder).append(key);

		fncMusic* a = new fncMusic(Mix_LoadMUS(path.c_str()));

		if (a == nullptr) {
			printf("Music loading failed: %s\n", Mix_GetError());
			return nullptr;
		}
		musicSheet[key] = a;
	}
	return musicSheet[key];
}

//SFX---															--------->
AssetManager::_sfx::_sfx() {}
AssetManager::_sfx::~_sfx() {
	for (auto& c : sfxSheet)
		delete c.second;
}
fncSFX* AssetManager::_sfx::operator[](const char * key){
	if (sfxSheet[key] == nullptr) printf("error, no SFX with that key found!\n");
	return sfxSheet[key];
}
void AssetManager::_sfx::load(const char * key){
	if (sfxSheet[key] != nullptr) return;

	std::string path = SDL_GetBasePath();
	path.append(SFXFolder).append(key);

	fncSFX* a = new fncSFX(Mix_LoadWAV_RW(SDL_RWFromFile(path.c_str(), "rb"), 1));

	if (a == nullptr) 
		printf("SFX file %s load failed: %s\n", path.c_str(), Mix_GetError());
	
	sfxSheet[key] = a;
}
template<typename ...Types> void AssetManager::_sfx::load(const char * key, Types ...keys) {
	load(key);
	load(keys);
}
