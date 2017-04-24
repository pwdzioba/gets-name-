#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModulePlayer.h"
#include "ModuleScene1.h"
#include "ModuleInput.h"
#include "ModuleUI.h"
#include "ModuleFonts.h"
#include "ModuleSaveData.h"

#include<stdio.h>
#include<string.h>

ModuleUI::ModuleUI()
{

	MarcadorVida.x = 6;
	MarcadorVida.y = 227;
	MarcadorVida.w = 12;
	MarcadorVida.h = 17;

	vida0.x = 27;
	vida0.y = 6;
	vida0.w = 10;
	vida0.h = 10;


	vida1.x = 40;
	vida1.y = 6;
	vida1.w = 8;
	vida1.h = 10;


	vida2.x = 52;
	vida2.y = 6;
	vida2.w = 12;
	vida2.h = 10;


	vida3.x = 69;
	vida3.y = 6;
	vida3.w = 10;
	vida3.h = 10;

	MarcadorGranada.x = 29;
	MarcadorGranada.y = 229;
	MarcadorGranada.w = 12;
	MarcadorGranada.h = 13;

}
ModuleUI::~ModuleUI()
{

}
bool ModuleUI::Start()
{
	bool ret = true;

	LOG("Loading screen UI");
	Hud = App->textures->Load("items&HUD&snake.png");
	Hud2 = App->textures->Load("Alphabet&Numbers&Extra.png");
	if (Hud == nullptr) {
		LOG("Cannot load the texture");
		ret = false;
	}
	gate = false;
	var = 0;
	var2 = 0;

	return ret;
}

bool ModuleUI::CleanUp()
{
	bool ret = true;

	LOG("Unloading screen UI");
	App->textures->Unload(Hud);

	return false;
}

update_status ModuleUI::Update()
{

		if (App->player->position.y == (110 - App->scene_1->cont) && App->input->keyboard[SDL_SCANCODE_W] == KEY_STATE::KEY_REPEAT ) {
			cont = 1;
			sprintf_s(str1, "%i", score);
			sprintf_s(str2, "%i", grenade);
			sprintf_s(str3, "%i", App->player->vides);
			App->render->Blit(Hud, 116, 213 - App->scene_1->cont, &MarcadorGranada);
			App->render->Blit(Hud, 22, 210 - App->scene_1->cont, &MarcadorVida);
			App->fonts->BlitText(35, 215 - App->scene_1->cont, App->savedata->font_score, str3);
			App->fonts->BlitText(129, 215 - App->scene_1->cont, App->savedata->font_score, str2);
			cont = 0;
			App->fonts->BlitText(37, 5 - App->scene_1->cont, App->savedata->font_score, str1);
		}
		else {
			cont = 1;
			sprintf_s(str1, "%i", score);
			sprintf_s(str2, "%i", grenade);
			sprintf_s(str3, "%i", App->player->vides);
			App->render->Blit(Hud, 116, 213 - App->scene_1->cont, &MarcadorGranada);
			App->render->Blit(Hud, 22, 210 - App->scene_1->cont, &MarcadorVida);
			App->fonts->BlitText(35, 215 - App->scene_1->cont, App->savedata->font_score, str3);
			App->fonts->BlitText(129, 215 - App->scene_1->cont, App->savedata->font_score, str2);
			cont = 0;
			App->fonts->BlitText(37, 5 - App->scene_1->cont, App->savedata->font_score, str1);
		}

	

	return UPDATE_CONTINUE;
}
