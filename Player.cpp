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

#define MINUS_FRAME 20.0f //�W�����v���̋t�������͎��̍��ɓ����n�߂�܂ł̃t���[����

int Player::mImagePlayer[MRIO_IMAGE_MAX] = {}; //static�ϐ��͌Ăяo���Ƃ��͕K������ȏ��������K�v(�O���[�o���ϐ��̗l�ɏ���)


void Player::Player_Initialize()//������
{
	LoadDivGraph("images/char/mario.png", MRIO_IMAGE_MAX, MRIO_IMAGE_MAX, 1, 32, 32, mImagePlayer);
	player_num = 0;
	cnt = 0;
	cnt_limit = CNT_LIMIT_MAX;
	array_num = 0;

	input = 0;

	speed = 0.0f;
	now_speed = speed;
	old_speed = now_speed;
	acceleration = 0.05f;
	j_accel = 1.0f;
	jump_power = 1.0f;
	moveX = 1.0f;
	j_direction = 0.0f;
	moveY = -1.0f;

	startX = 100;
	startY = block.gro_posY - 16/*(p_h * 0.5)*/;

	p_posX = startX;
	now_posX = p_posX;
	
	p_posY = startY;
	now_posY = p_posX;

	jump_pos = 0;

	minus = 0; //�X�s�[�h�������l
	
	//�v���C���[�̕��̒l��������
	p_w = 32;
	p_h = 32;

	dont_move = false; //�u���b�N�ɓ������Ă��鎞
	dash = false;
	walk = false;
	idle = true;
	fall = false;
	input_hold = false;
	isGround = true;
	reverse_input = false;
	turn_cancel = false;
	after_slide = false;
	zero_start = false;

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

	Jump();

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

	/* �y�W�����v���ȊO�̗����̏����z */
	if (!block.HitBoxPlayer() && !jump && (p_posY + (p_h * 0.5) < block.gro_posY)) {
		fall = true;
		isGround = false;
		Fall();
	}
	else if (!jump && (p_posX - (p_w * 0.5) > 498)/* �������Ƃ����n�ʂ̔���ɕς���ƃu���b�N�̉��ɗ������ɓ��������Ƃ��̂Ȃ��͎��� */) {
		fall = true;
		isGround = false;
		Fall();
	}

	/* �y�u���b�N�Ƃ̓����蔻��z */
	if (block.HitBoxPlayer()) { 

		/* �y�u���b�N��ɏ�鎞�̏����z */
		if ((p_posX + (p_w * 0.5) > block.b_x - (block.b_w * 0.5)) && (p_posX - (p_w * 0.5) < block.b_x + (block.b_w * 0.5)) && p_posY < block.b_y - (block.b_h * 0.5)) {
			/* ������ */
			jump = false;
			fall = false;
			jInput_time = 0;
			fall_time = 0;
			jump_power = 1.0f;

			p_posY = block.b_y - (block.b_h);

			isGround = true;

			reverse_input = false;
			zero_start = false;
		}

		/* �u���b�N��(��) */
		if ((p_posX + (p_w * 0.5) > block.b_x - (block.b_w * 0.5) && p_posX < block.b_x - (block.b_w * 0.5)) && (p_posY - (p_h * 0.5) < block.b_y + (block.b_h * 0.5) && p_posY + (p_h * 0.5) > block.b_y - (block.b_h * 0.5) + 2)) {
			dont_move = true;
			if ((speed < 1.5f) && p_posX + (p_w * 0.5) > block.b_x - (p_w * 0.5)) {
				p_posX -= 2.0f;
				now_posX = p_posX;
			}
			else {
				p_posX = (block.b_x - (block.b_w * 0.5)) - (p_w * 0.5);
				now_posX = p_posX;

				/* �e�X�g */
				//speed = 0.0f;
			}
			
		}
		/* �u���b�N��(�E) */
		if ((p_posX - (p_w * 0.5) < block.b_x + (block.b_w * 0.5) && p_posX > block.b_x + (block.b_w * 0.5)) && (p_posY - (p_h * 0.5) < block.b_y + (block.b_h * 0.5) && p_posY + (p_h * 0.5) > block.b_y - (block.b_h * 0.5) + 2)) {
			dont_move = true;
			if ((speed < 1.5f) && p_posX - (p_w * 0.5) < block.b_x + (p_w * 0.5)) {
				p_posX += 2.0f;
				now_posX = p_posX;
			}
			else {
				p_posX = (block.b_x + (block.b_w * 0.5)) + (p_w * 0.5);
				now_posX = p_posX;

				/* �e�X�g */
				//speed = 0.0f;
			}
		}
	}
	else {
		dont_move = false;
	}
	
	KillY(); //�����Ŏ��ʏ���
}

void Player::Player_Draw()//�`��
{
	if (!jump) {
		if (!idle) { //�ړ����Ă��鎞
			if (dash) {	//�_�b�V���t���O��true�̎�
				if (time >= 4.0f) {
					DrawRotaGraph(p_posX, p_posY, 1.0f, 0, mImagePlayer[player_num], true, turn); //�v���C���[�̕\���ʒu
					now_posX = p_posX;
				}
				else {
					DrawRotaGraph(now_posX, p_posY, 1.0f, 0, mImagePlayer[player_num], true, turn); //�v���C���[�̕\���ʒu
				}
			}
			else { //�_�b�V���t���O��false�̎�
				if (time >= 7.0f) {
					DrawRotaGraph(p_posX, p_posY, 1.0f, 0, mImagePlayer[player_num], true, turn); //�v���C���[�̕\���ʒu
					now_posX = p_posX;
				}
				else {
					DrawRotaGraph(now_posX, p_posY, 1.0f, 0, mImagePlayer[player_num], true, turn); //�v���C���[�̕\���ʒu
				}
			}
		}
		else { //�~�܂鎞

			if (speed > 0.0f) {
				DrawRotaGraph(p_posX, p_posY, 1.0f, 0, mImagePlayer[player_num], true, turn); //�v���C���[�̕\���ʒu
			}
			else {
				DrawRotaGraph(p_posX, p_posY, 1.0f, 0, mImagePlayer[player_num], true, turn); //�v���C���[�̕\���ʒu
			}
		}
	}
	else {
		DrawRotaGraph(p_posX, p_posY, 1.0f, 0, mImagePlayer[player_num], true, turn); //�v���C���[�̕\���ʒu
		now_posX = p_posX;
	}

	//ShowDebug();
}

void Player::ChangeImage() {
	if(!jump){
		
		if (!idle && !slide_turn) {
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
		}
		else if (idle) {
			cnt = 0;
			cnt_limit = 0;
			array_num = 0;
		}
		else if (slide_turn) {
			cnt = 0;
			cnt_limit = 0;
			array_num = 0;
		}
	}
}

void Player::Turn() {
	/* �y�W�����v�����Ă��Ȃ����z */
	if (!jump) {

		/* �y�X���C�h�^�[�����ɋt�����ɓ��͂������̏����z */
		if (slide_turn) { //�X���C�h�^�[����true�̎�
			if (left_turn && iKeyFlg & PAD_INPUT_RIGHT) { //�^�[����false�ŉE���͂���Ă��鎞
				turn_cancel = true;
				slide_turn = false;
			}
			else if (right_turn && iKeyFlg & PAD_INPUT_LEFT) { //�^�[����true�ō����͂���Ă��鎞
				turn_cancel = true;
				slide_turn = false;
			}
		}

		/* �y�X���C�h�^�[���ɓ��鎞�̏����z */
		if (iNowKey & PAD_INPUT_LEFT) { //���ɓ��͂���Ă��鎞
			DrawFormatString(0, 60, GetColor(255, 255, 255), "�����͒��ł�");
			if (moveX == 1.0f) { //�����ړ����������̒l��������
				left_turn = true;
				SlideTurn();
			}
			else {
				left_turn = false;
				turn = true;
			}

		}
		else if (iNowKey & PAD_INPUT_RIGHT) {
			DrawFormatString(0, 60, GetColor(255, 255, 255), "�E���͒��ł�");
			if (moveX == -1.0f) { //�����ړ����������̒l��������
				right_turn = true;
				SlideTurn();
			}
			else {
				right_turn = false;
				turn = false;
			}
		}

		/* �y�X���C�h�^�[�����ɓ��͂������ꂽ�ꍇ�͉��ŏ�����r���ĊJ�z */
		if (slide_turn && (iNowKey == 0 || iNowKey== 16 || after_slide)) {
			SlideTurn();
		}
		/* �y�����̏���������slide_turn�݂̂ɂ���ƁA�L�[���͂����Ă����Ԃ���2�d��SlideTurn�̏����ɓ����Ă��܂��z */
		
		if (slide_turn == false && iNowKey & PAD_INPUT_LEFT || iNowKey & PAD_INPUT_RIGHT) { 
			if (moveX == -1.0f) {
				turn = true;
			}
			else {
				turn = false;
			}
		}
	}
}

void Player::Walk() { //��������
	if (!jump && dash == false && (iNowKey & PAD_INPUT_LEFT || iNowKey & PAD_INPUT_RIGHT)) {
		/* �y�������z */
		if (idle == true) {
			idle = false;
			walk = true;
			stop_time = 0;
			speed = 0.0f;
			acceleration = 0.05f;
		}

		ChangeImage();//�摜�؂�ւ�����

		/* �y�X�s�[�h�̌v�Z�z */
		time++;
		if (speed < 3.0f) {
			if (time > 23.0f)acceleration = 0.1f;
			speed = acceleration * time + 0.1;
		}
		else {
			speed = 3.0f;
		}

		/* �y�|�W�V�����̑���z */
		p_posX = p_posX + (speed * moveX); //�v���C���[�̈ړ�
		now_posX;
	}
	else if (jump) {

		if (now_speed > 0.0f){ //�W�����v���̃X�s�[�h��0���傫��������
			if (moveX == 1.0f && iNowKey & PAD_INPUT_LEFT) {

				/* �y�t�������͔��肪�܂������Ă��Ȃ����z */
				if (!reverse_input) {
					time = 0;

					reverse_input = true;  //�t�����ɓ��͂����̂��L�^
					after_slide = true;    //���n���̃X���C�h���I��
				}
				
				/* �y�X�s�[�h�̌v�Z�z */
				if (now_time < MINUS_FRAME) { //�W�����v�p�����Ԃ�MINUS_FRAME��菬������
					minus = (double)(speed / (MINUS_FRAME - now_time)); //�����l���p�����Ԃ���v�Z
					speed -= (double)minus;                             //�X�s�[�h����minus�l����������
				}
				/* �y�W�����v���̃X�s�[�h��1.5��菬�������́A�W�����v���ɋt���͂������ۂ̏����z */
				else if (after_slide && old_speed < 1.5f && !zero_start) {

					/* �y�X�s�[�h�̌v�Z�z */
					++time;

					if (speed > -3.0f) {
						acceleration = 0.1f;
						speed = -(acceleration * time + 0.1f);
					}
					else if (speed >= -3.0f) {
						speed = -3.0f;
					}
				}
				/* �yold_speed��1.5���傫�������ꍇ�z */
				else if (fall) {
					speed -= 0.1f;
				}

				if (speed < -1.5f)speed = -1.5f;

			}
			/* �y�ړ����������̎��ɉE�ɓ��͂������̏����z */
			else if (moveX == -1.0f && iNowKey & PAD_INPUT_RIGHT) {

				/* �y�t�������͔��肪�܂������Ă��Ȃ����z */
				if (!reverse_input) {
					time = 0;

					reverse_input = true;  //�t�����ɓ��͂����̂��L�^
					after_slide = true;    //���n���̃X���C�h���I��
				}

				if (now_time < MINUS_FRAME) { //34.0f�̓W�����v�ōō����B�_�ɍs���Ă��痎���͂��߂钼�O�̃t���[��
					minus = (double)(speed / (MINUS_FRAME - now_time)); //�X�s�[�h��������l���{�^���̓��͎��Ԃɂ���ĉ�
					speed -= (double)minus; //�X�s�[�h����l������
				}
				/* �y�W�����v���̃X�s�[�h��1.5��菬�������́A�W�����v���ɋt���͂������ۂ̏����z */
				else if (after_slide && old_speed < 1.5f && !zero_start) {

					/* �y�X�s�[�h�̌v�Z�z */
					++time;

					if (speed > -3.0f) {
						acceleration = 0.1f;
						speed = -(acceleration * time + 0.1f);
					}
					else if (speed >= -3.0f) {
						speed = -3.0f;
					}
				}
				else if (fall) { //�t�����֓��͒��̗�����
					speed -= 0.1f;
				}

				if (speed < -1.5f)speed = -1.5f;

			}
			else if(!reverse_input){ //�t�����ւ̓��͂��Ȃ���
				/* �y�E�܂��͍��ɓ��͂���Ă��鎞�z */
				if (iNowKey & PAD_INPUT_LEFT || iNowKey & PAD_INPUT_RIGHT) {

					/* �X�s�[�h�̌v�Z */
					time++;

					if (speed < 3.0f) {
						if (time > 23.0f)acceleration = 0.1f;
						speed = acceleration * time + 0.1f;
					}
					else if (speed >= 3.0f) {
						speed = 3.0f;
					}
				}
			}
		}
		
		else if (now_speed == 0.0f) { //�W�����v���̃X�s�[�h��0�̎�

			zero_start = true; //�X�s�[�h0�ŃW�����v���n�߂�������I��

			/* �y�t�����ւ̓��͂�����Ă��Ȃ����z */
			if (!reverse_input) {
				if (iNowKey & PAD_INPUT_LEFT) { //���ɓ��͂�����
					time++;
					
					moveX = -1.0f;
					reverse_input = true; //�t�����ւ̓��͔�����I���ɂ��� ���t�ւ̓��͂�����Ă��Ȃ��Ă�
				}
				else if (iNowKey & PAD_INPUT_RIGHT) { //�E�ɓ��͂�����
					time++;
					
					moveX = 1.0f;
					reverse_input = true; //�t�����ւ̓��͔�����I���ɂ��� ���t�ւ̓��͂�����Ă��Ȃ��Ă�
				}
			}
			/* �yreverse_input���I���̏�Ԃŋt�����ւ̓��͂��������z */
			else if(reverse_input && (iNowKey & PAD_INPUT_LEFT) || (iNowKey & PAD_INPUT_RIGHT)){
				if((moveX == 1.0f && iNowKey & PAD_INPUT_LEFT) || (moveX == -1.0f && iNowKey & PAD_INPUT_RIGHT))now_speed = speed; //now_speed���X�V �������ōX�V�������Ƃ�349�s�ڂ̏����ɓ���
				time++;
			}

			/* �y�X�s�[�h�̌v�Z�z */
			if (iNowKey & PAD_INPUT_LEFT || iNowKey & PAD_INPUT_RIGHT) { //���E�̂ǂ��炩�ɓ��͂���Ă����
				if (speed < 3.0f) {
					if (time > 23.0f)acceleration = 0.1f;
					speed = acceleration * time + 0.1f;
				}
				else if (speed >= 3.0f) {
					speed = 3.0f;
				}
			}
		}

		/* �y�|�W�V�����̑���z */
		if (!dont_move) { //�u���b�N�Ƃ̓����蔻�肪�Ȃ���
			p_posX = p_posX + (speed * moveX); //�v���C���[�̈ړ�
			now_posX;
		}
		
	}
	else { //�����Ă����Ԃœ��͂��Ȃ��Ȃ����Ƃ�
		if (!jump) { //�W�����v���Ă��Ȃ��Ƃ�
			Stop(); //�v���C���[�̓������������Ď~�߂鏈��
		}
	}
}

void Player::Dash() { //���鏈��
	if (!jump && dash == true && (iNowKey & PAD_INPUT_LEFT || iNowKey & PAD_INPUT_RIGHT)) {
		/* �y�������z */
		if (idle == true) {
			idle = false;
			stop_time = 0;
			acceleration = 0.1f;
		}
		ChangeImage();//�摜�؂�ւ�����


		/*if (!jump) {*/
			time++;

		/* �y�X�s�[�h�̌v�Z�z */
		if (walk == true) { //walk���I���̎�(��������J�ڂ��鎞)
			if (wtor_time == 0.0f) {
				wtor_time = time + 16.0f; //�����̃X�s�[�h���瑖���MAX�X�s�[�h�ɂ���܂ł̃t���[����ݒ�
			}
			if (time < wtor_time && speed < 5.0f) { //�܂��X�s�[�h��MAX�܂ŗ��Ă��Ȃ��Ƃ�
				wtor_time++;
				if (speed >= 3.0f)acceleration = 0.15f; //�����x���㏸
				speed = acceleration * wtor_time + 0.1f; 
			}
			else {
				speed = 5.0f; //�X�s�[�h��5�ɌŒ�
				walk = false; //������false��
				wtor_time = 0.0f; //�����^�C��������
			}
		}
		else { /* �y�ŏ�����_�b�V������Ƃ��z */

			if (time < 33.0f && speed < 5.0f) {
				if (speed >= 3.0f)acceleration = 0.15f;
				speed = acceleration * time + 0.1f;
			}
			else {
				speed = 5.0f;
			}

		}

		p_posX = p_posX + (speed * moveX); //�v���C���[�̈ړ�
		now_posX = p_posX;
		
	}

	/* �y�W�����v���̓����z */
	else if (jump) {

		if (now_speed > 0.0f) {  //�W�����v���̃X�s�[�h��0���傫��������
			if (moveX == 1.0f && iNowKey & PAD_INPUT_LEFT) {

				if (!reverse_input) {
					time = 0;

					reverse_input = true;  //�t�����ɓ��͂����̂��L�^
					after_slide = true;
				}

				if (now_time < MINUS_FRAME) {
					minus = (double)(speed / (MINUS_FRAME - now_time));
					speed -= (double)minus;
				}
				else if (after_slide && old_speed < 1.5f && !zero_start) {

					/* �y�X�s�[�h�̌v�Z�z */
					++time;

					if (speed > -5.0f) {
						acceleration = 0.1f;
						speed = -(acceleration * time + 0.1f);
					}
					else if (speed >= -5.0f) {
						speed = -5.0f;
					}

				}
				else if (fall) {
					speed -= 0.1f;
				}

				if (speed < -2.5f)speed = -2.5f;

			}
			else if (moveX == -1.0f && iNowKey & PAD_INPUT_RIGHT) {

				if (!reverse_input) {
					time = 0;

					reverse_input = true;  //�t�����ɓ��͂����̂��L�^
					after_slide = true;
				}

				if (now_time < MINUS_FRAME) { //34.0f�̓W�����v�ōō����B�_�ɍs���Ă��痎���͂��߂钼�O�̃t���[��
					minus = (double)(speed / (MINUS_FRAME - now_time)); //�X�s�[�h��������l���{�^���̓��͎��Ԃɂ���ĉ�
					speed -= (double)minus; //�X�s�[�h����l������
				}
				else if (after_slide && old_speed < 1.5f && !zero_start) {

					/* �y�X�s�[�h�̌v�Z�z */
					++time;

					if (speed > -5.0f) {
						acceleration = 0.1f;
						speed = -(acceleration * time + 0.1f);
						DrawFormatString(0, 340, GetColor(255, 255, 255), "390�s�ڂ̉�������");
					}
					else if (speed >= -5.0f) {
						speed = -5.0f;
					}
				}
				else if (fall) { //�t�����֓��͒��̗�����
					speed -= 0.1f;
				}

				if (speed < -2.5)speed = -2.5f; 
			}
			else if (!reverse_input) { //�t�����ւ̓��͂��Ȃ����@

				if (iNowKey & PAD_INPUT_LEFT || iNowKey & PAD_INPUT_RIGHT) {

					/* �y�X�s�[�h�̌v�Z�z */
					time++;
					if (speed < 5.0f) {
						if (time > 33.0f)acceleration = 0.1f;
						speed = acceleration * time + 0.1;
					}
					else if (speed >= 5.0f) {
						speed = 5.0f;
					}
				}
				
			}
		}
		else if (now_speed == 0.0f) { //�W�����v���̃X�s�[�h��0�̎�

			if (!reverse_input) {
				if (iNowKey & PAD_INPUT_LEFT) {
					time++;
					reverse_input = true;
					moveX = -1.0f;
				}
				else if (iNowKey & PAD_INPUT_RIGHT) {
					time++;
					reverse_input = true;
					moveX = 1.0f;
				}
			}
			/* �yreverse_input���I���̏�Ԃŋt�����ւ̓��͂��������z */
			else if (reverse_input && (iNowKey & PAD_INPUT_LEFT) || (iNowKey & PAD_INPUT_RIGHT)) {
				if ((moveX == 1.0f && iNowKey & PAD_INPUT_LEFT) || (moveX == -1.0f && iNowKey & PAD_INPUT_RIGHT))now_speed = speed; //now_speed���X�V �������ōX�V�������Ƃ�349�s�ڂ̏����ɓ���
				time++;
			}

			/* �y�X�s�[�h�̌v�Z�z */
			if (speed < 5.0f) {
				if (time > 33.0f)acceleration = 0.1f;
				speed = acceleration * time + 0.1f;
			}
			else if (speed >= 5.0f) {
				speed = 5.0f;
			}
		}

		if (!dont_move) {
			p_posX = p_posX + (speed * moveX); //�v���C���[�̈ړ�
			now_posX = p_posX;
		}
	}
	else {
		if (!jump)Stop();
	}
}

void Player::Stop() { //�~�܂鎞�̏���

	time = 0.0f;

	if (idle == false && !slide_turn) {
		idle = true; //�~�܂鎞�̃t���O��true�ɂ���

		now_speed = speed;
		stop_max = speed * 5.3f;
		acceleration = 0.05f;

		cnt = 0;
		cnt_limit = 0;
		array_num = 0;
	}

	if (idle) {
		if (stop_time < stop_max && speed > 0.0f) {
			++stop_time;

			speed = speed - (acceleration)*stop_time + 0.1f;

			p_posX = p_posX + (speed * moveX); //�v���C���[�̈ړ�
			now_posX = p_posX;
		}

		else {
			speed = 0.0f;
			player_num = 0;
		}
	}
}

void Player::SlideTurn() {

	time = 0;

	if (!jump && slide_turn == false) {

		slide_turn = true;
		now_speed = speed;
		stop_max = speed * 5.3f;
		acceleration = 0.05f;

		if (after_slide) {
			if (turn) {
				turn = false;
			}
			else {
				turn = true;
			}
		}

		cnt = 0;
		cnt_limit = 0;
		array_num = 0;
	}
	
	else if (slide_turn) {
		if (speed > 0.0f) {
			player_num = 4;
		}
		if (slid_time < stop_max && speed > 0.0f) {
			++slid_time;

			speed = speed - (acceleration)*slid_time + 0.1f;

			p_posX = p_posX + (speed * moveX); //�v���C���[�̈ړ�
			now_posX = p_posX;
		}

		else {

			slide_turn = false;
			moveX = -moveX;
			slid_time = 0.0f;
			speed = 0.0f;
			player_num = 0;

			if (after_slide) {
				if (turn) {
					turn = false;
				}
				else {
					turn = true;
				}
				after_slide = false;
			}
			
		}

		/* �X���C�h�^�[�����ɃW�����v�����ꍇ */
		if (jump) {
			slide_turn = false;
			moveX = -moveX;
			slid_time = 0.0f;

			if (after_slide) {
				if (turn) {
					turn = false;
				}
				else {
					turn = true;
				}
				after_slide = false;
			}
		}
	}
}

void Player::Jump() {
	if (!fall) {
		moveY = -1.0f;
	}

	if (iKeyFlg & PAD_INPUT_B && iNowKey & PAD_INPUT_LEFT) {
		j_direction = -1.0f;
		//moveX = j_direction;
	}
	else if (iKeyFlg & PAD_INPUT_B && iNowKey & PAD_INPUT_RIGHT) {
		j_direction = 1.0f;
		//moveX = j_direction;
	}

	/* �W�����v�t���O�̐؂�ւ� */
	if (isGround == true && iKeyFlg & PAD_INPUT_B) {
		

		if (jump == false) {
			//speed = now_speed;
			jump = true;
			jump_pos = p_posY;
			input_hold = true;
			j_accel = 1.0f;
			jump_power = 6.5f;

			idle = false;
			now_speed = speed;
			old_speed = now_speed;
			now_time = 0.0f;
			/*speed = 1;
			if (now_speed < 3 && now_speed >= 0.2) {
				speed = 1;
			}
			*/
			isGround = false;

			if (slide_turn) {
				slide_turn = false;
				after_slide = true;
			}
		}
	}

	/* �オ�鏈�� */
	if (jump) {
		//if()
		

		/*if (iNowKey != 0 && iNowKey != 32) {
			speed = 1;
		}
		if (now_speed <= 0.1) {
			speed = 0;
		}*/

		
		player_num = 5;
		if (input_hold == true && iNowKey & PAD_INPUT_B) {
			now_posY = p_posY;
			if (++jInput_time > 24) {
				jInput_time = 24;
			}
		}
		else {
			input_hold = false;
			//jump_power = 1.0f;
		}

		if (!fall) {
			++now_time;
			if (jInput_time < 8) { //��ԏ������W�����v
				if (p_posY + (p_h * 0.5) < (double)430.0f - ((double)32.0f * 1.0f)) {
					jump_max = jump_pos - (((double)32 * (double)1) + 16);
				}
				/*else {
					jump_max = now_posY - (double)16.0f;
				}*/

				//if(jInput_time)
				//jump_power -= 0.1f;
				//if (p_posY < p_posY - ((p_posY - jump_max) / 2)) {
				//	//jump_power = (j_accel * jInput_time) + 0.1;
				//	jump_power -= 0.01f;
				//}
				

				

				if (p_posY <= jump_max) {
					if (input_hold == false) {
						p_posY = jump_max;
						jump_power = 2.0f;

						if (++fall_time >= 3) {
							fall = true;
							fall_time = 0;
						}
					}
				}
				else {
					p_posY = p_posY + (jump_power * moveY);
				}
			}
			else {
				
				if (jInput_time == 24) {
					jump_max = jump_pos - (((double)32 * (double)4) + 16);
				}
				else {
					jump_max = now_posY - (double)16.0f;
				}

				//jump_max = jump_pos - ((128.0f/24.0f) * (jInput_time) + 16);

				if (jump_power > 1) {
					if (p_posY < jump_pos - 64) {
						jump_power -= 0.2f;
					}
					else {
						jump_power -= 0.1f;
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
				
				
				if (input_hold && p_posY > jump_pos - ((double)32.0f * (double)4.0f)) { /* �W�����v�������ςȂ� */
					p_posY = p_posY + (jump_power * moveY); //moveY �̓}�C�i�X�l
				}
				else if (input_hold == false && p_posY > jump_max) { /* �������ςȂ��ł͂Ȃ��� */
					p_posY = p_posY + (jump_power * moveY);
				}
				else if (input_hold && p_posY < jump_pos - ((double)32.0f * (double)4.0f)) {
					input_hold = false;
				}
				else if (input_hold == false) {
					if (++fall_time >= 3) {
						fall = true;
						fall_time = 0;
						jump_power = 2.0f;
					}

				}
			}
		}

		/* �������� */

		Fall();
	}
}

void Player::Fall() {

	if (fall) {
		player_num = 5;
		moveY = 1.0f;
		DrawFormatString(0, 300, GetColor(255, 255, 255), "������");
		if (++fall_time < 24) {
			if (fall_time < 8) {
				jump_power += 0.6f;
			}
			
		}

		if (!isGround) {
			p_posY = p_posY - (-jump_power * moveY);
		}

		if(p_posX - (p_w * 0.5) >= 0 && p_posX - (p_w * 0.5) < 498 &&  p_posY + (p_h * 0.5) > block.gro_posY){

			isGround = true;
			p_posY = block.gro_posY - (p_h * 0.5);

			/* ������ */
			jump = false;
			fall = false;
			jInput_time = 0;
			fall_time = 0;
			jump_power = 1.0f;
			jump_max = 0;
			
			if (after_slide && !(iNowKey & PAD_INPUT_LEFT || iNowKey & PAD_INPUT_RIGHT)) {
				
				moveX = -moveX;
				speed = abs(speed);
				SlideTurn();
			}
			else {
				after_slide = false;
			}
			reverse_input = false;
			zero_start = false;
		}

	}
	
}

void Player::KillY() {
	if (p_posY + (p_h * 0.5) > 608) {
		p_posX = startX;
		now_posX = p_posX;
		p_posY = startY;

		isGround = true;
		jump = false;
		fall = false;
		jInput_time = 0;
		fall_time = 0;
		jump_power = 1.0f;
		jump_max = 0;
	}
}

void Player::ShowDebug() { //�f�o�b�O�\��
	/*DrawFormatString(0, 30, GetColor(255, 255, 255), "���͎���:%lf", time);
	DrawFormatString(0, 50, GetColor(255, 255, 255), "�v���C���[�̉摜�ԍ�:%d", player_num);
	
	DrawFormatString(0, 170, GetColor(255, 255, 255), "�摜�̌���[0:�E,1:��]%d", turn);
	DrawFormatString(0, 110, GetColor(255, 255, 255), "�X�s�[�h%f", abs(speed));
	DrawFormatString(0, 130, GetColor(255, 255, 255), "speed�v�Z%lf = %lf + (%lf * %f)", p_posX,p_posX,speed,moveX);
	DrawFormatString(0, 150, GetColor(255, 255, 255), "N�̒l : %d", slide_turn);
	DrawFormatString(0, 220, GetColor(255, 255, 255), "stop_time : %d",stop_time);
	DrawFormatString(0, 240, GetColor(255, 255, 255), "cnt_limit : %d", cnt_limit);
	DrawFormatString(0, 260, GetColor(255, 255, 255), "idle : %d", idle);
	DrawFormatString(0, 280, GetColor(255, 255, 255), "jump_max : %lf", jump_max);
	DrawFormatString(0, 300, GetColor(255, 255, 255), "jInput_time : %lf", jInput_time);
	DrawFormatString(0, 320, GetColor(255, 255, 255), "�v���C���[��Y���W : %lf", p_posY);
	DrawFormatString(0, 360, GetColor(255, 255, 255), "�W�����v�p���[: %lf", jump_power);*/
	DrawFormatString(0, 360, GetColor(255, 255, 255), "slide_turn: %d", slide_turn);
	DrawFormatString(0, 110, GetColor(255, 255, 255), "�����x%f", acceleration);
	DrawFormatString(0, 70, GetColor(255, 255, 255), "���݂̓���[��:2,�E:4]:%d", iNowKey);
	DrawFormatString(0, 30, GetColor(255, 255, 255), "���͎���:%lf", time);
	DrawFormatString(0, 130, GetColor(255, 255, 255), "jumpmax:%lf", jump_max);
	DrawFormatString(0, 150, GetColor(255, 255, 255), "�����̌v�Z :  %lf - (- %lf *  %lf)",p_posY,-jump_power,moveY);
	DrawFormatString(0, 170, GetColor(255, 255, 255), "�ړ��̌���[1.0:�E,-1.0:��]%.1lf", moveX);
	DrawFormatString(0, 190, GetColor(255, 255, 255), "�X�s�[�h%lf", speed);
	DrawFormatString(0, 230, GetColor(255, 255, 255), "now_time%lf", now_time);
	DrawFormatString(0, 250, GetColor(255, 255, 255), "34 - now_time = %lf", 34 - now_time);
	DrawFormatString(0, 320, GetColor(255, 255, 255), "p_posX : %lf", p_posX);
	DrawFormatString(0, 340, GetColor(255, 255, 255), "now_posX : %lf", now_posX);
	DrawFormatString(0, 380, GetColor(255, 255, 255), "idle : %d", idle);
	DrawFormatString(0, 400, GetColor(255, 255, 255), "�W�����v���̕��� : %f", j_direction);
	DrawFormatString(0, 420, GetColor(255, 255, 255), "�t����1.true 0.false: %d", reverse_input);

	/*�y�����蔻��f�o�b�O�z*/
	//DrawCircle(p_posX, p_posY,5,1,1); //Player�̒��S���W

	//DrawCircle(p_posX - p_w/2, p_posY, 5, GetColor(255, 255, 255), 1); //Player�̍��[
	//DrawCircle(p_posX + p_w / 2, p_posY, 5, GetColor(255, 255, 255), 1); //Player�̉E�[
	//DrawCircle(p_posX, p_posY - p_h / 2, 5, GetColor(255, 255, 255), 1); //Player�̏��
	//DrawCircle(p_posX, p_posY + p_h / 2, 5, GetColor(255, 255, 255), 1); //Player�̒��
}
