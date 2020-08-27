#pragma once

#ifndef __audio__
#define __audio__


class Audio {
private:
	static Audio * sInstance;
public:
	static Audio* Instance();

	static bool Start(bool, unsigned short, unsigned short);
	static void End();

private:
	Audio(unsigned short& channelNumber);
	~Audio();
};


#endif // !__audio__
