#include "DxLib.h"
#include "Player.h"
#include "Input.h"
#include <cstdlib>
#include <math.h>
#include "Game.h"

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

	input = 0;

	speed = 0.1f;
	now_speed = speed;
	acceleration = 0.05f;
	j_accel = 1.0f;
	jump_power = 5.0f;
	moveX = 1.0f;
	moveY = -1.0f;
	p_posX = 320;
	now_posX = p_posX;
	p_posY = 240;
	now_posY = p_posX;

	p_w = 32;
	p_h = 32;

	//N = 7;
	dash = false;
	walk = false;
	idle = true;
	fall = false;
	input_hold = false;
	isGround = true;

	time = 0;
	wtor_time = 0;
	stop_time = 0;
	stop_max = 0;
	slid_time = 0;
	jInput_time = 0;
	fall_time = 0;

	continue_time = 0;

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

	Jump();

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

	/* 【ジャンプ時以外の落下の処理】 */
	if (!block.HitBoxPlayer() && !jump && (p_posY + (p_h * 0.5) < 256)) {
		fall = true;
		isGround = false;
		Fall();
	}
	
	//else if (p_posX - (p_w * 0.5) <  500 && p_posY + (p_h * 0.5) >= 320) {
	//	//fall = false;
	//	
	//	p_posY = 320 -(p_h * 0.5);

	//	/* 初期化 */
	//	fall = false;
	//	fall_time = 0;
	//	jump_max = 0;
	//}



	if (block.HitBoxPlayer()) {

		/* 【ブロック上に乗る時の処理】 */
		if ((p_posX + (p_w * 0.5) > block.b_x - (block.b_w * 0.5)) && (p_posX - (p_w * 0.5) < block.b_x + (block.b_w * 0.5)) && p_posY < block.b_y - (block.b_h * 0.5)) {
			/* 初期化 */
			jump = false;
			fall = false;
			jInput_time = 0;
			fall_time = 0;
			jump_power = 1.0f;

			p_posY = block.b_y - (block.b_h);

			isGround = true;
		}

		/* ブロック横(左) */
		if (/*(moveX == 1.0f || speed < 0) && */(p_posX + (p_w * 0.5) > block.b_x - (block.b_w * 0.5) && p_posX < block.b_x - (block.b_w * 0.5)) && (p_posY - (p_h * 0.5) < block.b_y + (block.b_h * 0.5) && p_posY + (p_h * 0.5) > block.b_y - (block.b_h * 0.5) + 2)) {
			p_posX = (block.b_x - (block.b_w * 0.5)) - (p_w * 0.5);
		}
		/* ブロック横(右) */
		if (/*(moveX == -1.0f || speed < 0) && */(p_posX - (p_w * 0.5) < block.b_x + (block.b_w * 0.5) && p_posX > block.b_x + (block.b_w * 0.5)) && (p_posY - (p_h * 0.5) < block.b_y + (block.b_h * 0.5) && p_posY + (p_h * 0.5) > block.b_y - (block.b_h * 0.5) + 2)) {
			p_posX = (block.b_x + (block.b_w * 0.5)) + (p_w * 0.5);
		}
	}
	
	
}

void Player::Player_Draw()//描画
{
	/*【ジャンプの高さデバッグ】*/
	//DrawBox(0,0,640, jump_max,GetColor(100,100,255),true);
	//DrawBox(block.b_x - (block.b_w * 0.5), block.b_y + (block.b_h * 0.5), block.b_x + (block.b_w * 0.5), block.b_y - (block.b_h * 0.5), GetColor(255, 255, 255), TRUE);

	if (!idle) { //移動している時
		if (dash) {	//ダッシュフラグがtrueの時
			if (time >= 4) {
				DrawRotaGraph(p_posX, p_posY, 1.0f, 0, mImagePlayer[player_num], true, turn); //プレイヤーの表示位置
				now_posX = p_posX;
			}
			else {
				DrawRotaGraph(p_posX, p_posY, 1.0f, 0, mImagePlayer[player_num], true, turn); //プレイヤーの表示位置
			}
		}
		else { //ダッシュフラグがfalseの時
			if (time >= 7) {
				DrawRotaGraph(p_posX, p_posY, 1.0f, 0, mImagePlayer[player_num], true, turn); //プレイヤーの表示位置
				now_posX = p_posX;
			}
			else {
				DrawRotaGraph(p_posX, p_posY, 1.0f, 0, mImagePlayer[player_num], true, turn); //プレイヤーの表示位置
			}
		}
	}
	else { //止まる時
		
		if (speed > 0.1) {
			DrawRotaGraph(p_posX, p_posY, 1.0f, 0, mImagePlayer[player_num], true, turn); //プレイヤーの表示位置
		}
		else {
			DrawRotaGraph(now_posX, p_posY, 1.0f, 0, mImagePlayer[player_num], true, turn); //プレイヤーの表示位置
		}
	}

	ShowDebug();
}

void Player::ChangeImage() {
	if(!jump){
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
		}
		else if (idle) {
			cnt = 0;
			cnt_limit = 0;
			array_num = 0;
		}
	}
}

void Player::Turn() {
	if (!jump) {

		DrawFormatString(0, 50, GetColor(255, 255, 255), "ジャンプ中ではありません");

		if (slide_turn) { //スライドターンがtrueの時
			if (left_turn && iNowKey & PAD_INPUT_RIGHT) { //ターンがfalseで右入力されている時
				slide_turn = false;
			}
			else if (right_turn && iNowKey & PAD_INPUT_LEFT) { //ターンがtrueで左入力されている時
				slide_turn = false;
			}
		}

		if (iNowKey & PAD_INPUT_LEFT) { //左に入力されている時
			DrawFormatString(0, 60, GetColor(255, 255, 255), "左入力中です");
			if (moveX == 1.0f) { //もし移動方向が正の値だったら
				left_turn = true;
				SlideTurn();
			}
			else {
				left_turn = false;
				turn = true;
			}

		}
		else if (iNowKey & PAD_INPUT_RIGHT) {
			DrawFormatString(0, 60, GetColor(255, 255, 255), "右入力中です");
			if (moveX == -1.0f) { //もし移動方向が負の値だったら
				right_turn = true;
				SlideTurn();
			}
			else {
				right_turn = false;
				turn = false;
			}
		}

		/* スライドターン中に入力が離された場合は下で処理を途中再開 */
		if (slide_turn && iNowKey == 0) {
			SlideTurn();
		}

		if (slide_turn == false) {
			if (moveX == -1.0f) {
				turn = true;
			}
			else {
				turn = false;
			}
		}
	}
	else {
		
		//if (iNowKey & PAD_INPUT_LEFT || (iNowKey & PAD_INPUT_LEFT && iNowKey & PAD_INPUT_A)) {
		//	if (now_speed >= 0.3 && fall == true) {
		//		//moveX = -1.0f;
		//	}
		//	else if(now_speed <= 0.2){
		//		moveX = -1.0f;
		//	}
		//}
		//if (iNowKey & PAD_INPUT_RIGHT || (iNowKey & PAD_INPUT_RIGHT && iNowKey & PAD_INPUT_A)) {
		//	if (now_speed >= 0.3 && fall == true) {
		//		//moveX = 1.0f;
		//	}
		//	else if (now_speed <= 0.2) {
		//		moveX = 1.0f;
		//	}
		//}
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

		/*if (!jump) {*/
			time++;
			if (speed < 3) {
				if (time > 23)acceleration = 0.1;
				speed = acceleration * time + 0.1;
			}
			
		/*}*/
			if (moveX == 1.0f && iNowKey & PAD_INPUT_LEFT) {
				--speed;
				if (!dash) {
					if (abs(speed) > 1.5f)speed = -1.5f;
				}
				else {
					if (abs(speed) > 2.5)speed = -2.5;
				}

			}
			else if (moveX == -1.0f && iNowKey & PAD_INPUT_RIGHT) {
				--speed;
				if (!dash) {
					if (abs(speed) > 1.5f)speed = -3.0f;
				}
				else {
					if (abs(speed) > 2.5)speed = -5.0f;
				}
			}

		else if(speed >= 3.0f) {
			speed = 3.0f;
		}

		p_posX = p_posX + (speed * moveX); //プレイヤーの移動
	}
	else if (jump) {
		/*if (moveX == 1.0f && iNowKey & PAD_INPUT_LEFT)--speed;
		if (abs(speed) > 1)speed = -1;*/
		if (now_speed > 0.1f) {
			p_posX = p_posX + (speed * moveX); //プレイヤーの移動
		}
	}
	else {
		if (!jump) {
			Stop();
		}
		/*if (++continue_time >= 10) {
			continue_time = 0;
			_continue = true;
		}*/
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


		/*if (!jump) {*/
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
		}
		if(speed >= 5){
			speed = 5;
		}

		p_posX = p_posX + (speed * moveX); //プレイヤーの移動
	//}
	//else if (jump) {
	//	JumpMove();
	//	p_posX = p_posX + (speed * moveX); //プレイヤーの移動
	//}
	/*else {
		if (!jump)Stop();
	}*/
}

void Player::Stop() { //止まる時の処理
		if (idle == false) {
			idle = true; //止まる時のフラグをtrueにする
			now_speed = speed;
			stop_max = speed * 5.3f;
			acceleration = 0.05f;

			cnt = 0;
			array_num = 0;
		}

		if (idle) {
			if (stop_time < stop_max && speed > 0.1f) {
				++stop_time;
				time = 0;

				//if (stop_time > 20)acceleration = 0.05f;
				speed = speed - (acceleration)*stop_time + 0.1;

				p_posX = p_posX + (speed * moveX); //プレイヤーの移動
				now_posX = p_posX;
			}

			else {
				speed = 0.1f;
				player_num = 0;
			}
		}
}

void Player::SlideTurn() {
	DrawFormatString(0, 30, GetColor(255, 255, 255), "スライドターン処理中" );
	if (!jump && slide_turn == false) {
		//double input = iNowKey;
		input = iNowKey;
		slide_turn = true;

		now_speed = speed;
		stop_max = speed * 5.3f;
	}
	
	if (slide_turn) {

		if ((iNowKey & PAD_INPUT_LEFT || iNowKey & PAD_INPUT_RIGHT) && input != iNowKey) {
			slide_turn = false;
			moveX = -moveX;
			slid_time = 0;
			
		}

		if (speed < now_speed && speed > 0.1) {
			player_num = 4;
		}
		if (speed > 0.1f) {

			time = 0;
			++slid_time;

			speed = speed - (acceleration) * slid_time + 0.1;

			p_posX = p_posX + (speed * moveX); //プレイヤーの移動
			now_posX = p_posX;
		}
	}

	

	if (speed <= 0.1f) {
		
		slide_turn = false;
		moveX = -moveX;
		slid_time = 0;
		speed = 0.1f;
	}
}

void Player::Jump() {
	if (!fall) {
		moveY = -1.0f;
	}

	/* ジャンプフラグの切り替え */
	if (isGround == true && iNowKey & PAD_INPUT_B) {
		if (jump == false) {
			jump = true;
			now_posY = p_posY;
			input_hold = true;
			j_accel = 1.0f;
			jump_power = 5.0f;

			idle = false;
			now_speed = speed;
			//now_time = time;
			/*speed = 1;
			if (now_speed < 3 && now_speed >= 0.2) {
				speed = 1;
			}
			*/
			isGround = false;
		}
	}

	/* 上がる処理 */
	if (jump) {
		/*if (iNowKey != 0 && iNowKey != 32) {
			speed = 1;
		}
		if (now_speed <= 0.1) {
			speed = 0;
		}*/

		if (++now_time > 30) now_time = 30;
		player_num = 5;
		if (input_hold == true && iNowKey & PAD_INPUT_B) {
			if (++jInput_time > 24) {
				jInput_time = 24;
				input_hold = false;
			}
		}
		else {
			input_hold = false;
		}

		if (!fall) {
			
			if (jInput_time < 8) { //一番小さいジャンプ
				jump_max = now_posY - (4 * 8);

				//if(jInput_time)
				//jump_power -= 0.1f;
				//if (p_posY < p_posY - ((p_posY - jump_max) / 2)) {
				//	//jump_power = (j_accel * jInput_time) + 0.1;
				//	jump_power -= 0.01f;
				//}
				

				p_posY = p_posY + (jump_power * moveY);

				if (p_posY < jump_max) {
					p_posY = jump_max;
					if (input_hold == false) {
						fall = true;
					}
				}
			}
			else {
				jump_max = now_posY - ((128.0/24.0) * (jInput_time));

				if (jump_power > 1) {
					if (p_posY > now_posY - 64) {
						jump_power -= 0.15f;
					}
					else {
						jump_power -= 0.05f;
					}
				}
				else {
					jump_power = 1;

				}
				

				/*if (jump_power > 1.0) {
					jump_power -= 0.15f;
				}
				else if (jump_power > 0.1) {
					jump_power -= 0.05f;
				}
				else {
					jump_power = 0.1;
				}*/
				
				
				if (p_posY > jump_max) {
					p_posY = p_posY + (jump_power * moveY);
				}
				else if (p_posY < jump_max) {
					p_posY = jump_max;
				}
				else if (p_posY == jump_max && input_hold == false) {
					if (++fall_time >= 3) {
						fall = true;
						fall_time = 0;
					}

				}
			}
		}

		/* 落下処理 */

		Fall();
	}
}

void Player::JumpMove() {
	/*if ((moveX == 1.0f && iNowKey & PAD_INPUT_LEFT) && (moveX == -1.0f && iNowKey & PAD_INPUT_RIGHT)) {
		if (speed > 0.1f) {

			speed = speed / 2;
		}
	}*/
}

void Player::Fall() {

	if (fall) {
		player_num = 5;
		moveY = 1.0f;
		DrawFormatString(0, 300, GetColor(255, 255, 255), "落下中");
		if (++fall_time < 24) {
			if (fall_time < 8) {
				//jump_power = (j_accel * fall_time) + 0.1;
				jump_power += 0.6f;
			}
			
			//else {
			//	if (p_posY + (p_h * 0.5) < 256/* ←後でここは地面との当たり判定になる */) {
			//		if (fall_time < 8) {
			//			//jump_power = (j_accel * fall_time) + 0.1;
			//			jump_power += 0.6f;
			//		}
			//		/*if (fall_time > 5)*/p_posY = p_posY - (-jump_power * moveY);

			//	}
			//}
			
		}//fall_time = 24;

		if (!isGround/*(p_posX + (p_w * 0.5) > block.b_x - (block.b_w * 0.5)) && (p_posX - (p_w * 0.5) < block.b_x + (block.b_w * 0.5)) && p_posY < block.b_y - (block.b_h * 0.5)*/) {
			p_posY = p_posY - (-jump_power * moveY);
		}

		if(/*jump &&*/ p_posY + (p_h * 0.5) > 256){

			isGround = true;
			p_posY = 256 - (p_h * 0.5);

			/* 初期化 */
			jump = false;
			fall = false;
			jInput_time = 0;
			fall_time = 0;
			jump_power = 1.0f;
			jump_max = 0;
			
		}

	}
	
}

void Player::ShowDebug() { //デバッグ表示
	/*DrawFormatString(0, 30, GetColor(255, 255, 255), "入力時間:%lf", time);
	DrawFormatString(0, 50, GetColor(255, 255, 255), "プレイヤーの画像番号:%d", player_num);
	
	DrawFormatString(0, 90, GetColor(255, 255, 255), "画像の向き[0:右,1:左]%d", turn);
	DrawFormatString(0, 110, GetColor(255, 255, 255), "スピード%f", abs(speed));
	DrawFormatString(0, 130, GetColor(255, 255, 255), "speed計算%lf = %lf + (%lf * %f)", p_posX,p_posX,speed,moveX);
	DrawFormatString(0, 150, GetColor(255, 255, 255), "Nの値 : %d", slide_turn);
	DrawFormatString(0, 220, GetColor(255, 255, 255), "stop_time : %d",stop_time);
	DrawFormatString(0, 240, GetColor(255, 255, 255), "cnt_limit : %d", cnt_limit);
	DrawFormatString(0, 260, GetColor(255, 255, 255), "idle : %d", idle);
	DrawFormatString(0, 280, GetColor(255, 255, 255), "jump_max : %lf", jump_max);
	DrawFormatString(0, 300, GetColor(255, 255, 255), "jInput_time : %lf", jInput_time);
	DrawFormatString(0, 320, GetColor(255, 255, 255), "プレイヤーのY座標 : %lf", p_posY);
	DrawFormatString(0, 360, GetColor(255, 255, 255), "ジャンプパワー: %lf", jump_power);*/
	DrawFormatString(0, 360, GetColor(255, 255, 255), "slide_turn: %d", slide_turn);
	DrawFormatString(0, 110, GetColor(255, 255, 255), "スピード%f", abs(speed));
	DrawFormatString(0, 70, GetColor(255, 255, 255), "現在の入力[左:2,右:4]:%d", iNowKey);
	DrawFormatString(0, 30, GetColor(255, 255, 255), "入力時間:%lf", time);
	DrawFormatString(0, 130, GetColor(255, 255, 255), "jumpmax:%lf", jump_max);
	DrawFormatString(0, 150, GetColor(255, 255, 255), "落下の計算 :  %lf - (- %lf *  %lf)",p_posY,-jump_power,moveY);
	//p_posY = p_posY - (-jump_power * moveY);



	/*【当たり判定デバッグ】*/
	//DrawCircle(p_posX, p_posY,5,1,1); //Playerの中心座標

	//DrawCircle(p_posX - p_w/2, p_posY, 5, GetColor(255, 255, 255), 1); //Playerの左端
	//DrawCircle(p_posX + p_w / 2, p_posY, 5, GetColor(255, 255, 255), 1); //Playerの右端
	//DrawCircle(p_posX, p_posY - p_h / 2, 5, GetColor(255, 255, 255), 1); //Playerの上底
	//DrawCircle(p_posX, p_posY + p_h / 2, 5, GetColor(255, 255, 255), 1); //Playerの底辺
}

double Player::CircOut(double t, double totaltime, double min, double max)
{
	max -= min;
	t = t / totaltime - 1;
	return max * sqrt(1 - t * t) + min;
}

bool Player::Continue() {
	bool cont;
	if (iNowKey != iOldKey) {
		if (++continue_time >= 10) {
			cont = true;
			return cont;
		}
		else {
			continue_time = 0;
			cont = false;
			return cont;
		}
	}
}