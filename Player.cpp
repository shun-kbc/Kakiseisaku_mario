#include "DxLib.h"
#include "Player.h"
#include "Input.h"
#include <cstdlib>
#include <math.h>

#define PI    3.1415926535897932384626433832795f
#define WALK_MAX 16
#define RUN_MAX 9
#define CNT_LIMIT_MAX 8

int Player::mImagePlayer[MRIO_IMAGE_MAX] = {}; //static�ϐ��͌Ăяo���Ƃ��͕K������ȏ��������K�v(�O���[�o���ϐ��̗l�ɏ���)

void Player::Player_Initialize()//������
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

void Player::Player_Finalize()//�I������
{
	for (int i = 0; i < MRIO_IMAGE_MAX; i++) {
		DeleteGraph(mImagePlayer[i]);
	}
}

void Player::Player_Update()//�X�V
{
	/* �_�b�V���t���O�̐؂�ւ� */
	if (iNowKey & PAD_INPUT_A && (iNowKey & PAD_INPUT_LEFT || iNowKey & PAD_INPUT_RIGHT)) {
		dash = true;
		
	}
	else {
		dash = false;
	}

	Turn(); //�U���������

	/* �ړ����� */
	if (!slide_turn) {
		if (dash) { //���鎞
			Dash();
		}
		else { //������    
			Walk();
		}
	}

	if (idle) {
		walk = false;
	}
}

void Player::Player_Draw()//�`��
{
	if (!idle) { //�ړ����Ă��鎞
		if (dash) {	//�_�b�V���t���O��true�̎�
			if (time >= 4) {
				DrawRotaGraph(p_posX, 240, 1.0f, 0, mImagePlayer[player_num], true, turn); //�v���C���[�̕\���ʒu
				now_posX = p_posX;
			}
			else {
				DrawRotaGraph(p_posX, 240, 1.0f, 0, mImagePlayer[player_num], true, turn); //�v���C���[�̕\���ʒu
			}
		}
		else { //�_�b�V���t���O��false�̎�
			if (time >= 7) {
				DrawRotaGraph(p_posX, 240, 1.0f, 0, mImagePlayer[player_num], true, turn); //�v���C���[�̕\���ʒu
				now_posX = p_posX;
			}
			else {
				DrawRotaGraph(p_posX, 240, 1.0f, 0, mImagePlayer[player_num], true, turn); //�v���C���[�̕\���ʒu
			}
		}
	}
	else { //�~�܂鎞
		
		if (speed > 0.1) {
			DrawRotaGraph(p_posX, 240, 1.0f, 0, mImagePlayer[player_num], true, turn); //�v���C���[�̕\���ʒu
		}
		else {
			DrawRotaGraph(now_posX, 240, 1.0f, 0, mImagePlayer[player_num], true, turn); //�v���C���[�̕\���ʒu
		}
	}

	ShowDebug();
}

void Player::ChangeImage() {
	if (slide_turn && speed > 0.5) {
		//player_num = 4;
	}
	else if (!idle && !slide_turn) {
		if (!dash) { //����
			if (++cnt >= cnt_limit) {
				cnt = 0;
				if (++player_num > 3) {
					player_num = 1;
				}
				if (++array_num > WALK_MAX) array_num = WALK_MAX;
				cnt_limit = change_walk[array_num];
			}
		}
		else { //����
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
	if (iNowKey & PAD_INPUT_LEFT) { //���ɓ��͂���Ă��鎞
		if (moveX == 1.0f) { //�����ړ����������̒l��������
			SlideTurn();
		}
		else {
			turn = true;
		}
		
	}
	else if (iNowKey & PAD_INPUT_RIGHT) {
		if (moveX == -1.0f) { //�����ړ����������̒l��������
			SlideTurn();
		}
		else {
			turn = false;
		}
	}

	/* �X���C�h�^�[�����ɓ��͂������ꂽ�ꍇ�͉��ŏ�����r���ĊJ */
	if (slide_turn && iNowKey == 0) { 
		SlideTurn();
	}
}

void Player::Walk() { //��������
	if (dash == false && (iNowKey & PAD_INPUT_LEFT || iNowKey & PAD_INPUT_RIGHT)) {
		if (idle == true) {
			idle = false;
			walk = true;
			stop_time = 0;
			speed = 0.1f;
			acceleration = 0.05f;
		}

		ChangeImage();//�摜�؂�ւ�����

		time++;
		if (speed < 3) {
			if (time > 23)acceleration = 0.1;
			speed = acceleration * time + 0.1;
		}
		else {
			speed = 3;
		}

		p_posX = p_posX + (speed * moveX); //�v���C���[�̈ړ�
		DrawFormatString(0, 170, GetColor(255, 255, 255), "���ݐi��ł��܂�");
	}
	else {
		Stop();
	}
	
	
}

void Player::Dash() { //���鏈��
	if (dash == true && (iNowKey & PAD_INPUT_LEFT || iNowKey & PAD_INPUT_RIGHT)) {
		if (idle == true) {
			idle = false;
			stop_time = 0;
			acceleration = 0.1f;
		}
		ChangeImage();//�摜�؂�ւ�����

		time++;

		/* �������� */
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
				wtor_time = 0; //�����^�C��������
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

		p_posX = p_posX + (speed * moveX); //�v���C���[�̈ړ�
		DrawFormatString(0, 170, GetColor(255, 255, 255), "���ݐi��ł��܂�");
	}
	else {
		Stop();
	}
}

void Player::Stop() { //�~�܂鎞�̏���
		DrawFormatString(0, 210, GetColor(255, 255, 255), "Stop�����ɓ����Ă��܂�");
		if (idle == false) {
			idle = true; //�~�܂鎞�̃t���O��true�ɂ���
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

			p_posX = p_posX + (speed * moveX); //�v���C���[�̈ړ�
			now_posX = p_posX;
			DrawFormatString(0, 170, GetColor(255, 255, 255), "���ݐi��ł��܂�");
			DrawFormatString(0, 170, GetColor(255, 255, 255), "���ݐi��ł��܂�");
		}

		else {
			speed = 0.1f;
			player_num = 0;
		}
	
}

void Player::SlideTurn() {
	if (slide_turn == false) {
		slide_turn = true;

		DrawFormatString(0, 210, GetColor(255, 255, 255), "�X���C�h�^�[�����̃u���[�L����");
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

		p_posX = p_posX + (speed * moveX); //�v���C���[�̈ړ�
		now_posX = p_posX;
		DrawFormatString(0, 170, GetColor(255, 255, 255), "�u���[�L������");
	}
	else {
		
		slide_turn = false;
		moveX = -moveX;
		slid_time = 0;
		speed = 0.1f;
	}
}

void Player::ShowDebug() { //�f�o�b�O�\��
	DrawFormatString(0, 30, GetColor(255, 255, 255), "���͎���:%lf", time);
	DrawFormatString(0, 50, GetColor(255, 255, 255), "�v���C���[�̉摜�ԍ�:%d", player_num);
	DrawFormatString(0, 70, GetColor(255, 255, 255), "���݂̓���[��:2,�E:4]:%d", iNowKey);
	DrawFormatString(0, 90, GetColor(255, 255, 255), "�摜�̌���[0:�E,1:��]%d", turn);
	DrawFormatString(0, 110, GetColor(255, 255, 255), "�X�s�[�h%f", abs(speed));
	DrawFormatString(0, 130, GetColor(255, 255, 255), "speed�v�Z%lf = %lf + (%lf * %f)", p_posX,p_posX,speed,moveX);
	DrawFormatString(0, 150, GetColor(255, 255, 255), "N�̒l : %d", slide_turn);
	DrawFormatString(0, 220, GetColor(255, 255, 255), "stop_time : %d",stop_time);
	DrawFormatString(0, 240, GetColor(255, 255, 255), "cnt_limit : %d", cnt_limit);
	DrawFormatString(0, 260, GetColor(255, 255, 255), "idle : %d", idle);
}