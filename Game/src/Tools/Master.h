#pragma once

#ifndef __master_tool__
#define __master_tool__

#include "fnc.h"

struct SDL_Renderer;

struct fncFont;

struct fncSprite;

struct fncColor {
	fnc::usmall r, g, b;

	bool operator ==(const fncColor& c);
};

class MasterTool : fnc::iSing<MasterTool>{
public:
	struct Draw{
		struct Data{
			fncColor background_color, color;
			fnc::usmall alpha;
			fncFont* font;
		}set, &const get = set;
		struct Outline{
			void rect(fig::cRect&);
			void circle(fig::cSphere&);
			void tria(fig::cTria&);
		}outline{};
		struct Filled{
			void rect(fig::cRect&);
			void circle(fig::cSphere&);
			void tria(fig::cTria&);
		}filled{};
		struct Sprite {
			void operator()(fncSprite* sprite, fnc::usmall subimg, int, int);
			void operator()(fnc::text key, fnc::usmall subimg, int, int);
			void ext(fnc::text key, fnc::usmall subimg, int, int, fig::Vector2D scale = {0.0f, 0.0f}, float angle = 0.0f, fncColor color = { 0xff, 0xff, 0xff }, fnc::usmall alpha = 0xff);
		}sprite;
		struct Texture {
			void operator()(fnc::text key, int, int);
			void ext(fnc::text key, int, int, fig::Vector2D scale = { 0.0f, 0.0f }, float angle = 0.0f, fncColor color = { 0xff, 0xff, 0xff }, fnc::usmall alpha = 0xff);
		}texture;
		struct Text {
			void operator()(fnc::text text, int, int);
		}text;
		void line(fig::Point<int>& src, fig::Point<int>& dst);
		void clear();
		void clear(fncColor&);
		void setUp(SDL_Renderer*);
	}draw;
	struct Play{
		struct Music{
			void operator()(fnc::text key, fnc::ushort loops = -1);
			void pause(bool flag);
		}music;
		struct Sfx{
			void operator()(fnc::text key, fnc::ushort loops = 0);
			void in_channel(fnc::text key, fnc::usmall channel, fnc::ushort loops = 0);
		}sfx;

		void setUp(fnc::usmall channelNumber);
	}play;

	void SetUp() {}	;

	static void Release();
	static MasterTool* Instance();
private:
	MasterTool() { printf("MasterTool init!\n"); }
	~MasterTool() { printf("MasterTool destroy!\n"); }

	friend class fnc::iSing<MasterTool>;
};


#endif