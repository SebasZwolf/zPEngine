#pragma once
#ifndef __asset_Mannager__
#define __asset_Mannager__

#include <map>
#include <string>

#include "fnc.h"

#define TextureFolder	"Assets\\"
#define FontFolder		"Assets\\Font\\"
#define MusicFolder		"Assets\\Music\\"
#define SFXFolder		"Assets\\SFX\\"

struct SDL_Texture;
struct SDL_Renderer;

typedef struct _Mix_Music Mix_Music;
struct Mix_Chunk;

typedef struct fncTexture {
private:
	fig::Point<fnc::uint> _size;
	SDL_Texture* _texture;
public:
	SDL_Texture* const& texture = _texture;
	const fig::Point<fnc::uint> &size = _size;
	fncTexture(SDL_Texture* t);
	~fncTexture();
} fncTexture;
typedef struct fncSprite{
private:
	void config(fig::Point<fnc::ushort>(*func)(fnc::ushort, fncSprite& texture));
public:
	const fncTexture& texture;
	fig::Point<fnc::ushort>(*subImage)(fnc::ushort i, fncSprite& texture);

	const fig::Point<fnc::ushort> offset;
	const fig::Point<fnc::ushort> size;
	const fnc::ushort image_number;

	fncSprite(fncTexture& texture, fig::Point<fnc::ushort>& sprite_offset, fnc::ushort& image_number, fig::Point<fnc::ushort>sprite_size);
	~fncSprite();
	friend class AssetManager;
} fncSprite;
typedef struct fncFont {
private:
public:
	SDL_Texture* const texture;
	std::map<char, std::pair<fnc::uint, fnc::ushort>> glyph_atlas;
	char* const data;

	enum Detail {
		Solid,
		Shadow,
		Blended
	}const detail;

	fnc::ushort height;
	fnc::ushort ascent;
	fnc::ushort descent;

	fncFont(SDL_Texture* texture, char* inner, Detail detail);
	~fncFont();
} fncFont;
typedef struct fncMusic {
	Mix_Music* const source;
	fncMusic(Mix_Music*);
	~fncMusic();
} fncMusic;
typedef struct fncSFX {
	Mix_Chunk* const source;
	fncSFX(Mix_Chunk*);
	~fncSFX();
} fncSFX;

class AssetManager{
private:
	static AssetManager* sInstance;
	SDL_Renderer* renderer;
	
public:
	static AssetManager* Instance();
	static void Release();

	struct _textures {
	private:
		SDL_Renderer* &renderer;
		std::map<std::string, fncTexture *> textureSheet;

		_textures(SDL_Renderer*&renderer);
		~_textures();
	public:
		fncTexture* operator[](const char* key);
		fncTexture* load(const char* file);

		friend class AssetManager;
	}texture{ renderer };

	struct _sprites {
	private:
		SDL_Renderer* &renderer;
		std::map<std::string, fncSprite *> spriteSheet;

		_sprites(SDL_Renderer*&renderer);
		~_sprites();
	public:
		fncSprite* operator[](const char* key);
		fncSprite* load(fncTexture* texture,
			std::string key,
			fnc::ushort image_number,
			fig::Point<fnc::ushort> size,
			fig::Point<fnc::ushort> offset = {0,0},
			fig::Point<fnc::ushort>(*subImage)(fnc::ushort, fncSprite&) = [](fnc::ushort i, fncSprite& self)->fig::Point<fnc::ushort> { return { static_cast<fnc::ushort>(i * self.size.y), i }; });

		friend class AssetManager;
	}sprite{ renderer };

	struct _font {
	private:
		SDL_Renderer*& renderer;
		std::map<std::string, fncFont*> fontSheet;
		_font(SDL_Renderer*&renderer);
		~_font();
	public:
		fncFont* operator[](const char* fileName);
		fncFont* current;
		fncFont* load(fnc::text fileName, fnc::usmall char_size, fnc::text key, fncFont::Detail detail = fncFont::Detail::Solid);

		friend class AssetManager;
	}font{renderer};

	struct _music {
	private:
		_music();
		~_music();
		
		std::map <std::string, fncMusic*> musicSheet;
	public:
		fncMusic* operator[](const char* key);

		friend class AssetManager;
	}music{};

	struct _sfx {
	private:
		_sfx();
		~_sfx();

		std::map <std::string, fncSFX*> sfxSheet;
	public:
		fncSFX* operator[](const char* key);
		void load(const char* key);

		template <typename... Types>
		void load(const char* key, Types... keys);

		friend class AssetManager;
	}sfx{ };


private:
	AssetManager();
	~AssetManager();

	AssetManager(const AssetManager&) = delete;

	friend class Graphics;
};

#define sngAManager (*AssetManager::Instance())
#endif // __asset_Mannager__

