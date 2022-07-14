#pragma once
#define MRIO_IMAGE_MAX 9
#include "Input.h"

class Player : Input{
	static int mImagePlayer[MRIO_IMAGE_MAX];    //画像ハンドル格納用変数
	int change_walk[17] = {8,7,7,6,2,3,4,5,3,5,4,3,5,4,3,5,4}; //歩きの画像変更フレーム
	int change_run[10] = {8,7,4,3,4,4,4,3,4,2}; //走りの画像変更フレーム 
	int array_num;
	int player_num;
	int cnt;
	int cnt_limit;

	int input;

	bool turn = false; // true:左に向く false:右に向く
	bool old_turn = false;
	bool slide_turn = false;
	bool right_turn;
	bool left_turn;
	bool dont_move;
	
	bool input_hold;

	double time;
	double wtor_time;
	double stop_time;
	double stop_max;
	double slid_time;
	double jInput_time;
	double fall_time;
	double now_time;

	double continue_time;

	double acceleration;
	double j_accel;
	double speed;
	double now_speed;
	double jump_power;
	float moveX;
	float moveY;
	double startX;
	double startY;
	double now_posX;
	double now_posY;
	double jump_pos;
	double jump_max; //最高到達点
	double minus;

	//int N;
	bool dash;
	bool walk;
	bool idle;
	bool jump;
	bool _continue;
	bool isGround;

	void Turn(); //振り向き処理
	void Walk(); //歩き
	void Dash(); //走り
	void Stop();
	void SlideTurn();
	void Jump(); //ジャンプ
	void JumpMove();
	void Fall();
	void KillY();
	void ChangeImage(); //画像切り替え処理
	void ShowDebug(); //デバッグ表示処理
	double CircOut(double t, double totaltime, double min, double max);
	bool Continue();

public:
	double p_posX; //プレイヤーのポジション
	double p_posY; //プレイヤーのポジション
	double p_w;
	double p_h;

	bool fall = false;

	void Player_Initialize();//初期化
	void Player_Finalize();//終了処理
	void Player_Update();//更新
	void Player_Draw();//描画
};

