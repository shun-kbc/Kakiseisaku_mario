#pragma once
#define MRIO_IMAGE_MAX 9
#include "Input.h"

class Player : Input{
	static int mImagePlayer[MRIO_IMAGE_MAX];    //�摜�n���h���i�[�p�ϐ�
	int change_walk[17] = {8,7,7,6,2,3,4,5,3,5,4,3,5,4,3,5,4}; //�����̉摜�ύX�t���[��
	int change_run[10] = {8,7,4,3,4,4,4,3,4,2}; //����̉摜�ύX�t���[�� 
	int array_num;
	int player_num;
	int cnt;
	int cnt_limit;

	int input;

	bool turn = false; // true:���Ɍ��� false:�E�Ɍ���
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
	double jump_max; //�ō����B�_
	double minus;

	//int N;
	bool dash;
	bool walk;
	bool idle;
	bool jump;
	bool _continue;
	bool isGround;

	void Turn(); //�U���������
	void Walk(); //����
	void Dash(); //����
	void Stop();
	void SlideTurn();
	void Jump(); //�W�����v
	void JumpMove();
	void Fall();
	void KillY();
	void ChangeImage(); //�摜�؂�ւ�����
	void ShowDebug(); //�f�o�b�O�\������
	double CircOut(double t, double totaltime, double min, double max);
	bool Continue();

public:
	double p_posX; //�v���C���[�̃|�W�V����
	double p_posY; //�v���C���[�̃|�W�V����
	double p_w;
	double p_h;

	bool fall = false;

	void Player_Initialize();//������
	void Player_Finalize();//�I������
	void Player_Update();//�X�V
	void Player_Draw();//�`��
};

