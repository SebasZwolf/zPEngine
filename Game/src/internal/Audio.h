#pragma once

#ifndef __audio__
#define __audio__


class Audio {
private:
	static Audio * sInstance;

	unsigned short channels;
public:
	static Audio* Instance();

	static bool Start(bool, unsigned short, unsigned short);
	static void End();

	unsigned short ask();
private:
	Audio(unsigned short& channelNumber);
	~Audio();
};


#endif // !__audio__
