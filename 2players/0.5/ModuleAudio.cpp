#include "Globals.h"
#include "Application.h"
#include "ModuleAudio.h"
#include "Module.h"
#include "ModuleFadeToBlack.h"
#include "ModuleScene1.h"
#include "ModuleScene2.h"
#include "ModuleEND.h"
#include "ModuleMenu.h"
#include "SDL/include/SDL.h"
#include "SDL_mixer/include/SDL_mixer.h"

#pragma comment( lib, "SDL_mixer/libx86/SDL2_mixer.lib" )

ModuleAudio::ModuleAudio() {}

bool ModuleAudio::Init() {

	Mix_Init(MIX_INIT_OGG);
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

	//Load music
	music[0] = Mix_LoadMUS("Assets/Audio/Commando_Sound-01-Title.ogg");
	music[1] = Mix_LoadMUS("Assets/Audio/Commando_Sound-03-Areas_1-2-3.ogg");
	music[2] = Mix_LoadMUS("Assets/Audio/Commando_Sound-03-Areas_1-2-3.ogg");
	music[3] = Mix_LoadMUS("Assets/Audio/Commando_Sound-10-Win-Phase-1-2-3.ogg"); //when you finish the level
	music[4] = Mix_LoadMUS("Assets/Audio/Commando_Sound-12-Game-Over.ogg"); //when you lose all lives
	music[5] = Mix_LoadMUS("Assets/Audio/Commando_Sound-07-Restart.ogg"); //it plays when you die and you respawn
	music[6] = Mix_LoadMUS("Assets/Audio/Commando_Sound-08-Fortress.ogg"); //final battle
	music[7] = Mix_LoadMUS("Assets/Audio/Commando_Sound-02-Start.ogg"); //helicopter animation

														   //Load FX
	fx[0] = Mix_LoadWAV("Assets/Audio/Commando_Effect-02-Principal-Shoot.wav");
	fx[1] = Mix_LoadWAV("Assets/Audio/Commando_Effect-01-Choose-Menu.wav");
	fx[2] = Mix_LoadWAV("Assets/Audio/Commando_Effect-12-Helicopter.wav"); //helicopter sound
	fx[3] = Mix_LoadWAV("Assets/Audio/Commando_Effect-08-Kill-Enemy.wav"); //when you kill an enemy sound
	fx[4] = Mix_LoadWAV("Assets/Audio/Commando_Effect-04-Grenade.wav"); //when a grenade explodes sound
	fx[5] = Mix_LoadWAV("Assets/Audio/Commando_Effect-09-Life-lost.wav"); //when an enemy kills you sound
	fx[6] = Mix_LoadWAV("Assets/Audio/Commando_Effect-10-Bonus.wav"); //when you pick an item
	fx[7] = Mix_LoadWAV("Assets/Audio/Commando_Effect-10-Bonus-Prisoner.wav"); //when you free a prisoner
	fx[8] = Mix_LoadWAV("Assets/Audio/Commando_Sound-13-Vehicle.wav"); //moto right-centre
	fx[9] = Mix_LoadWAV("Assets/Audio/Commando_Sound-14-Vehicle-Decreasing.wav"); //moto centre-left

	return true;
}

bool ModuleAudio::play_music1() {
	//Mix_PlayMusic(music[0], -1); play music without fade in
	Mix_FadeInMusic(music[0], -1, 2000);
	return true;
}
bool ModuleAudio::play_music2() {

	Mix_FadeInMusic(music[1], -1, 2000);
	return true;
}
bool ModuleAudio::play_music3() {
	Mix_FadeInMusic(music[2], -1, 2000);
	return true;
}
bool ModuleAudio::play_music4() {
	//Mix_PlayMusic(music[0], -1); play music without fade in
	Mix_FadeInMusic(music[3], -1, 2000);
	return true;
}
bool ModuleAudio::play_music5() {
	Mix_FadeInMusic(music[4], -1, 2000);
	return true;
}
bool ModuleAudio::play_music6() {
	Mix_FadeInMusic(music[5], -1, 2000);
	return true;
}
bool ModuleAudio::play_music7() {
	Mix_FadeInMusic(music[6], -1, 2000);
	return true;
}
bool ModuleAudio::play_music8() {
	Mix_FadeInMusic(music[7], -1, 2000);
	return true;
}

bool ModuleAudio::pause_music() {
	Mix_PauseMusic(); //pause music without fade out
					  //Mix_FadeOutMusic(2000);
	return true;
}
bool ModuleAudio::play_fx1() {
	Mix_PlayChannel(-1, fx[0], 0);
	return true;
}
bool ModuleAudio::play_fx2() {
	Mix_PlayChannel(-1, fx[1], 0);
	return true;
}
void ModuleAudio::play_fx3() {
	
	Mix_PlayChannel(-1, fx[2], 0);
}
bool ModuleAudio::play_fx4() {
	
	Mix_PlayChannel(-1, fx[3], 0);
	return true;
}
bool ModuleAudio::play_fx5() {
	Mix_PlayChannel(-1, fx[4], 0);
	return true;
}
bool ModuleAudio::play_fx6() {
	Mix_PlayChannel(-1, fx[5], 0);
	return true;
}
bool ModuleAudio::play_fx7() {
	Mix_PlayChannel(-1, fx[6], 0);
	return true;
}
bool ModuleAudio::play_fx8() {
	Mix_PlayChannel(-1, fx[7], 0);
	return true;
}
bool ModuleAudio::play_fx9() {
	Mix_PlayChannel(-1, fx[8], 0);
	return true;
}
bool ModuleAudio::play_fx10() {
	Mix_PlayChannel(-1, fx[9], 0);
	return true;
}

bool ModuleAudio::CleanUp() {

	for (int i = 0; i < MAX_MUSIC; i++)
		Mix_FreeMusic(music[i]);

	for (int i = 0; i < MAX_FX; i++)
		Mix_FreeChunk(fx[i]);

	return true;
}

ModuleAudio::~ModuleAudio() {}