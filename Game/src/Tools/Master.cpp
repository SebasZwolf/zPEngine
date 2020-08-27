#include "Master.h"

#include <SDL_image.h>
#include <SDL_mixer.h>

#include <bitset>

#include "AssetManager.h"

MasterTool* MasterTool::sInstance = NULL;

void MasterTool::Release() {
	fnc::iSing<MasterTool>::Release();
}

MasterTool* MasterTool::Instance() {
	return fnc::iSing <MasterTool>::Instance();
}

//DRAW
static SDL_Renderer* renderer = NULL;
static fncColor realColor = {0x00,0x00,0x00};

void MasterTool::Draw::setUp(SDL_Renderer* _renderer) {
	static bool config = false;
	if (config) return;
	
	renderer = _renderer;

	config = true;
}

static void updateColor() {
	static MasterTool::Draw::Data& data = fnc::iSing<MasterTool>::Instance()->draw.get;

	if (realColor == data.color) return;
	realColor = data.color;
	SDL_SetRenderDrawColor(renderer, data.color.r, data.color.g, data.color.b, data.alpha);
}

void MasterTool::Draw::Outline::rect(fig::cRect & _dst){
	updateColor();
	
	SDL_Rect dst{
	_dst.pos.x,
	_dst.pos.y,
	_dst.siz.x,
	_dst.siz.y,
	};

	SDL_RenderDrawRect(renderer, &dst);
}

void MasterTool::Draw::Outline::circle(fig::cSphere & _dst){
	updateColor();

	int p = std::sqrt(_dst.r / 2) + 2;
	float angle = M_PI / p;

	for (unsigned char ii = 0; ii < p; ii++) {
		float
			ax = _dst.r*std::cos(ii * angle),
			ay = _dst.r*std::sin(ii * angle),
			bx = _dst.r*std::cos((ii + 1) * angle),
			by = _dst.r*std::sin((ii + 1) * angle);
		SDL_RenderDrawLine(
			renderer,
			_dst.pos.x + ax,
			_dst.pos.y + ay,
			_dst.pos.x + bx,
			_dst.pos.y + by
		);
		SDL_RenderDrawLine(
			renderer,
			_dst.pos.x + ax,
			_dst.pos.y - ay,
			_dst.pos.x + bx,
			_dst.pos.y - by
		);
	}
}

void MasterTool::Draw::Outline::tria(fig::cTria & t){
	updateColor();

	fig::Vector2D points[3];
	t.getPoints(points);

	for (fnc::usmall i = 0; i < 3; i++) {
		SDL_RenderDrawLine(
			renderer,	
			points[i].x,
			points[i].y,
			points[(i + 1) % 3].x,
			points[(i + 1) % 3].y
		);
	}	
}

void MasterTool::Draw::Filled::rect(fig::cRect & _dst){
	updateColor();

	SDL_Rect dst{
	_dst.pos.x,
	_dst.pos.y,
	_dst.siz.x,
	_dst.siz.y,
	};

	SDL_RenderFillRect(renderer, &dst);
}

void MasterTool::Draw::Filled::circle(fig::cSphere & _dst){
	updateColor();

	for (fnc::uint dx = 0; dx < _dst.r * 2; dx++) {
		float dr = std::sqrtf(std::powf(_dst.r, 2) - std::powf(dx - _dst.r, 2));
		SDL_RenderDrawLine(renderer,
			_dst.pos.x - _dst.r + dx,
			_dst.pos.y + dr,
			_dst.pos.x - _dst.r + dx,
			_dst.pos.y - dr
		);
	}
}

void MasterTool::Draw::Filled::tria(fig::cTria &dst ){
	updateColor();

	fnc::usmall drawable = 0;

	fig::Vector2D points[3];
	dst.getPoints(points);

	for (fnc::usmall i = 0; i < 3; i++)
		if (points[i].x == points[(i + 1) % 3].x) { drawable = (i + 2) % 3 + 1; break; }
		else
			if (points[i].y == points[(i + 1) % 3].y) { drawable = (i + 2) % 3 + 4; break; }

	if (drawable == 0) {
		std::pair<fig::cTria, fig::cTria> t = dst.subdivide(
			dst.getboundary().siz.x < dst.getboundary().siz.y
		);
		tria(t.first);
		tria(t.second);
	}
	else if (drawable < 4) {
		short dx = -points[drawable - 1].x + points[drawable % 2].x;
		for (short i = 0; true; i = fnc::aproach((float)i, 1, dx)) {
			SDL_RenderDrawLine(
				renderer,
				points[drawable - 1].x + i,
				fnc::lerp(points[drawable - 1].y, points[(drawable + 1) % 3].y, (float)i / (float)dx),
				points[drawable - 1].x + i,
				fnc::lerp(points[drawable - 1].y, points[drawable % 3].y, (float)i / (float)dx)
			);
			if (i == dx) break;
		}
	}
	else {
		drawable -= 3;

		short dy = -points[drawable - 1].y + points[drawable % 2].y;
		for (short i = 0; true; i = fnc::aproach((float)i, 1, dy)) {
			SDL_RenderDrawLine(
				renderer,
				fnc::lerp(points[drawable - 1].x, points[(drawable + 1) % 3].x, (float)i / (float)dy),
				points[drawable - 1].y + i,
				fnc::lerp(points[drawable - 1].x, points[drawable % 3].x, (float)i / (float)dy),
				points[drawable - 1].y + i
			);
			if (i == dy) break;
		}
	}
}

void MasterTool::Draw::line(fig::Point<int>& src, fig::Point<int>& dst){
	updateColor();
	SDL_RenderDrawLine(renderer, src.x, src.y, dst.x, dst.y);
}

void MasterTool::Draw::clear(){
	auto& c = MasterTool::Instance()->draw.get;
	SDL_SetRenderDrawColor(renderer, c.background_color.r, c.background_color.g, c.background_color.b, 0xff);

	SDL_RenderClear(renderer);

	SDL_SetRenderDrawColor(renderer, c.color.r, c.color.g, c.color.b, 0xff);
}

void MasterTool::Draw::clear(fncColor & c){
	SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, 0xff);

	SDL_RenderClear(renderer);

	auto& _c = MasterTool::Instance()->draw.get.color;
	SDL_SetRenderDrawColor(renderer, _c.r, _c.g, _c.b, 0xff);
}

bool fncColor::operator==(const fncColor & c){
	return (this->r == c.r && this->g == c.g && this->b == c.b);
}

void MasterTool::Draw::Sprite::operator()(fnc::text key, fnc::usmall subimg, int x, int y){
	fncSprite* sprite = AssetManager::Instance()->sprite[key];

	subimg %= sprite->image_number;

	SDL_Rect dst{ x,y,sprite->size.x, sprite->size.y };
	const fig::Point<fnc::ushort>& p = sprite->subImage(subimg, *sprite);
	SDL_Rect src{ p.x, p.y, sprite->size.x, sprite->size.y };

	SDL_RenderCopy(renderer, sprite->texture.texture, &src, &dst);
}

void MasterTool::Draw::Sprite::operator()(fncSprite * sprite, fnc::usmall subimg, int x, int y) {
	subimg %= sprite->image_number;

	SDL_Rect dst{ x,y,sprite->size.x, sprite->size.y };
	const fig::Point<fnc::ushort>& p = sprite->subImage(subimg, *sprite);
	SDL_Rect src{ p.x, p.y, sprite->size.x, sprite->size.y };

	SDL_RenderCopy(renderer, sprite->texture.texture, &src, &dst);
}

void MasterTool::Draw::Sprite::ext(fnc::text key, fnc::usmall subimg, int x, int y, fig::Vector2D scale, float angle, fncColor color, fnc::usmall alpha){
	fncSprite* sprite = AssetManager::Instance()->sprite[key];

	subimg %= sprite->image_number;

	SDL_Rect dst{ x,y,sprite->size.x * std::abs(scale.x), sprite->size.y * std::abs(scale.y) };

	const fig::Point<fnc::ushort>& p = sprite->subImage(subimg, *sprite);

	SDL_Rect src{ p.x, p.y, sprite->size.x, sprite->size.y };

	SDL_RendererFlip flag;
	if (scale.x * scale.y > 0)
		flag = SDL_FLIP_NONE;
	else
		if (scale.x > 0)
			flag = SDL_FLIP_VERTICAL;
		else
			flag = SDL_FLIP_HORIZONTAL;

	SDL_SetTextureColorMod(sprite->texture.texture, color.r, color.g, color.b);
	SDL_RenderCopyEx(renderer, sprite->texture.texture, &src, &dst, angle, NULL, flag);
	SDL_SetTextureColorMod(sprite->texture.texture, 0xff, 0xff, 0xff);

}

void MasterTool::Draw::Texture::operator()(fnc::text key, int x, int y){
	fncTexture* tex = AssetManager::Instance()->texture[key];
	SDL_Rect dst{ x,y,tex->size.x, tex->size.y };
	SDL_RenderCopy(renderer, tex->texture, NULL, &dst);
}

void MasterTool::Draw::Texture::ext(fnc::text key, int x, int y, fig::Vector2D scale, float angle, fncColor color, fnc::usmall alpha){
	fncTexture* tex = AssetManager::Instance()->texture[key];
	SDL_Rect dst{ x,y,tex->size.x * std::abs(scale.x), tex->size.y * std::abs(scale.y)};

	SDL_RendererFlip flag;

	if (scale.x * scale.y > 0)
		flag = SDL_FLIP_NONE;
	else
		if (scale.x > 0)
			flag = SDL_FLIP_VERTICAL;
		else
			flag = SDL_FLIP_HORIZONTAL;
	
	SDL_SetTextureColorMod(tex->texture, color.r, color.g, color.b);
	SDL_RenderCopyEx(renderer, tex->texture, NULL, &dst, angle, NULL, flag);
	SDL_SetTextureColorMod(tex->texture, 0xff, 0xff, 0xff);
}

typedef fncFont fncFont;

void MasterTool::Draw::Text::operator()(fnc::text text, int x, int y) {
	fncFont* font = MasterTool::Instance()->draw.get.font;

#ifdef Testing
	SDL_Rect a{ 0,0,0,0 };
	SDL_QueryTexture(font->texture, NULL, NULL, &a.w, &a.h);
	SDL_RenderCopy(renderer, font->texture, NULL, &a);
#endif
	if (font == nullptr) {
		printf("ERROR");
		return;
	}

	SDL_Rect dst{ x,y,0,0 };
	SDL_Rect src{ 0,0,0,0 };
#ifdef Testing
	int i = 0;
#endif
	while (*text != '\0') {
		if (*text == '\n') {
			dst.x = x;
			dst.y += font->height + 2;
			text++;
			continue;
		}

		auto& a = font->glyph_atlas[*text];
		src.w = dst.w = a.second; src.h = dst.h = font->height;
		src.x = a.first; src.y = 0;

		SDL_RenderCopy(renderer, font->texture, &src, &dst);

		dst.x += dst.w;
		text++;
#ifdef Testing
		if (i++ >= 200) return;
#endif
	}
}

//SOUND!
static std::bitset<48> BSChannels;
static fnc::usmall channels;

static unsigned char getFreeChannel(fnc::usmall max) {
	if (BSChannels[0] == false) return 0;

	unsigned short i = 0;
	while (i++ != max)
		if (BSChannels[i] == false) break;

#ifdef Testing
	printf("%u", i);
#endif
	return  i % max;
}

void MasterTool::Play::Music::operator()(fnc::text key, fnc::ushort loops){
	Mix_PlayMusic(AssetManager::Instance()->music[key]->source, loops);
}

void MasterTool::Play::Music::pause(bool flag){
	if (Mix_PlayingMusic() != flag) Mix_PauseMusic();	
}

void MasterTool::Play::Sfx::operator()(fnc::text key, fnc::ushort loops){
	unsigned char i = getFreeChannel(channels);

	Mix_PlayChannelTimed(i, AssetManager::Instance()->sfx[key]->source, loops, -1);
	BSChannels[i] = true;
}

void MasterTool::Play::Sfx::in_channel(fnc::text key, fnc::usmall channel, fnc::ushort loops){
	Mix_PlayChannelTimed(channel, AssetManager::Instance()->sfx[key]->source, loops, -1);
}

void MasterTool::Play::setUp(fnc::usmall channelNumber){
	static bool config = false;
	if (config) return;

	channels = channelNumber;
	config = true;
}
