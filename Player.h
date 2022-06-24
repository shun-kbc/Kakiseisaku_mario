#pragma once
#define MRIO_IMAGE_MAX 9
#include "Input.h"

class Player : Input{
	static int mImagePlayer[MRIO_IMAGE_MAX];    //�摜�n���h���i�[�p�ϐ�
	int change_walk[17] = {8,7,7,6,2,3,4,5,3,5,4,3,5,4,3,5,4}; //�����̉摜�ύX�t���[��
	int change_run[10] = {8,7,4,3,4,4,4,3,4,2}; //�����̉摜�ύX�t���[�� 
	int array_num;
	int player_num;
	int cnt;
	int cnt_limit;
	bool turn = false; // true:���Ɍ��� false:�E�Ɍ���
	bool old_turn = false;
	bool slide_turn = false;

	double time;
	double wtor_time;
	double stop_time;
	double stop_max;
	double slid_time;

	double speed;
	double now_speed;
	float moveX;
	double p_posX; //�v���C���[�̃|�W�V����
	double now_posX;
	//int N;
	bool dash;
	bool walk;
	bool idle;

	void Turn(); //�U���������
	void Walk(); //����
	void Dash(); //����
	void Stop();
	void SlideTurn();
	void ChangeImage(); //�摜�؂�ւ�����
	void ShowDebug(); //�f�o�b�O�\������

	double InCubic(double t, double totaltime, double max = 1.0, double min = 0.0);
public:
	void Player_Initialize();//������
	void Player_Finalize();//�I������
	void Player_Update();//�X�V
	void Player_Draw();//�`��
};

