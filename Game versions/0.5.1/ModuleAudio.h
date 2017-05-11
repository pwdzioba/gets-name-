#ifndef _ModuleAudio_H_
#define _ModuleAudio_H_

#include "Module.h"
#include "SDL_mixer/include/SDL_mixer.h"

class ModuleAudio : public Module
{
public:
	ModuleAudio();
	~ModuleAudio();

	bool Init();
	bool CleanUp();

	bool play_music1();
	bool play_music2();
	bool play_music3();
	bool play_music4();
	bool play_music5();
	bool play_music6();
	bool play_music7();
	bool play_music8();

	bool pause_music();

	bool play_fx1();
	bool play_fx2();
	void play_fx3();
	bool play_fx4();
	bool play_fx5();
	bool play_fx6();
	bool play_fx7();
	bool play_fx8();
	bool play_fx9();
	bool play_fx10();

public:
	Mix_Music* music[MAX_MUSIC];
	Mix_Chunk* fx[MAX_FX];
};

#endif
