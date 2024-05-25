#include "Main.h"
#include "Game.h"
#include "Hit.h"
const float PLAYER_SPEED = 8.0f;
const float JUMP_HEIGHT = 120.0f;
const float JUMP_SPEED = 10.0f;
const float GROUND = 450.0f;
const int TARGET_NUM = 8;
const int SHADOW_TIMES = 100;
const int HP_MAX = 3;
const int TIME_MAX = 3600;

//	プレイヤー用の変数
int player_image;	//	画像
float player_x;		//	Ｘ座標
float player_y;		//	Ｙ座標
float rota;
float rota_record[100];
float player_position_y;
bool isDamage;
int timeInvi;
int NowHP;
int player_HPx[HP_MAX];
int player_HPy[HP_MAX];

//	一応オプションの数用の定数
const int OPTION_MAX = 4;

//	オプション用の変数
int option_image;	//	画像
float shadow_x[4];		//	Ｘ座標
float shadow_y[4];		//	Ｙ座標
float rota_option[4];

//Jump
int jump_time;
bool isJumping;
bool isDoubleJump;
bool isGround;
float delay_jump_down;

//	プレイヤーの座標を記録していく用の変数
float record_x[SHADOW_TIMES];
float record_y[SHADOW_TIMES];
int delay;

//Screen
int background;
int back_ground2;
int screen_mode;
bool fade_effect;
int fade_alpha_x;
int fade_alpha_y;
int fade_speed;

//Enemy
float target_x[TARGET_NUM];
float target_y[TARGET_NUM];
int target_image;
int target_position_X;
int target_position_Y;
bool target_appear[TARGET_NUM];
float target_speed;
float target_rota[TARGET_NUM];

//Sound
int jump_sound;
int hit_sound;
int appear_sound;
int volume;

//Orther
int time_s;
int textWidth;
int textX;
int textY;
int color[3];
int mouseX, mouseY;


enum Target_Y {
	TARGET_BOTTOM_UPSIDE,
	TARGET_BOTTOM_MIDDLE,
	TARGET_MIDDLE_UPSIDE,

};

enum Target_X {
	TARGET_LEFT,
	TARGET_RIGHT,
	TARGET_LEFT_RIGHT,
};

enum SCREEN {
	BEGIN,
	INGAME,
	GAME_CLEAR,
	GAME_OVER,
};

void GetTextLength(const char* text, int size, int textx, int texty) {
	textWidth = GetDrawStringWidth(text, strlen(text), size);
	textX = textx;
	textY = texty;
}

bool CheckMouseHitText(int mouse_x, int mouse_y, int x, int y, int size) {
	if (GetMouseInput() & MOUSE_INPUT_LEFT) {
		if (mouse_x >= x && mouse_x <= x + textWidth &&
			mouse_y >= y && mouse_y <= y + size) {
			return true;
		}
		else return false;
	}
}

bool IsMouseHitText(int mouse_x, int mouse_y, int x, int y, int size) {

	if (mouse_x >= x && mouse_x <= x + textWidth &&
		mouse_y >= y && mouse_y <= y + size) {
		return true;
	}
	else return false;
}

void ResetGame() {
	//reset target
	for (int i = 0; i < TARGET_NUM; i++)
	{
		target_x[i] = -30.0f;
		target_y[i] = 480.0f - i * 50;
		target_appear[i] = false;
	}
	target_position_Y = TARGET_BOTTOM_MIDDLE;
	target_position_X = TARGET_RIGHT;
	target_speed = 5.0f;
	//reset player
	player_x = SCREEN_W / 2;
	player_y = GROUND;
	isDamage = false;
	NowHP = HP_MAX;
	timeInvi = 100;
	for (int i = 0; i < NowHP; i++) {
		player_HPx[i] = 20 + i * 40;
		player_HPy[i] = 30;
		time_s = TIME_MAX;
	}
	//reset shadow
	for (int i = 0; i < 4; i++) {
		shadow_x[i] = SCREEN_W / 2;
		shadow_y[i] = GROUND;
	}
	for (int i = 0; i < SHADOW_TIMES; i++) {
		record_x[i] = player_x;
		record_y[i] = player_y;
	}
	//reset jump
	isJumping = false;
	isDoubleJump = false;
	jump_time = 0;
}

//---------------------------------------------------------------------------------
//	初期化処理
//---------------------------------------------------------------------------------
void GameInit()
{
IMAGE_INIT:
	{
		//Image Setting
		player_image = LoadGraph("data/player.jpg");
		option_image = LoadGraph("data/player.jpg");
		background = LoadGraph("data/background.jpg");
		target_image = LoadGraph("data/fireball3.png");
		back_ground2 = LoadGraph("data/start.jpg");
	}

SOUND_INIT:
	{
		jump_sound = LoadSoundMem("data/jump.mp3");
		hit_sound = LoadSoundMem("data/hit.mp3");
		appear_sound = LoadSoundMem("data/appear.mp3");
		volume = 80;
	}

PLAYER_INIT:
	{
		//Player Setting
		player_x = SCREEN_W / 2;
		player_y = GROUND;
		delay = 3;
		rota = 0.0f;
		player_position_y = player_y;
		isDamage = false;
		timeInvi = 100;
		NowHP = HP_MAX;
		for (int i = 0; i < NowHP; i++) {
			player_HPx[i] = 20 + i * 40;
			player_HPy[i] = 30;
		}
	}

JUMP_INIT:
	{
		//Jump Setting
		isJumping = false;
		isDoubleJump = false;
		jump_time = 0;
	}

TARGET_INIT:
	{
		//Enemy Setting

		for (int i = 0; i < TARGET_NUM; i++)
		{
			target_x[i] = -30.0f;
			target_y[i] = 480.0f - i * 50;
			target_rota[i] = 0.0f;
			target_appear[i] = false;
		}
		target_position_Y = TARGET_BOTTOM_MIDDLE;
		target_position_X = TARGET_RIGHT;
		target_speed = 5.0f;

	}

SHADOW_INIT:
	{
		//Shadow Setting
		for (int i = 0; i < 4; i++) {
			shadow_x[i] = SCREEN_W / 2;
			shadow_y[i] = GROUND;
		}

		for (int i = 0; i < SHADOW_TIMES; i++) {

			record_x[i] = player_x;
			record_y[i] = player_y;
		}
	}

SCREEN_INIT:
	{
		screen_mode = BEGIN;
		fade_effect = false;
		fade_alpha_x = 0;
		fade_alpha_y = 0;
		fade_speed = 10;
	}

ORTHER_INIT:
	{
		//Orther Setting
		isGround = true;
		delay_jump_down = 1.0f;
		time_s = TIME_MAX;
		for (int i = 0; i < 3; i++)
		{
			color[i] = GetColor(255, 0, 0);
		}
	}
}
//---------------------------------------------------------------------------------
//	更新処理
//---------------------------------------------------------------------------------
void MouseHitText(int i) {
	if (CheckMouseHitText(mouseX, mouseY, textX, textY, 24)) {
		fade_effect = true;
	}
	if (IsMouseHitText(mouseX, mouseY, textX, textY, 24)) {
		color[i] = GetColor(255, 0, 0);
	}
	else color[i] = GetColor(255, 255, 0);
}
void GameUpdate()
{
	mouseX = GetMouseX();
	mouseY = GetMouseY();

	switch (screen_mode)
	{
	case BEGIN:
	{

		GetTextLength("Game Start", 24, SCREEN_W / 2 - 80, SCREEN_H / 2 - 30);
		MouseHitText(0);
		if (fade_alpha_x > SCREEN_W && fade_effect == true) {
			screen_mode = INGAME;
		}
		GetTextLength("Exit Game", 24, SCREEN_W / 2 - 75, SCREEN_H / 2 + 20);
		MouseHitText(1);
		if (CheckMouseHitText(mouseX, mouseY, textX, textY, 24)) {
			exit('0');
		}
		break;
	}

	case INGAME:
	{
	GAME_CLEAR:
		{
			if (time_s <= 0) {

				fade_effect = true;
			}
			if (fade_alpha_x > SCREEN_W && fade_effect == true && time_s <= 0) {
				screen_mode = GAME_CLEAR;
			}
		}

	GAME_OVER:
		{
			if (NowHP == 0) {
				fade_effect = true;
			}
			if (fade_alpha_x > SCREEN_W && fade_effect == true && NowHP == 0) {
				screen_mode = GAME_OVER;
			}
		}
	GAME_PLAY:
		{
			if (fade_effect == false)
			{
				time_s -= 1;
			KEYBOARD:
				{
					if (CheckHitKey(KEY_INPUT_RIGHT)) {
						player_x += PLAYER_SPEED;
						rota = 0.0f;
					}
					if (CheckHitKey(KEY_INPUT_LEFT)) {
						player_x -= PLAYER_SPEED;
						rota = 0.0f;
					}
					if (CheckHitKey(KEY_INPUT_DOWN)) {
					}
					if (isDoubleJump == false)
					{
						if (PushHitKey(KEY_INPUT_UP) || PushHitKey(KEY_INPUT_SPACE)) {

							// Initial jump from the ground
							ChangeVolumeSoundMem(volume, jump_sound);
							PlaySoundMem(jump_sound, DX_PLAYTYPE_BACK);
							isGround = false;
							isJumping = true;
							jump_time++;
							player_position_y = player_y;
						}
					}
				}

			JUMP_RULES:
				{
					// Apply jump logic
					if (jump_time > 1) {
						isDoubleJump = true;
					}
					//Jump One
					if (isJumping == true && jump_time == 1) {
						if (player_y > GROUND - JUMP_HEIGHT) {
							player_y -= JUMP_SPEED;
						}
						else {

							delay_jump_down -= 0.1f;

							if (delay_jump_down <= 0.0f)
							{
								delay_jump_down = 1.0f;
								isJumping = false;
							}
						}
					}
					//Double Jump
					if (isJumping == true && jump_time == 2) {
						if (player_y > player_position_y - JUMP_HEIGHT) {
							player_y -= JUMP_SPEED;
							delay_jump_down = 1.0f;
						}
						else {
							delay_jump_down -= 0.1f;
							if (delay_jump_down <= 0.0f)
							{
								delay_jump_down = 1.0f;
								isJumping = false;
							}
						}
					}
					//If Not Jump
					if (isJumping == false && isGround == false) {
						if (player_y < GROUND) {
							player_y += JUMP_SPEED + 2.0f;
						}
						else {
							player_y = GROUND;
							isGround = true;
						}
					}
					if (isGround == true) {
						isDoubleJump = false;
						jump_time = 0;
					}
				}

			SHADOW:
				{
					//Shadow Follow Player
					for (int i = SHADOW_TIMES - 1; i > 0; i--) {
						record_x[i] = record_x[i - 1];
						record_y[i] = record_y[i - 1];
						rota_record[i] = rota_record[i - 1];
					}

					record_x[0] = player_x;
					record_y[0] = player_y;
					rota_record[0] = rota;

					for (int i = 0; i < 4; i++)
					{
						shadow_x[i] = record_x[5 + delay * i];
						shadow_y[i] = record_y[5 + delay * i];
						rota_option[i] = rota_record[5 + delay * i];
					}
				}

			TARGET:
				{
					if (time_s % 400 == 0) {
						target_speed += 1.3f;
						if (target_speed > 13.0f) {
							target_speed = 13.0f;
						}
					}
				TARGET_MOVING:
					{
						if (target_appear[0] == true && target_position_X != TARGET_LEFT_RIGHT)
						{
							for (int i = 0; i < 4; i++)
							{
								if (target_position_X == TARGET_RIGHT) {
									target_x[i] -= target_speed;
									target_rota[i] -= target_speed * 0.2;
								}
								else if (target_position_X == TARGET_LEFT) {
									target_x[i] += target_speed;
									target_rota[i] += target_speed * 0.2;
								}
							}
						}
						else if (target_appear[0] == true && target_position_X == TARGET_LEFT_RIGHT) {
							for (int i = 0; i < 4; i++) {
								target_x[i] += target_speed;
								target_rota[i] += target_speed * 0.15;
							}
							for (int i = 4; i < 8; i++) {
								target_x[i] -= target_speed;
								target_rota[i] -= target_speed * 0.15;
							}
						}
						for (int i = 0; i < TARGET_NUM; i++) {

							if (target_appear[i] && (target_x[i] >= SCREEN_W + 30 || target_x[i] <= -30)) {
								ChangeVolumeSoundMem(volume, appear_sound);
								PlaySoundMem(appear_sound, DX_PLAYTYPE_BACK);
								target_appear[i] = false;
							}
						}

					}

				SET_TARGET_POSITION_RANDOM:
					{

						if ((target_appear[0] == false && target_x[0] <= -30) || (target_appear[0] == false && target_x[0] >= SCREEN_W + 30)) {

							switch (target_position_X = GetRand(3)) {
							case TARGET_RIGHT:

								target_x[0] = SCREEN_W + 30.0f;
								target_x[1] = SCREEN_W + 30.0f;
								target_x[2] = SCREEN_W + 30.0f;
								target_x[3] = SCREEN_W + 30.0f;
								break;
							case TARGET_LEFT:

								target_x[0] = -30.0f;
								target_x[1] = -30.0f;
								target_x[2] = -30.0f;
								target_x[3] = -30.0f;
								break;
							case TARGET_LEFT_RIGHT:
								target_x[0] = -30.0f;
								target_x[1] = -30.0f;
								target_x[2] = -30.0f;
								target_x[3] = -30.0f;
								target_x[4] = SCREEN_W + 30.0f;
								target_x[5] = SCREEN_W + 30.0f;
								target_x[6] = SCREEN_W + 30.0f;
								target_x[7] = SCREEN_W + 30.0f;
							}
							if (target_position_X != TARGET_LEFT_RIGHT)
							{
								switch (target_position_Y = GetRand(3)) {
								case TARGET_BOTTOM_MIDDLE:
									target_y[0] = 450.0f;
									target_y[1] = 450.0f - 50;
									target_y[2] = 450.0f - 100;
									target_y[3] = 450.0f - 150;
									break;
								case TARGET_BOTTOM_UPSIDE:
									target_y[0] = 450.0f;
									target_y[1] = 450.0f - 50;
									target_y[2] = 450.0f - 200;
									target_y[3] = 450.0f - 250;
									break;
								case TARGET_MIDDLE_UPSIDE:
									target_y[0] = 450.0f - 100;
									target_y[1] = 450.0f - 150;
									target_y[2] = 450.0f - 200;
									target_y[3] = 450.0f - 250;
									break;
								}
								for (int i = 0; i < TARGET_NUM; i++) {
									target_appear[i] = true;
								}
							}
							else if (target_position_X == TARGET_LEFT_RIGHT) {
								target_y[0] = 450.0f;
								target_y[1] = 450.0f - 50;
								target_y[2] = 450.0f - 200;
								target_y[3] = 450.0f - 250;
								target_y[4] = 450.0f;
								target_y[5] = 450.0f - 50;
								target_y[6] = 450.0f - 200;
								target_y[7] = 450.0f - 250;
								for (int i = 0; i < TARGET_NUM; i++) {
									target_appear[i] = true;
								}
							}
						}

					}

				TARGET_HIT_PLAYER:
					{
						for (int i = 0; i < TARGET_NUM; i++) {
							if (CheckCircleHit(player_x, player_y, 30, target_x[i], target_y[i], 15) && isDamage == false) {
								isDamage = true;
								ChangeVolumeSoundMem(volume, hit_sound);
								PlaySoundMem(hit_sound, DX_PLAYTYPE_BACK);
								NowHP--;
							}
						}
					}

				}
			PLAYER:
				{
					if (isDamage) {
						timeInvi -= 1;
						if (timeInvi <= 0.0f) {
							timeInvi = 60;
							isDamage = false;
						}
					}
					if (player_x >= SCREEN_W - 28) {
						player_x = SCREEN_W - 28;
					}
					if (player_x <= 28) {
						player_x = 28;
					}
				}
			}
		}
		break;
	}

	case GAME_CLEAR:
	{

		GetTextLength("Play Again", 24, SCREEN_W / 2 - 85, SCREEN_H / 2 + 80);
		MouseHitText(0);

		GetTextLength("Exit Game", 24, SCREEN_W / 2 - 80, SCREEN_H / 2 + 120);
		MouseHitText(1);
		if (CheckMouseHitText(mouseX, mouseY, textX, textY, 24)) {
			exit('0');
		}
		if (fade_alpha_x == SCREEN_W) {
			ResetGame();
			screen_mode = INGAME;
		}
		break;
	}

	case GAME_OVER:
	{

		GetTextLength("Play Again", 24, SCREEN_W / 2 - 85, SCREEN_H / 2 + 80);
		MouseHitText(0);

		GetTextLength("Exit Game", 24, SCREEN_W / 2 - 80, SCREEN_H / 2 + 120);
		MouseHitText(1);
		if (CheckMouseHitText(mouseX, mouseY, textX, textY, 24)) {
			exit('0');
		}

		if (fade_alpha_x == SCREEN_W) {
			ResetGame();
			screen_mode = INGAME;
		}
		break;
	}
	}

FADE_EFFECT:
	{
		if (fade_effect == true) {
			fade_alpha_x += fade_speed;
		}
		if (fade_alpha_x > SCREEN_W * 2) {
			fade_alpha_x = 0;
			fade_effect = false;
		}
	}

}




//---------------------------------------------------------------------------------
//	描画処理
//---------------------------------------------------------------------------------
void DrawGamePlay() {
BACKGROUND_DRAW:
	{
		DrawExtendGraph(0, 0, SCREEN_W, SCREEN_H, background, TRUE);
	}

SHADOW_DRAW:
	{
		if (timeInvi % 10 >= 0 && timeInvi % 10 <= 3)
		{
			for (int i = 3; i > -1; i--)
			{
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 130 - i * 30);

				DrawRotaGraphF(shadow_x[i], shadow_y[i], 0.5f, TO_RADIAN(rota_option[i]), option_image, TRUE);
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
			}
		}
	}

PLAYER_DRAW:
	{
		if ((timeInvi % 10 >= 0 && timeInvi % 10 <= 3))
		{
			DrawRotaGraphF(player_x, player_y, 0.5f, TO_RADIAN(rota), player_image, TRUE);
		}
		for (int i = 0; i < NowHP; i++) {
			DrawRotaGraphF(player_HPx[i], player_HPy[i], 0.3f, TO_RADIAN(rota), player_image, TRUE);
		}
	}

TARGET_DRAW:
	{

		for (int i = 0; i < TARGET_NUM; i++)
		{
			DrawRotaGraphF(target_x[i], target_y[i], 0.1f, TO_RADIAN(target_rota[i]), target_image, TRUE);
		}

	}
}
void GameRender()
{
	switch (screen_mode)
	{
	case BEGIN:

	{
		DrawExtendGraph(0, 0, SCREEN_W, SCREEN_H, back_ground2, TRUE);
		SetFontSize(48);
		DrawString(SCREEN_W / 2 - 150, 100, "Survival 1m", GetColor(25, 250, 25));
		SetFontSize(24);
		DrawString(SCREEN_W / 2 - 80, SCREEN_H / 2 - 30, "Game Start", color[0]);
		DrawString(SCREEN_W / 2 - 75, SCREEN_H / 2 + 20, "Exit Game", color[1]);

		if (fade_alpha_x < SCREEN_W * 2 && fade_effect == true) {
			DrawFillBox(fade_alpha_x - SCREEN_W, fade_alpha_y, fade_alpha_x, fade_alpha_y + SCREEN_H, GetColor(0, 0, 0));
		}

		break;
	}

	case INGAME:
	{
		if (fade_alpha_x < SCREEN_W)
		{

			DrawRotaGraphF(player_x, player_y, 0.5f, TO_RADIAN(rota), player_image, TRUE);

			DrawGamePlay();

			SetFontSize(24);
			DrawFormatString(SCREEN_W - 120, 30, GetColor(0, 0, 255), "Time: %d", time_s / 60);

			DrawFillBox(fade_alpha_x - SCREEN_W, fade_alpha_y, fade_alpha_x, fade_alpha_y + SCREEN_H, GetColor(0, 0, 0));
		}
		if (fade_alpha_x > SCREEN_W) {
			DrawExtendGraph(0, 0, SCREEN_W, SCREEN_H, background, TRUE);
			SetFontSize(24);
			DrawFormatString(SCREEN_W - 120, 30, GetColor(0, 0, 255), "Time: %d", time_s / 60);

			DrawRotaGraphF(player_x, player_y, 0.5f, TO_RADIAN(rota), player_image, TRUE);
			for (int i = 0; i < NowHP; i++) {
				DrawRotaGraphF(player_HPx[i], player_HPy[i], 0.3f, TO_RADIAN(rota), player_image, TRUE);
			}

			DrawFillBox(fade_alpha_x - SCREEN_W, fade_alpha_y, fade_alpha_x, fade_alpha_y + SCREEN_H, GetColor(0, 0, 0));
		}
		break;
	}

	case GAME_CLEAR:
	{
		DrawExtendGraph(0, 0, SCREEN_W, SCREEN_H, back_ground2, TRUE);
		SetFontSize(48);
		DrawString(SCREEN_W / 2 - 130, SCREEN_H / 2 - 50, "GAME CLEAR", GetColor(25, 250, 25));

		SetFontSize(24);
		DrawFormatString(SCREEN_W / 2 - 90, SCREEN_H / 2 + 30, GetColor(0, 0, 255), "Score: %d", (TIME_MAX - time_s) * 3);

		DrawString(SCREEN_W / 2 - 85, SCREEN_H / 2 + 80, "Play Again", color[0]);
		DrawString(SCREEN_W / 2 - 80, SCREEN_H / 2 + 120, "Exit Game", color[1]);
		if (fade_alpha_x < SCREEN_W * 2 && fade_effect == true) {
			DrawFillBox(fade_alpha_x - SCREEN_W, fade_alpha_y, fade_alpha_x, fade_alpha_y + SCREEN_H, GetColor(0, 0, 0));
		}
		break;
	}

	case GAME_OVER:
	{
		DrawExtendGraph(0, 0, SCREEN_W, SCREEN_H, back_ground2, TRUE);
		SetFontSize(48);
		DrawString(SCREEN_W / 2 - 125, SCREEN_H / 2 - 50, "GAME OVER", GetColor(25, 250, 25));

		SetFontSize(24);
		DrawFormatString(SCREEN_W / 2 - 90, SCREEN_H / 2 + 30, GetColor(0, 0, 255), "Score: %d", (TIME_MAX - time_s) * 3);

		DrawString(SCREEN_W / 2 - 85, SCREEN_H / 2 + 80, "Play Again", color[0]);
		DrawString(SCREEN_W / 2 - 80, SCREEN_H / 2 + 120, "Exit Game", color[1]);
		if (fade_alpha_x < SCREEN_W * 2 && fade_effect == true) {
			DrawFillBox(fade_alpha_x - SCREEN_W, fade_alpha_y, fade_alpha_x, fade_alpha_y + SCREEN_H, GetColor(0, 0, 0));
		}
		break;
	}

	}
}
//---------------------------------------------------------------------------------
//	終了処理
//---------------------------------------------------------------------------------
void GameExit()
{
	DeleteGraph(player_image);
	DeleteGraph(option_image);
	DeleteGraph(background);
	DeleteGraph(target_image);
	DeleteGraph(back_ground2);
	DeleteSoundMem(jump_sound);
	DeleteSoundMem(hit_sound);
	DeleteSoundMem(appear_sound);
}
