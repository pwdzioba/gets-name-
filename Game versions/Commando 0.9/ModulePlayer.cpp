#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleScene1.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"
#include "ModuleParticles.h"
#include "ModulePlayer.h"
#include "ModuleCollision.h"
#include "ModuleSecretRoom1A.h"
#include "ModuleEnemies.h"
#include "ModuleSecretRoomB.h"
#include "ModuleSecretRoomC.h"
#include "ModuleSecretRoomD.h"
#include "ModuleSecretRoomE.h"
#include "ModuleFadeToBlack.h"
#include "ModuleMenu.h"
#include "ModulePlayer2.h"
#include "ModuleUI.h"
#include "ModuleEnding.h"
#include "ModuleParticlesGrenade.h"
#include "ModuleParticlesGrenade1.h"
#include "SDL/include/SDL_timer.h"
#include "ModuleCinematicRadio.h"
#include <time.h>
#include <stdlib.h>
//We have to change the position of the end of the bullet and the end of the grenade!

ModulePlayer::ModulePlayer()
{
	// idle animation (arcade sprite sheet)
	idleF.PushBack({ 34, 15, 16, 24 });
	idleF.PushBack({ 48, 15, 15, 26 });

	// idle animation (arcade sprite sheet)
	backward.PushBack({ 4, 16, 16, 26 });
	backward.PushBack({ 20, 15, 15, 26 });

	// walk forward animation (arcade sprite sheet)
	//forward.frames.PushBack({9, 136, 53, 83});
	forward.PushBack({ 34, 15, 16, 24 });
	forward.PushBack({ 48, 15, 15, 26 });

	right.PushBack({ 200, 13, 19, 28 });
	right.PushBack({ 219, 14, 18, 26 });

	left.PushBack({ 5, 41, 19, 25 });
	left.PushBack({ 23, 42, 19, 25 });

	diagWD.PushBack({ 113, 15, 17, 25 });
	diagWD.PushBack({ 130, 14, 18, 26 });

	diagWA.PushBack({ 165, 14, 19, 27 });
	diagWA.PushBack({ 183, 16, 18, 25 });

	diagSA.PushBack({ 63, 15, 17, 25 });
	diagSA.PushBack({ 148, 16, 17, 24 });

	diagSD.PushBack({ 80, 15, 17, 25 });
	diagSD.PushBack({ 97, 16, 17, 24 });

	invisible.PushBack({ 1, 1, 1, 1 });

	waterDie.PushBack({ 239, 16, 13, 16 });

	grenade.PushBack({ 170, 44, 15, 22 });
	grenade.PushBack({ 188, 44, 16, 24 });
	grenade.speed = 0.04;

	die.PushBack({ 43, 44, 16, 27 });
	die.PushBack({ 62, 39, 15, 27 });
	die.PushBack({ 80, 44, 17, 27 });
	die.speed = 0.02;

	stairsRoom.PushBack({ 116,43,15,17 });

	R.x = 0;
	R.y = -2656;
	R.w = 256;
	R.h = 2880;

	help.x = 226;
	help.y = 45;
	help.w = 9;
	help.h = 19;

	dead2player.x = 153;
	dead2player.y = 85;
	dead2player.w = 13;
	dead2player.h = 13;
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{

	
	position.x = 130;
	position.y = 110;

	

	collrev = App->collision->AddCollider({ 300, 300, 40, 40 }, COLLIDER_REVIVE, this);
	coll = App->collision->AddCollider({ 300, 300, 12, 15 }, COLLIDER_PLAYER, this); //the collider is out of the screen!
	feetC = App->collision->AddCollider({ 300 - 16, 300 - 19, 8, 4 }, COLLIDER_PLAYER, this);
	
	follow_p = App->collision->AddCollider({ 300 - 16, 300 - 19, 50, 120 }, COLLIDER_FOLLOW, this);
	follow_p1 = App->collision->AddCollider({ 300 - 16, 300 - 19, 50, 120 }, COLLIDER_FOLLOW, this);
	follow_p2 = App->collision->AddCollider({ 300 - 16, 300 - 19, 120, 50 }, COLLIDER_FOLLOW, this);
	follow_p3 = App->collision->AddCollider({ 300 - 16, 300 - 19, 120, 50 }, COLLIDER_FOLLOW, this);

	p1.x = position.x - 30;
	p1.y = position.y;

	p2.x = position.x;
	p2.y = position.y - 30;

	p3.x = position.x + 30;
	p3.y = position.y;

	p4.x = position.x;
	p4.y = position.y + 30;

	LOG("Loading player textures");
	graphics = App->textures->Load("Assets/Sprites/spritesheet_humanos.png");

	return true;
}

bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	App->textures->Unload(graphics);
	if (coll != nullptr)
		coll->to_delete = true;

	return true;
}

// Update: draw background
update_status ModulePlayer::Update()
{
	p1.x = position.x - 30;
	p1.y = position.y;

	p2.x = position.x;
	p2.y = position.y - 30;

	p3.x = position.x + 30;
	p3.y = position.y;

	p4.x = position.x;
	p4.y = position.y + 30;

	speed = 1;

	if ((App->input->keyboard[SDL_SCANCODE_F2] == KEY_DOWN || App->input->buttonBack == KEY_DOWN) && GOD == false)
		GOD = true;
	if ((App->input->keyboard[SDL_SCANCODE_F3] == KEY_DOWN || App->input->buttonStart == KEY_DOWN) && GOD == true)
		GOD = false;

	if (App->player->position.y <= 1405 - 2656 && App->player->position.y >= 1338 - 2666) {
		current_animation = &invisible;
	}

	//GRENADE
	//Grenades must be retouched because now if player throws 2 grenades, only kills the last one

	if (cooldown == false && non_grenade == false && (App->input->keyboard[SDL_SCANCODE_LSHIFT] == KEY_STATE::KEY_DOWN || App->input->buttonB == KEY_STATE::KEY_DOWN) && App->UI->grenade > 0 && (position.y >= 1405 - 2656 || App->player->position.y <= 1338 - 2666))
	{
		cooldown = true;
		non_grenade = false;
		check_grenade = 0;
		update_position_grenade = 0;
		App->UI->grenade--;

		//Reset player animation throwing a grenade
		grenade.Reset();
		grenade.loops = 0;

		current_animation = &grenade;

		//Reset grenade
		App->particlesgrenade->grenade.anim.Reset();
		App->particlesgrenade->grenade.anim.loops = 0;

		App->particlesgrenade->grenade.speed.x = 0;
		App->particlesgrenade->grenade.speed.y = -2;

		App->particlesgrenade->AddParticle(App->particlesgrenade->grenade, position.x, position.y, COLLIDER_PLAYER_GRENADE, NULL);
		GOD = true;
	}

	update_position_grenade += App->particlesgrenade->grenade.speed.y;

	if (update_position_grenade == -110) {
		GOD = false;
		App->particlesgrenade1->grenade.speed.x = 0;
		App->particlesgrenade1->grenade.speed.y = +1;
		App->particlesgrenade1->AddParticle(App->particlesgrenade1->grenade, position.x, position.y - 110, COLLIDER_PLAYER_GRENADE, NULL);
		App->audio->play_fx5();
		cooldown = false;
		if ((detectionitem[0] == true || detectionitem[3] == true) && granadeUp == true)
			granade = true;
	}

	if (grenade.loops == 1) {
		current_animation = &forward;
		grenade.loops = 0;
		check_grenade = 1;
	}

	allTime = SDL_GetTicks();
	if (granade == false)
		finalTime = SDL_GetTicks();
	if (granade == true)
		allTime -= finalTime;

	if (granade == true) {
		if (allTime > 150 && allTime < 250 && (detectionitem[0] == true || detectionitem[3] == true))
			App->render->DrawQuad(App->player->R, 196, 0, 255, 0);

		else if (allTime > 250 && allTime < 400 && (detectionitem[0] == true || detectionitem[3] == true)) {
			App->render->DrawQuad(App->player->R, 196, 0, 255, 90);
			grenadeC = App->collision->AddCollider({ 0, position.y - 200, SCREEN_WIDTH, SCREEN_HEIGHT + 100 }, COLLIDER_END_OF_GRENADE, this);
		}
		else if (allTime > 400 && allTime < 425 && (detectionitem[0] == true || detectionitem[3] == true))
			App->render->DrawQuad(App->player->R, 196, 0, 255, 90);
		else if (allTime > 425 && allTime < 450 && (detectionitem[0] == true || detectionitem[3] == true))
			App->render->DrawQuad(App->player->R, 196, 0, 255, 0);
		else if (allTime > 450 && allTime < 475 && (detectionitem[0] == true || detectionitem[3] == true))
			App->render->DrawQuad(App->player->R, 196, 0, 255, 90);
		else if (allTime > 475 && allTime < 500 && (detectionitem[0] == true || detectionitem[3] == true))
			App->render->DrawQuad(App->player->R, 196, 0, 255, 0);

		else if (allTime > 500 && allTime < 650 && (detectionitem[0] == true || detectionitem[3] == true))
			App->render->DrawQuad(App->player->R, 196, 0, 255, 90);

		else if (allTime > 650 && allTime < 750 && (detectionitem[0] == true || detectionitem[3] == true))
			App->render->DrawQuad(App->player->R, 196, 0, 255, 0);

		else if (allTime > 750 && allTime < 900 && (detectionitem[0] == true || detectionitem[3] == true))
			App->render->DrawQuad(App->player->R, 196, 0, 255, 90);

		else if (allTime > 900 && allTime < 1000 && (detectionitem[0] == true || detectionitem[3] == true)) {
			App->render->DrawQuad(App->player->R, 196, 0, 255, 0);
			granade = false;
			granadeUp = false;
		}
		if (grenadeC != nullptr)
			grenadeC->to_delete = true;
	}

	//_grenade_end
	
	if (((App->input->keyboard[SDL_SCANCODE_W] == KEY_REPEAT || App->input->dpadUp == KEY_STATE::KEY_REPEAT || App->input->joy_up == KEY_STATE::KEY_REPEAT))&& move == true && check_grenade == 1 && collW == false && move2 == true)
	{
		if (App->player->position.y <= 1405 - 2656 && App->player->position.y >= 1338 - 2666)
			current_animation = &invisible;
		else {
			current_animation = &forward;
			forward.Start();
		}
		
		position.y -= speed;
	}

	else if (App->input->keyboard[SDL_SCANCODE_W] == KEY_UP || App->input->dpadUp == KEY_STATE::KEY_UP || App->input->joy_up == KEY_STATE::KEY_UP)
	{
		forward.Stop();
	}

	if (((App->input->keyboard[SDL_SCANCODE_S] == KEY_REPEAT || App->input->dpadDown == KEY_STATE::KEY_REPEAT || App->input->joy_down == KEY_STATE::KEY_REPEAT)) && move == true && check_grenade == 1 && collS == false && move2 == true)
	{
		if (App->player->position.y <= 1405 - 2656 && App->player->position.y >= 1338 - 2666)
			current_animation = &invisible;
		else {
			current_animation = &backward;
			backward.Start();
		}


		if (App->fade->on == App->scene_1) {
			if (App->scene_1->start1) {
				if (App->player->position.y <= (110 - App->scene_1->cont + 88)) {
					position.y += speed;
				}
			}
			else if (App->scene_1->start2) {
				if (App->player->position.y <= (2294 - 2656 - App->scene_1->cont + 88)) {
					position.y += speed;
				}
			}
			else if (App->scene_1->start3) {
				if (App->player->position.y <= (1836 - 2656 - App->scene_1->cont + 88)) {
					position.y += speed;
				}
			}
			else if (App->scene_1->start4) {
				if (App->player->position.y <= (882 - 2656 - App->scene_1->cont + 88)) {
					position.y += speed;
				}
			}
			else if (App->scene_1->start5) {
				if (App->player->position.y <= (395 - 2656 - App->scene_1->cont + 88)) {
					position.y += speed;
				}
			}
			else {
				if (App->player->position.y <= (110 - App->scene_1->cont + 88)) {
					position.y += speed;
				}
			}
		}
		else if (App->fade->on == App->room1A || App->fade->on == App->roomB || App->fade->on == App->roomC || App->fade->on == App->roomD || App->fade->on == App->roomE) {
			position.y += speed;
		}
	}

	else if (App->input->keyboard[SDL_SCANCODE_S] == KEY_UP || App->input->dpadDown == KEY_STATE::KEY_UP || App->input->joy_down == KEY_STATE::KEY_UP)
	{
		backward.Stop();
	}

	if (((App->input->keyboard[SDL_SCANCODE_D] == KEY_REPEAT || App->input->dpadRight == KEY_STATE::KEY_REPEAT || App->input->joy_right == KEY_STATE::KEY_REPEAT)) && position.x < (SCREEN_WIDTH - right.frames->w) && move == true && check_grenade == 1 && collD == false && move2 == true)
	{
		if (App->player->position.y <= 1405 - 2656 && App->player->position.y >= 1338 - 2666)
			current_animation = &invisible;
		else {
			current_animation = &right;
			right.Start();
		}
		position.x += speed;
	}

	else if (App->input->keyboard[SDL_SCANCODE_D] == KEY_UP || App->input->dpadRight == KEY_STATE::KEY_UP || App->input->joy_right == KEY_STATE::KEY_UP)
	{
		right.Stop();
	}
	if (((App->input->keyboard[SDL_SCANCODE_A] == KEY_REPEAT || App->input->dpadLeft == KEY_STATE::KEY_REPEAT || App->input->joy_left == KEY_STATE::KEY_REPEAT)) && position.x > 0 && move == true && check_grenade == 1 && collA == false && move2 == true)
	{
		if (App->player->position.y <= 1405 - 2656 && App->player->position.y >= 1338 - 2666)
			current_animation = &invisible;
		else {
			current_animation = &left;
			left.Start();
		}
		position.x -= speed;
	}
	else if (App->input->keyboard[SDL_SCANCODE_A] == KEY_UP || App->input->dpadLeft == KEY_STATE::KEY_UP || App->input->joy_left == KEY_STATE::KEY_UP)
	{
		left.Stop();
	}
	if (((App->input->keyboard[SDL_SCANCODE_W] == KEY_REPEAT && App->input->keyboard[SDL_SCANCODE_D] == KEY_REPEAT) || ((App->input->dpadUp == KEY_STATE::KEY_REPEAT || App->input->joy_up == KEY_STATE::KEY_REPEAT) && (App->input->dpadRight == KEY_STATE::KEY_REPEAT || App->input->joy_right == KEY_STATE::KEY_REPEAT))) && move == true && check_grenade == 1 && move2 == true) {

		if (App->player->position.y <= 1405 - 2656 && App->player->position.y >= 1338 - 2666)
			current_animation = &invisible;
		else {
			current_animation = &diagWD;
			diagWD.Start();
		}


	}

	if ((App->input->keyboard[SDL_SCANCODE_W] == KEY_UP && App->input->keyboard[SDL_SCANCODE_D] == KEY_UP) || (App->input->dpadUp == KEY_STATE::KEY_UP || App->input->joy_up == KEY_STATE::KEY_UP) && (App->input->dpadRight == KEY_STATE::KEY_UP || App->input->joy_right == KEY_STATE::KEY_UP))
	{
		diagWD.Stop();
	}

	if (((App->input->keyboard[SDL_SCANCODE_A] == KEY_REPEAT && App->input->keyboard[SDL_SCANCODE_W] == KEY_REPEAT) || ((App->input->dpadUp == KEY_STATE::KEY_REPEAT || App->input->joy_up == KEY_STATE::KEY_REPEAT) && (App->input->dpadLeft == KEY_STATE::KEY_REPEAT || App->input->joy_left == KEY_STATE::KEY_REPEAT))) && move == true && check_grenade == 1 && move2 == true ) {
		if (App->player->position.y <= 1405 - 2656 && App->player->position.y >= 1338 - 2666)
			current_animation = &invisible;
		else {
			current_animation = &diagWA;
		}
		diagWA.Start();

	}
	if ((App->input->keyboard[SDL_SCANCODE_W] == KEY_UP && App->input->keyboard[SDL_SCANCODE_A] == KEY_UP) || (App->input->dpadUp == KEY_STATE::KEY_UP || App->input->joy_up == KEY_STATE::KEY_UP) && (App->input->dpadLeft == KEY_STATE::KEY_UP || App->input->joy_left == KEY_STATE::KEY_UP))
	{
		diagWA.Stop();
	}

	if (((App->input->keyboard[SDL_SCANCODE_S] == KEY_REPEAT && App->input->keyboard[SDL_SCANCODE_D] == KEY_REPEAT) || ((App->input->dpadDown == KEY_STATE::KEY_REPEAT || App->input->joy_down == KEY_STATE::KEY_REPEAT) && (App->input->dpadRight == KEY_STATE::KEY_REPEAT || App->input->joy_right == KEY_STATE::KEY_REPEAT))) && move == true && check_grenade == 1 && move2 == true) {
		if (App->player->position.y <= 1405 - 2656 && App->player->position.y >= 1338 - 2666)
			current_animation = &invisible;
		else {
			current_animation = &diagSD;
		}
		diagSD.Start();

	}
	if ((App->input->keyboard[SDL_SCANCODE_S] == KEY_UP && App->input->keyboard[SDL_SCANCODE_D] == KEY_UP) || (App->input->dpadDown == KEY_STATE::KEY_UP || App->input->joy_down == KEY_STATE::KEY_UP) && (App->input->dpadRight == KEY_STATE::KEY_UP || App->input->joy_right == KEY_STATE::KEY_UP))
	{
		diagSD.Stop();
	}

	if (((App->input->keyboard[SDL_SCANCODE_S] == KEY_REPEAT && App->input->keyboard[SDL_SCANCODE_A] == KEY_REPEAT) || ((App->input->dpadDown == KEY_STATE::KEY_REPEAT || App->input->joy_down == KEY_STATE::KEY_REPEAT) && (App->input->dpadLeft == KEY_STATE::KEY_REPEAT || App->input->joy_left == KEY_STATE::KEY_REPEAT))) && move == true && check_grenade == 1 && move2 == true) {
		if (App->player->position.y <= 1405 - 2656 && App->player->position.y >= 1338 - 2666)
			current_animation = &invisible;
		else {
			current_animation = &diagSA;
		}
		diagSA.Start();
	}
	if ((App->input->keyboard[SDL_SCANCODE_S] == KEY_UP && App->input->keyboard[SDL_SCANCODE_A] == KEY_UP) || (App->input->dpadDown == KEY_STATE::KEY_UP || App->input->joy_down == KEY_STATE::KEY_UP) && (App->input->dpadLeft == KEY_STATE::KEY_UP || App->input->joy_left == KEY_STATE::KEY_UP))
	{
		diagSA.Stop();
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



	if ((App->input->keyboard[SDL_SCANCODE_SPACE] == KEY_STATE::KEY_DOWN || App->input->buttonA == KEY_STATE::KEY_DOWN) && current_animation == &diagSA && move2) //Esquerra Abaix
	{
		App->particles->bala.speed.y = +6;
		App->particles->bala.speed.x = -6;
		App->particles->balaUp.speed.y = +6;
		App->particles->balaUp.speed.x = -6;
		Shot();
	}
	else if ((App->input->keyboard[SDL_SCANCODE_SPACE] == KEY_STATE::KEY_DOWN || App->input->buttonA == KEY_STATE::KEY_DOWN) && current_animation == &diagSD && move2)//Dreta Abaix
	{
		App->particles->bala.speed.y = +6;
		App->particles->bala.speed.x = +6;
		App->particles->balaUp.speed.y = +6;
		App->particles->balaUp.speed.x = +6;
		Shot();
	}
	else if ((App->input->keyboard[SDL_SCANCODE_SPACE] == KEY_STATE::KEY_DOWN || App->input->buttonA == KEY_STATE::KEY_DOWN) && current_animation == &diagWD && move2)//Dreta Adalt
	{
		App->particles->bala.speed.y = -6;
		App->particles->bala.speed.x = +6;
		App->particles->balaUp.speed.y = -6;
		App->particles->balaUp.speed.x = +6;
		Shot();
	}
	else if ((App->input->keyboard[SDL_SCANCODE_SPACE] == KEY_STATE::KEY_DOWN || App->input->buttonA == KEY_STATE::KEY_DOWN) && current_animation == &diagWA && move2) //Esquerra Adalt
	{
		App->particles->bala.speed.y = -6;
		App->particles->bala.speed.y = -6;
		App->particles->balaUp.speed.y = -6;
		App->particles->balaUp.speed.y = -6;
		Shot();
	}
	else if ((App->input->keyboard[SDL_SCANCODE_SPACE] == KEY_STATE::KEY_DOWN || App->input->buttonA == KEY_STATE::KEY_DOWN) && current_animation == &forward && move2) //Adalt
	{
		App->particles->bala.speed.y = -6;
		App->particles->bala.speed.x = 0;
		App->particles->balaUp.speed.y = -6;
		App->particles->balaUp.speed.x = 0;
		Shot();
	}

	else if ((App->input->keyboard[SDL_SCANCODE_SPACE] == KEY_STATE::KEY_DOWN || App->input->buttonA == KEY_STATE::KEY_DOWN) && current_animation == &backward && move2) //Abaix
	{
		App->particles->bala.speed.y = +6;
		App->particles->bala.speed.x = 0;
		App->particles->balaUp.speed.y = +6;
		App->particles->balaUp.speed.x = 0;
		Shot();
	}
	else if ((App->input->keyboard[SDL_SCANCODE_SPACE] == KEY_STATE::KEY_DOWN || App->input->buttonA == KEY_STATE::KEY_DOWN) && current_animation == &right && move2) //Dreta
	{
		App->particles->bala.speed.x = +6;
		App->particles->bala.speed.y = 0;
		App->particles->balaUp.speed.x = +6;
		App->particles->balaUp.speed.y = 0;
		Shot();
	}
	else if ((App->input->keyboard[SDL_SCANCODE_SPACE] == KEY_STATE::KEY_DOWN || App->input->buttonA == KEY_STATE::KEY_DOWN) && current_animation == &left && move2) //Esquer0a
	{
		App->particles->bala.speed.x = -6;
		App->particles->bala.speed.y = 0;
		App->particles->balaUp.speed.x = -6;
		App->particles->balaUp.speed.y = 0;
		Shot();

	}
	else if ((App->input->keyboard[SDL_SCANCODE_SPACE] == KEY_STATE::KEY_DOWN || App->input->buttonA == KEY_STATE::KEY_DOWN) && move == true && move2)
	{
		Shot();
	}

	r = current_animation->GetCurrentFrame();

	collrev->SetPos(position.x - 12, position.y - 10);
	coll->SetPos(position.x + 2, position.y + 4);
	feetC->SetPos(position.x + 4, position.y + 19);

	if (move2) {
		follow_p->SetPos(position.x - 50, position.y - 50);
		follow_p1->SetPos(position.x + 20, position.y - 50);
		follow_p2->SetPos(position.x - 50, position.y - 50);
		follow_p3->SetPos(position.x - 50, position.y + 25);
	}
	else if (move2 == false && App->player2->move2) {
		follow_p->SetPos(App->player2->position.x - 50, App->player2->position.y - 50);
		follow_p1->SetPos(App->player2->position.x + 20, App->player2->position.y - 50);
		follow_p2->SetPos(App->player2->position.x - 50, App->player2->position.y - 50);
		follow_p3->SetPos(App->player2->position.x - 50, App->player2->position.y + 25);
	}

	if (stairsUp == false) {
		current_animation = &stairsRoom;
		move = false;
	}
	if (waterB == false || stairsDown == false) {
		current_animation = &waterDie;
		move = false;
	}
	currentTime = SDL_GetTicks();
	currentTime -= lastTime;
	if (waterB == true && stairsDown == true && stairsUp == true)
		lastTime = SDL_GetTicks();

	if (currentTime > 500 && waterB == false) {
		current_animation = &invisible;
		timeW = true;
	}
	if (currentTime > 300 && (stairsDown == false || stairsUp == false)) {
		App->player->current_animation = &invisible;
	}

	if (currentTime > 500 && (stairsDown == false || stairsUp == false)) {
		App->player->current_animation = &App->player->forward;
		App->player->move = true;
	}
	if (enemyB == false) {
		current_animation = &die;
		move = false;
	}
	App->render->Blit(graphics, position.x, position.y, &r);

	

	collW = false;
	collA = false;
	collS = false;
	collD = false;

	//print help
	if (move2 == false) {
		detectionlive = 1;
		App->render->Blit(App->player2->graphics, position.x + 10, position.y, &help);
		App->render->Blit(App->player2->graphics, position.x - 2, position.y + 5, &dead2player);
		current_animation = &invisible;
		contdead = 1;
	}
	else if (move2 == true && contdead == 1) {
		current_animation = &idleF;
		contdead = 0;
		detectionlive = 0;
	}

	if (App->input->keyboard[SDL_SCANCODE_N] == KEY_STATE::KEY_DOWN)
		App->UI->grenade++;

	else if (App->input->keyboard[SDL_SCANCODE_M] == KEY_STATE::KEY_DOWN)
		App->player2->torevive++;

	return UPDATE_CONTINUE;
}

void ModulePlayer::OnCollision(Collider* c1, Collider* c2)
{
	if ((c1->type == COLLIDER_PLAYER || c1->type == COLLIDER_PLAYER_FEET) && c2->type == COLLIDER_WALL && GOD == false)
		OnCollisionWall(c1, c2);
	if ((c1->type == COLLIDER_PLAYER || c1->type == COLLIDER_PLAYER_FEET) && c2->type == COLLIDER_WATER && GOD == false)
		OnCollisionWater(c1, c2);
	if ((c1->type == COLLIDER_PLAYER_FEET || c1->type == COLLIDER_PLAYER) && c2->type == COLLIDER_ITEM && App->fade->on == App->scene_1)
		OnCollisionItem(c1, c2);
	if ((c1->type == COLLIDER_PLAYER && (c2->type == COLLIDER_ENEMY || c2->type == COLLIDER_ENEMY_SHOT) && GOD == false) && detectionlive == 0)
		OnCollisionEnemy(c1, c2);
	if ((c1->type == COLLIDER_PLAYER_FEET || c1->type == COLLIDER_PLAYER)&& c2->type == COLLIDER_SECRET_ROOM)
		OnCollisionSecretRooms(c1, c2);
	if (c1->type == COLLIDER_PLAYER && c2->type == COLLIDER_REVIVE2 && App->player2->move2 == false)
		OnCollisionRev(c1, c2);

}

void ModulePlayer::OnCollisionWall(Collider* c1, Collider* c2)
{

	if ((c1->rect.x + c1->rect.w) - c2->rect.x != 1 && (c2->rect.x + c2->rect.w) - c1->rect.x != 1 && (c2->rect.y + c2->rect.h) - c1->rect.y == 1 && (c1->rect.y + c1->rect.h) - c2->rect.y != 1)

		collW = true;

	if ((c1->rect.x + c1->rect.w) - c2->rect.x != 1 && (c2->rect.x + c2->rect.w) - c1->rect.x == 1 && (c2->rect.y + c2->rect.h) - c1->rect.y != 1 && (c1->rect.y + c1->rect.h) - c2->rect.y != 1)

		collA = true;

	if ((c1->rect.x + c1->rect.w) - c2->rect.x != 1 && (c2->rect.x + c2->rect.w) - c1->rect.x != 1 && (c2->rect.y + c2->rect.h) - c1->rect.y != 1 && (c1->rect.y + c1->rect.h) - c2->rect.y == 1)

		collS = true;

	if ((c1->rect.x + c1->rect.w) - c2->rect.x == 1 && (c2->rect.x + c2->rect.w) - c1->rect.x != 1 && (c2->rect.y + c2->rect.h) - c1->rect.y != 1 && (c1->rect.y + c1->rect.h) - c2->rect.y != 1)

		collD = true;
}



void ModulePlayer::OnCollisionItem(Collider* c1, Collider* c2) {


	App->audio->play_fx7();

	if (App->player->position.y < 2655 - 2656 && App->player->position.y > 2547 - 2656) {
		detectionitem[0] = true;
		granadeUp = true;
	}
	if (App->player->position.y < 2547 - 2656 && App->player->position.y > 2340 - 2656) {
		detectionitem[1] = true;
		App->UI->grenade++;
	}
	if (App->player->position.y < 2016 - 2656 && App->player->position.y > 1829 - 2656) {
		detectionitem[2] = true;
		App->UI->grenade++;
	}
	if (App->player->position.y < 1269 - 2656 && App->player->position.y > 1104 - 2656) {
		detectionitem[3] = true;
		granadeUp = true;
	}
	if (App->player->position.y < 1041 - 2656 && App->player->position.y > 896 - 2656) {
		detectionitem[4] = true;
		App->UI->grenade++;
	}
	if (App->player->position.y < 792 - 2656 && App->player->position.y > 613 - 2656) {
		detectionitem[5] = true;
		App->UI->grenade++;
	}
	if (App->player->position.y < 577 - 2656 && App->player->position.y > 416 - 2656) {
		detectionitem[6] = true;
		App->UI->grenade++;
	}
	if (App->player->position.y < 380 - 2656 && App->player->position.y > 212 - 2656) {
		detectionitem[7] = true;
		App->UI->grenade++;
	}
	else if (App->player->position.y < 1836 - 2656 && App->player->position.y > 1621 - 2656) {
		App->fade->FadeToBlack(App->scene_1, App->CinematicRadio, 1);
		move = false;
	}
	App->UI->grenade++;
	c2->to_delete = true;

}

void ModulePlayer::OnCollisionWater(Collider* c1, Collider* c2) {
	if (App->player2->twoplayerson == false) {
		non_grenade = true;
		App->ending->cont = 0;
		play_ending = true;

		if (App->fade->IsFading() == false)
		{
			if (vides != 0) {
				waterB = false;
				if (timeW == true) {
					vides--;
					App->audio->pause_music();
					App->audio->play_fx6();
					App->particles->AddParticle(App->particles->explosion, position.x + 3, position.y + 5, COLLIDER_END_OF_BULLET, NULL);
					App->scene_1->start = false;
					save_player_position = position.y;

					if (save_player_position < 110 && save_player_position >= 2529 - 2656) {
						App->scene_1->start = false;
						App->scene_1->start1 = true;
						App->scene_1->start2 = false;
						App->scene_1->start3 = false;
						App->scene_1->start4 = false;
						App->scene_1->start5 = false;
					}
					else if (save_player_position < 2529 - 2656 && save_player_position >= 2003 - 2656) {
						App->scene_1->start = false;
						App->scene_1->start1 = false;
						App->scene_1->start2 = true;
						App->scene_1->start3 = false;
						App->scene_1->start4 = false;
						App->scene_1->start5 = false;
					}
					else if (save_player_position < 2003 - 2656 && save_player_position > 1235 - 2656) {
						App->scene_1->start = false;
						App->scene_1->start1 = false;
						App->scene_1->start2 = false;
						App->scene_1->start3 = true;
						App->scene_1->start4 = false;
						App->scene_1->start5 = false;
					}
					else if (save_player_position < 1235 - 2656 && save_player_position > 378 - 2656) {
						App->scene_1->start = false;
						App->scene_1->start1 = false;
						App->scene_1->start2 = false;
						App->scene_1->start3 = false;
						App->scene_1->start4 = true;
						App->scene_1->start5 = false;
					}
					else if (save_player_position < 378 - 2656 && save_player_position > -2656) {
						App->scene_1->start = false;
						App->scene_1->start1 = false;
						App->scene_1->start2 = false;
						App->scene_1->start3 = false;
						App->scene_1->start4 = false;
						App->scene_1->start5 = true;
					}
					else {
						App->scene_1->start = true;
						App->scene_1->start1 = false;
						App->scene_1->start2 = false;
						App->scene_1->start3 = false;
						App->scene_1->start4 = false;
						App->scene_1->start5 = false;
					}

					App->fade->FadeToBlack(App->scene_1, App->scene_1);
				}
			}
			else if (vides == 0) {
				waterB = false;
				if (timeW == true) {
					App->audio->pause_music();
					App->audio->play_fx6();
					App->particles->AddParticle(App->particles->explosion, position.x + 3, position.y + 5, COLLIDER_END_OF_BULLET, NULL);
					App->scene_1->start = true;
					App->fade->FadeToBlack(App->scene_1, App->ending);
				}
				vides = 3;
			}
		}
	}
	else {
		vides = 0;
		non_grenade = true;
		play_ending = true;
		waterB = false;
		App->player2->checkwaterdead = true;
		App->player2->checkwaterdead3 = true;
	}
	GunPowerUp = false;
}
void ModulePlayer::OnCollisionEnemy(Collider* c1, Collider* c2) {

	if (App->player2->twoplayerson == false) {
		non_grenade = true;
		play_ending = true;
		App->ending->cont = 0;

		if (App->fade->IsFading() == false)
		{

			if (vides != 0) {

				App->player->follow_p->to_delete = true;
				App->player->follow_p1->to_delete = true;
				App->player->follow_p2->to_delete = true;
				App->player->follow_p3->to_delete = true;

				vides--;
				App->audio->pause_music();
				App->audio->play_fx6();
				App->scene_1->start = false;
				save_player_position = position.y;

				if (save_player_position < 110 && save_player_position >= 2529 - 2656) {
					App->scene_1->start = false;
					App->scene_1->start1 = true;
					App->scene_1->start2 = false;
					App->scene_1->start3 = false;
					App->scene_1->start4 = false;
					App->scene_1->start5 = false;
				}
				else if (save_player_position < 2529 - 2656 && save_player_position >= 2003 - 2656) {
					App->scene_1->start = false;
					App->scene_1->start1 = false;
					App->scene_1->start2 = true;
					App->scene_1->start3 = false;
					App->scene_1->start4 = false;
					App->scene_1->start5 = false;
				}
				else if (save_player_position < 2003 - 2656 && save_player_position > 1235 - 2656) {
					App->scene_1->start = false;
					App->scene_1->start1 = false;
					App->scene_1->start2 = false;
					App->scene_1->start3 = true;
					App->scene_1->start4 = false;
					App->scene_1->start5 = false;
				}
				else if (save_player_position < 1235 - 2656 && save_player_position > 378 - 2656) {
					App->scene_1->start = false;
					App->scene_1->start1 = false;
					App->scene_1->start2 = false;
					App->scene_1->start3 = false;
					App->scene_1->start4 = true;
					App->scene_1->start5 = false;
				}
				else if (save_player_position < 378 - 2656 && save_player_position > -2656) {
					App->scene_1->start = false;
					App->scene_1->start1 = false;
					App->scene_1->start2 = false;
					App->scene_1->start3 = false;
					App->scene_1->start4 = false;
					App->scene_1->start5 = true;
				}
				else {
					App->scene_1->start = true;
					App->scene_1->start1 = false;
					App->scene_1->start2 = false;
					App->scene_1->start3 = false;
					App->scene_1->start4 = false;
					App->scene_1->start5 = false;
				}
				if (App->fade->on == App->roomD) {
					App->fade->FadeToBlack(App->roomD, App->scene_1, 5.0f);
					App->scene_1->start = false;
					App->scene_1->start1 = false;
					App->scene_1->start2 = false;
					App->scene_1->start3 = false;
					App->scene_1->start4 = true;
					App->scene_1->start5 = false;
				}
					
				App->fade->FadeToBlack(App->scene_1, App->scene_1, 5.0f);
				enemyB = false;


			}

			else if (vides == 0) {

				App->player->follow_p->to_delete = true;
				App->player->follow_p1->to_delete = true;
				App->player->follow_p2->to_delete = true;
				App->player->follow_p3->to_delete = true;

				App->audio->pause_music();
				App->audio->play_fx6();
				App->scene_1->start = true;
				App->fade->FadeToBlack(App->scene_1, App->ending, 5.0f);
				enemyB = false;
				vides = 3;
			}
		}
	}
	else {
		move2 = false;
		randomsound = rand() % 2;
		if (randomsound == 1)
			App->audio->play_fx16();
		else
			App->audio->play_fx19();
		//vides--;
		if (vides != 0) {
			vides--;
		}

	}
	GunPowerUp = false;
}

void ModulePlayer::OnCollisionSecretRooms(Collider* c1, Collider* c2) {

	if (App->fade->on == App->scene_1) {
		if (App->scene_1->roomA == true) {
			App->fade->FadeToBlack(App->scene_1, App->room1A, 1);
			stairsDown = false;
			position.x = 150;
			position.y = 2372 - 2656;
			App->scene_1->roomA = false;

		}
		else if (App->scene_1->roomB == true) {
			App->fade->FadeToBlack(App->scene_1, App->roomB, 1);
			stairsDown = false;
			position.x = 11;
			position.y = 2051 - 2656;
			App->scene_1->roomB = false;

		}
		else if (App->scene_1->roomC == true) {
			App->fade->FadeToBlack(App->scene_1, App->roomC, 1);
			stairsDown = false;
			position.x = 217;
			position.y = 1426 - 2656;
			App->scene_1->roomC = false;

		}
		else if (App->scene_1->roomD == true) {
			App->fade->FadeToBlack(App->scene_1, App->roomD, 1);
			stairsDown = false;
			position.x = 121;
			position.y = 1056 - 2656;
			App->scene_1->roomD = false;
		}
		else if (App->scene_1->roomE == true) {
			App->fade->FadeToBlack(App->scene_1, App->roomE, 1);
			stairsDown = false;
			position.x = 233;
			position.y = 522 - 2656;
			App->scene_1->roomE = false;
		}
	}

	else if (stairsDown == true) {
		if (App->fade->on == App->room1A) {
			App->fade->FadeToBlack(App->room1A, App->scene_1, 1);
			stairsUp = false;
			position.x = 169;
			position.y = 31;
			App->scene_1->start = false;
			App->scene_1->start1 = false;
			App->scene_1->start2 = true;
			App->scene_1->start3 = false;
			App->scene_1->start4 = false;
			App->scene_1->start5 = false;
		}
		else if (App->fade->on == App->roomB) {
			App->fade->FadeToBlack(App->roomB, App->scene_1, 1);
			stairsUp = false;
			position.x = 216;
			position.y = 65;
			App->scene_1->start = false;
			App->scene_1->start1 = false;
			App->scene_1->start2 = false;
			App->scene_1->start3 = true;
			App->scene_1->start4 = false;
			App->scene_1->start5 = false;
		}
		else if (App->fade->on == App->roomC) {
			App->fade->FadeToBlack(App->roomC, App->scene_1, 1);
			stairsUp = false;
			position.x = 121;
			position.y = 80;
			App->scene_1->start = false;
			App->scene_1->start1 = false;
			App->scene_1->start2 = false;
			App->scene_1->start3 = true;
			App->scene_1->start4 = false;
			App->scene_1->start5 = false;
		}
		else if (App->fade->on == App->roomD) {
			App->fade->FadeToBlack(App->roomD, App->scene_1, 1);
			stairsUp = false;
			position.x = 121;
			position.y = 32;
			App->scene_1->start = false;
			App->scene_1->start1 = false;
			App->scene_1->start2 = false;
			App->scene_1->start3 = false;
			App->scene_1->start4 = true;
			App->scene_1->start5 = false;
		}
		else if (App->fade->on == App->roomE) {
			App->fade->FadeToBlack(App->roomE, App->scene_1, 1);
			stairsUp = false;
			position.x = 121;
			position.y = 32;
			App->scene_1->start = false;
			App->scene_1->start1 = false;
			App->scene_1->start2 = false;
			App->scene_1->start3 = false;
			App->scene_1->start4 = false;
			App->scene_1->start5 = true;
		}

	}

}

void ModulePlayer::OnCollisionRev(Collider* c1, Collider* c2)
{
	if (App->player2->torevive != 0) {
		if (App->input->keyboard[SDL_SCANCODE_9] == KEY_STATE::KEY_REPEAT || App->input->buttonX == KEY_STATE::KEY_REPEAT) {
			App->player2->revTime++;

			if (App->player2->revTime <= 30 && App->player2->revTime >= 1) {
				//if (revTime == 2)
					App->audio->play_fx11();
				App->render->Blit(App->player2->graphics, App->player2->position.x - 5, App->player2->position.y, &App->player2->hpRev1);
			}
			else if (App->player2->revTime <= 60 && App->player2->revTime > 30) {
				//if(revTime ==32)
					App->audio->play_fx12();
				App->render->Blit(App->player2->graphics, App->player2->position.x - 5, App->player2->position.y, &App->player2->hpRev2);
			}
			else if (App->player2->revTime <= 100 && App->player2->revTime > 60) {
				//if(revTime == 62)
					App->audio->play_fx13();

				App->render->Blit(App->player2->graphics, App->player2->position.x - 5, App->player2->position.y, &App->player2->hpRev3);
			}
			else if (App->player2->revTime <= 130 && App->player2->revTime > 100) {
			//	if(revTime == 102)
					App->audio->play_fx14();

				App->render->Blit(App->player2->graphics, App->player2->position.x - 5, App->player2->position.y, &App->player2->hpRev4);
			}
			if (App->player2->revTime >= 130) {
				App->player2->move2 = true;
				App->player2->revTime = 0;
				App->player2->torevive--;
				App->audio->play_fx17();
				
			}
		}
		if (App->input->keyboard[SDL_SCANCODE_9] == KEY_STATE::KEY_UP || App->input->buttonX == KEY_STATE::KEY_UP)
			App->player2->revTime = 0;
	}
	else {}
}

void ModulePlayer::Shot() {
	if (GunPowerUp == false)
		App->particles->AddParticle(App->particles->bala, position.x, position.y, COLLIDER_PLAYER_SHOT, NULL);
	if(GunPowerUp == true)
		App->particles->AddParticle(App->particles->balaUp, position.x, position.y, COLLIDER_PLAYER_SHOT, NULL);
	App->audio->play_fx1();
}