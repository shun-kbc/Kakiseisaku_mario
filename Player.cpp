#include "DxLib.h"
#include "Player.h"
#include "Input.h"
#include <cstdlib>
#include <math.h>

#define PI    3.1415926535897932384626433832795f
#define WALK_MAX 16
#define RUN_MAX 9
#define CNT_LIMIT_MAX 8

int Player::mImagePlayer[MRIO_IMAGE_MAX] = {}; //static変数は呼び出すときは必ず特殊な初期化が必要(グローバル変数の様に書く)

void Player::Player_Initialize()//初期化
{
	LoadDivGraph("images/char/mario.png", MRIO_IMAGE_MAX, MRIO_IMAGE_MAX, 1, 32, 32, mImagePlayer);
	player_num = 0;
	cnt = 0;
	cnt_limit = CNT_LIMIT_MAX;
	array_num = 0;

	speed = 0.1f;
	now_speed = speed;
	acceleration = 0.05f;
	moveX = 1.0f;
	p_posX = 320;
	now_posX = p_posX;
	//N = 7;
	dash = false;
	walk = false;
	idle = true;

	time = 0;
	wtor_time = 0;
	stop_time = 0;
	stop_max = 0;
	slid_time = 0;

	slide_turn = false;
}

void Player::Player_Finalize()//終了処理
{
	for (int i = 0; i < MRIO_IMAGE_MAX; i++) {
		DeleteGraph(mImagePlayer[i]);
	}
}

void Player::Player_Update()//更新
{
	/* ダッシュフラグの切り替え */
	if (iNowKey & PAD_INPUT_A && (iNowKey & PAD_INPUT_LEFT || iNowKey & PAD_INPUT_RIGHT)) {
		dash = true;
		
	}
	else {
		dash = false;
	}

	Turn(); //振り向き処理

	/* 移動処理 */
	if (!slide_turn) {
		if (dash) { //走る時
			Dash();
		}
		else { //歩く時    
			Walk();
		}
	}

	if (idle) {
		walk = false;
	}
}

void Player::Player_Draw()//描画
{
	if (!idle) { //移動している時
		if (dash) {	//ダッシュフラグがtrueの時
			if (time >= 4) {
				DrawRotaGraph(p_posX, 240, 1.0f, 0, mImagePlayer[player_num], true, turn); //プレイヤーの表示位置
				now_posX = p_posX;
			}
			else {
				DrawRotaGraph(p_posX, 240, 1.0f, 0, mImagePlayer[player_num], true, turn); //プレイヤーの表示位置
			}
		}
		else { //ダッシュフラグがfalseの時
			if (time >= 7) {
				DrawRotaGraph(p_posX, 240, 1.0f, 0, mImagePlayer[player_num], true, turn); //プレイヤーの表示位置
				now_posX = p_posX;
			}
			else {
				DrawRotaGraph(p_posX, 240, 1.0f, 0, mImagePlayer[player_num], true, turn); //プレイヤーの表示位置
			}
		}
	}
	else { //止まる時
		
		if (speed > 0.1) {
			DrawRotaGraph(p_posX, 240, 1.0f, 0, mImagePlayer[player_num], true, turn); //プレイヤーの表示位置
		}
		else {
			DrawRotaGraph(now_posX, 240, 1.0f, 0, mImagePlayer[player_num], true, turn); //プレイヤーの表示位置
		}
	}

	ShowDebug();
}

void Player::ChangeImage() {
	if (slide_turn && speed > 0.5) {
		//player_num = 4;
	}
	else if (!idle && !slide_turn) {
		if (!dash) { //歩き
			if (++cnt >= cnt_limit) {
				cnt = 0;
				if (++player_num > 3) {
					player_num = 1;
				}
				if (++array_num > WALK_MAX) array_num = WALK_MAX;
				cnt_limit = change_walk[array_num];
			}
		}
		else { //走り
			if (++cnt >= cnt_limit) {
				cnt = 0;
				if (++player_num > 3) {
					player_num = 1;
				}
				if (++array_num > RUN_MAX) array_num = RUN_MAX;
				cnt_limit = change_run[array_num];
			}
		}
	}else if(idle){
		cnt = 0;
		cnt_limit = 0;
		array_num = 0;
	}
	
}

void Player::Turn() {
	if (iNowKey & PAD_INPUT_LEFT) { //左に入力されている時
		if (moveX == 1.0f) { //もし移動方向が正の値だったら
			SlideTurn();
		}
		else {
			turn = true;
		}
		
	}
	else if (iNowKey & PAD_INPUT_RIGHT) {
		if (moveX == -1.0f) { //もし移動方向が負の値だったら
			SlideTurn();
		}
		else {
			turn = false;
		}
	}

	/* スライドターン中に入力が離された場合は下で処理を途中再開 */
	if (slide_turn && iNowKey == 0) { 
		SlideTurn();
	}
}

void Player::Walk() { //歩く処理
	if (dash == false && (iNowKey & PAD_INPUT_LEFT || iNowKey & PAD_INPUT_RIGHT)) {
		if (idle == true) {
			idle = false;
			walk = true;
			stop_time = 0;
			speed = 0.1f;
			acceleration = 0.05f;
		}

		ChangeImage();//画像切り替え処理

		time++;
		if (speed < 3) {
			if (time > 23)acceleration = 0.1;
			speed = acceleration * time + 0.1;
		}
		else {
			speed = 3;
		}

		p_posX = p_posX + (speed * moveX); //プレイヤーの移動
		DrawFormatString(0, 170, GetColor(255, 255, 255), "現在進んでいます");
	}
	else {
		Stop();
	}
	
	
}

void Player::Dash() { //走る処理
	if (dash == true && (iNowKey & PAD_INPUT_LEFT || iNowKey & PAD_INPUT_RIGHT)) {
		if (idle == true) {
			idle = false;
			stop_time = 0;
			acceleration = 0.1f;
		}
		ChangeImage();//画像切り替え処理

		time++;

		/* 加速処理 */
		if (walk == true) {
			if (wtor_time == 0) {
				wtor_time = time + 16;
			}
			if (time < wtor_time && speed < 5) {
				wtor_time++;
				if (speed >= 3)acceleration = 0.15;
				speed = acceleration * wtor_time + 0.1;
			}
			else {
				speed = 5;
				walk = false;
				wtor_time = 0; //ランタイム初期化
			}
			//speed = 5;
		}
		else {
			if (time < 33 && speed < 5) {
				if (speed >= 3)acceleration = 0.15;
				speed = acceleration * time + 0.1;
			}
			else {
				speed = 5;
			}
		}

		p_posX = p_posX + (speed * moveX); //プレイヤーの移動
		DrawFormatString(0, 170, GetColor(255, 255, 255), "現在進んでいます");
	}
	else {
		Stop();
	}
}

void Player::Stop() { //止まる時の処理
		DrawFormatString(0, 210, GetColor(255, 255, 255), "Stop処理に入っています");
		if (idle == false) {
			idle = true; //止まる時のフラグをtrueにする
			now_speed = speed;
			stop_max = speed * 5.3f;
			acceleration = 0.05f;

			cnt = 0;
			array_num = 0;
		}


		if (stop_time < stop_max && speed > 0.1f) {
			++stop_time;
			time = 0;

			//if (stop_time > 20)acceleration = 0.05f;
			speed = speed - (acceleration) * stop_time + 0.1;

			p_posX = p_posX + (speed * moveX); //プレイヤーの移動
			now_posX = p_posX;
			DrawFormatString(0, 170, GetColor(255, 255, 255), "現在進んでいます");
			DrawFormatString(0, 170, GetColor(255, 255, 255), "現在進んでいます");
		}

		else {
			speed = 0.1f;
			player_num = 0;
		}
	
}

void Player::SlideTurn() {
	if (slide_turn == false) {
		slide_turn = true;

		DrawFormatString(0, 210, GetColor(255, 255, 255), "スライドターン時のブレーキ処理");
		now_speed = speed;
		stop_max = speed * 5.3f;
	}
	
	if (speed < now_speed && speed > 0.2) {
		player_num = 4;
	}
	if (speed > 0.1f) {

		time = 0;
		++slid_time;
		
		speed = speed - (acceleration) * slid_time + 0.1;

		p_posX = p_posX + (speed * moveX); //プレイヤーの移動
		now_posX = p_posX;
		DrawFormatString(0, 170, GetColor(255, 255, 255), "ブレーキ処理中");
	}
	else {
		
		slide_turn = false;
		moveX = -moveX;
		slid_time = 0;
		speed = 0.1f;
	}
}

void Player::ShowDebug() { //デバッグ表示
	DrawFormatString(0, 30, GetColor(255, 255, 255), "入力時間:%lf", time);
	DrawFormatString(0, 50, GetColor(255, 255, 255), "プレイヤーの画像番号:%d", player_num);
	DrawFormatString(0, 70, GetColor(255, 255, 255), "現在の入力[左:2,右:4]:%d", iNowKey);
	DrawFormatString(0, 90, GetColor(255, 255, 255), "画像の向き[0:右,1:左]%d", turn);
	DrawFormatString(0, 110, GetColor(255, 255, 255), "スピード%f", abs(speed));
	DrawFormatString(0, 130, GetColor(255, 255, 255), "speed計算%lf = %lf + (%lf * %f)", p_posX,p_posX,speed,moveX);
	DrawFormatString(0, 150, GetColor(255, 255, 255), "Nの値 : %d", slide_turn);
	DrawFormatString(0, 220, GetColor(255, 255, 255), "stop_time : %d",stop_time);
	DrawFormatString(0, 240, GetColor(255, 255, 255), "cnt_limit : %d", cnt_limit);
	DrawFormatString(0, 260, GetColor(255, 255, 255), "idle : %d", idle);
}