#pragma once

#include "AssetManager.h"
#include <memory>

//*this requieres to use an specific notation
//*.fnt to return a fncFont
//*.spr to return a fncSprite
//*.sfx to return a fncSFX
//*.msc to return a fncMusic


fncSprite* operator""spr(const char* c, std::size_t n) {
	const char *type = &c[n - 4];

	if (strcmp(".spr", &c[n - 4]) == 0) {

		//printf("%s", c);
		return AssetManager::Instance()->sprite[c];
	}
	else {
		std::unique_ptr<char[]> key = std::make_unique<char[]>(n+5);
		/*for (unsigned char i = 0; i < n + 4; i++)
			key[i] = ' ';*/

		memcpy_s(key.get(), n, c, n);
		memcpy_s(key.get() + n, n, ".spr", 5);

		//printf("%s", key.get());
		return AssetManager::Instance()->sprite[&key[0]];
	}	
}