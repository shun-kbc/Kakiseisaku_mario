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
	acceleration = 0.05f;
	j_accel = 1.0f;
	jump_power = 1.0f;
	moveX = 1.0f;
	moveY = -1.0f;

	startX = 100;
	startY = block.gro_posY - 16/*(p_h * 0.5)*/;

	p_posX = startX;
	now_posX = p_posX;
	
	p_posY = startY;
	now_posY = p_posX;

	jump_pos = 0;

	minus = 0; //�����l
	

	p_w = 32;
	p_h = 32;

	//N = 7;
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
		reverse_input = false;
		Fall();
	}
	
	//else if (p_posX - (p_w * 0.5) <  500 && p_posY + (p_h * 0.5) >= 320) {
	//	//fall = false;
	//	
	//	p_posY = 320 -(p_h * 0.5);

	//	/* ������ */
	//	fall = false;
	//	fall_time = 0;
	//	jump_max = 0;
	//}



	if (block.HitBoxPlayer()) { //�u���b�N�Ƃ̓����蔻��

		/* �y�u���b�N��ɏ�鎞�̏����z */
		if ((p_posX + (p_w * 0.5) > block.b_x - (block.b_w * 0.5)) && (p_posX - (p_w * 0.5) < block.b_x + (block.b_w * 0.5)) && p_posY < block.b_y - (block.b_h * 0.5)) {
			/* ������ */
			jump = false;
			fall = false;
			jInput_time = 0;
			fall_time = 0;
			jump_power = 1.0f;

			reverse_input = false;

			p_posY = block.b_y - (block.b_h);

			isGround = true;
		}

		/* �u���b�N��(��) */
		if (/*(speed < 0.5) && */(p_posX + (p_w * 0.5) > block.b_x - (block.b_w * 0.5) && p_posX < block.b_x - (block.b_w * 0.5)) && (p_posY - (p_h * 0.5) < block.b_y + (block.b_h * 0.5) && p_posY + (p_h * 0.5) > block.b_y - (block.b_h * 0.5) + 2)) {
			dont_move = true;
			if ((speed < 1.5f) && p_posX + (p_w * 0.5) > block.b_x - (p_w * 0.5)) {
				p_posX -= 2.0f;
				now_posX = p_posX;
			}
			else {
				p_posX = (block.b_x - (block.b_w * 0.5)) - (p_w * 0.5);
				now_posX = p_posX;
			}
			
		}
		/* �u���b�N��(�E) */
		if (/*(speed < 0.5) && */(p_posX - (p_w * 0.5) < block.b_x + (block.b_w * 0.5) && p_posX > block.b_x + (block.b_w * 0.5)) && (p_posY - (p_h * 0.5) < block.b_y + (block.b_h * 0.5) && p_posY + (p_h * 0.5) > block.b_y - (block.b_h * 0.5) + 2)) {
			dont_move = true;
			if ((speed < 1.5f) && p_posX - (p_w * 0.5) < block.b_x + (p_w * 0.5)) {
				p_posX += 2.0f;
				now_posX = p_posX;
			}
			else {
				p_posX = (block.b_x + (block.b_w * 0.5)) + (p_w * 0.5);
				now_posX = p_posX;
			}
		}
	}
	else {
		dont_move = false;
	}
	
	KillY();
}

void Player::Player_Draw()//�`��
{
	/*�y�W�����v�̍����f�o�b�O�z*/
	//DrawBox(0,0,640, jump_max,GetColor(100,100,255),true);
	//DrawBox(block.b_x - (block.b_w * 0.5), block.b_y + (block.b_h * 0.5), block.b_x + (block.b_w * 0.5), block.b_y - (block.b_h * 0.5), GetColor(255, 255, 255), TRUE);
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

	ShowDebug();
}

void Player::ChangeImage() {
	if(!jump){
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

		DrawFormatString(0, 50, GetColor(255, 255, 255), "�W�����v���ł͂���܂���");

		/* �X���C�h�^�[�����ɋt�����ɓ��͂������̏��� */
		if (slide_turn) { //�X���C�h�^�[����true�̎�
			if (left_turn && iKeyFlg & PAD_INPUT_RIGHT) { //�^�[����false�ŉE���͂���Ă��鎞
				turn_cancel = true;
				slide_turn = false;
			}
			else if (right_turn && iKeyFlg & PAD_INPUT_LEFT) { //�^�[����true�ō����͂���Ă��鎞
				turn_cancel = true;
				slide_turn = false;
				//turn = true;
			}
		}

		/* �X���C�h�^�[���ɓ��鎞�̏��� */
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

		/* �X���C�h�^�[�����ɓ��͂������ꂽ�ꍇ�͉��ŏ�����r���ĊJ */
		if (slide_turn && (iNowKey == 0 || iNowKey== 16 || after_slide)) {
			SlideTurn();
		}
		/* �����̏���������slide_turn�݂̂ɂ���ƁA�L�[���͂����Ă����Ԃ���2��SlideTurn�̏����ɓ����Ă��܂� */
		
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
		if (idle == true) {
			idle = false;
			walk = true;
			stop_time = 0;
			speed = 0.0f;
			acceleration = 0.05f;
		}

		ChangeImage();//�摜�؂�ւ�����

		if (!jump) {
			time++;
			if (speed < 3.0f) {
				if (time > 23.0f)acceleration = 0.1f;
				speed = acceleration * time + 0.1;
			}
			else {
				speed = 3.0f;
			}

			p_posX = p_posX + (speed * moveX); //�v���C���[�̈ړ�
			now_posX;
		}
	}
	else if (jump) {

		if (now_speed > 0.0f){
			if (moveX == 1.0f && iNowKey & PAD_INPUT_LEFT) {

				reverse_input = true;  //�t�����ɓ��͂����̂��L�^
				after_slide = true;

				if (now_time < 34.0f) {
					minus = (double)(speed / (34.0f - now_time));
					speed -= (double)minus;
				}
				else if (fall) {
					speed -= 0.1f;
				}

				if (speed < -1.5f)speed = -1.5f;

			}
			else if (moveX == -1.0f && iNowKey & PAD_INPUT_RIGHT) {

				reverse_input = true;  //�t�����ɓ��͂����̂��L�^
				after_slide = true;

				if (now_time < 34.0f) { //34.0f�̓W�����v�ōō����B�_�ɍs���Ă��痎���͂��߂钼�O�̃t���[��
					minus = (double)(speed / (34.0f - now_time)); //�X�s�[�h��������l���{�^���̓��͎��Ԃɂ���ĉ�
					speed -= (double)minus; //�X�s�[�h����l������
				}
				else if (fall) { //�t�����֓��͒��̗�����
					speed -= 0.1f;
				}

				if (speed < -1.5f)speed = -1.5f;

			}
			else if(!reverse_input){ //�t�����ւ̓��͂��Ȃ����@
				time++;
				if (speed < 3.0f) {
					if (time > 23.0f)acceleration = 0.1f;
					speed = acceleration * time + 0.1f;
					DrawFormatString(0, 340, GetColor(255, 255, 255), "390�s�ڂ̉�������");
				}
				else if (speed >= 3.0f) {
					speed = 3.0f;
				}
			}

		}
		else if (now_speed == 0.0f) {

			time++;
			if (iNowKey & PAD_INPUT_RIGHT) {
				if (speed < 3.0f) {
					if (time > 23.0f)acceleration = 0.1f;
					speed = acceleration * time + 0.1f;
					DrawFormatString(0, 340, GetColor(255, 255, 255), "404�s�ڂ̉�������");
				}
				else if (speed >= 3.0f) {
					speed = 3.0f;
				}
				moveX = 1.0f;
			}
			else if (iNowKey & PAD_INPUT_LEFT) {
				if (speed < 3.0f) {
					if (time > 23.0f)acceleration = 0.1f;
					speed = acceleration * time + 0.1f;
					DrawFormatString(0, 340, GetColor(255, 255, 255), "414�s�ڂ̉�������");
				}
				else if (speed >= 3.0f) {
					speed = 3.0f;
				}
				moveX = -1.0f;
			}
		}

		if (!dont_move/* && (iNowKey & PAD_INPUT_LEFT) || (iNowKey & PAD_INPUT_RIGHT)*/) {
			p_posX = p_posX + (speed * moveX); //�v���C���[�̈ړ�
			now_posX;
		}
		
	}
	else {
		if (!jump) {
			Stop();
		}
	}
}

void Player::Dash() { //���鏈��
	if (!jump && dash == true && (iNowKey & PAD_INPUT_LEFT || iNowKey & PAD_INPUT_RIGHT)) {
		if (idle == true) {
			idle = false;
			stop_time = 0;
			acceleration = 0.1f;
		}
		ChangeImage();//�摜�؂�ւ�����


		/*if (!jump) {*/
			time++;

		/* �������� */
		if (walk == true) {
			if (wtor_time == 0.0f) {
				wtor_time = time + 16.0f;
			}
			if (time < wtor_time && speed < 5.0f) {
				wtor_time++;
				if (speed >= 3.0f)acceleration = 0.15f;
				speed = acceleration * wtor_time + 0.1f;
			}
			else {
				speed = 5.0f;
				walk = false;
				wtor_time = 0.0f; //�����^�C��������
			}
		}
		else {
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
	else if (jump) {
		if (now_speed > 0.0f) {
			if (moveX == 1.0f && iNowKey & PAD_INPUT_LEFT) {

				reverse_input = true;  //�t�����ɓ��͂����̂��L�^
				after_slide = true;

				if (now_time < 34.0f) {
					minus = (double)(speed / (34.0f - now_time));
					speed -= (double)minus;
				}
				else if (fall) {
					speed -= 0.1f;
				}

				if (speed < -2.5f)speed = -2.5f;

			}
			else if (moveX == -1.0f && iNowKey & PAD_INPUT_RIGHT) {

				reverse_input = true;  //�t�����ɓ��͂����̂��L�^
				after_slide = true;

				if (now_time < 34.0f) { //34.0f�̓W�����v�ōō����B�_�ɍs���Ă��痎���͂��߂钼�O�̃t���[��
					minus = (double)(speed / (34.0f - now_time)); //�X�s�[�h��������l���{�^���̓��͎��Ԃɂ���ĉ�
					speed -= (double)minus; //�X�s�[�h����l������
				}
				else if (fall) { //�t�����֓��͒��̗�����
					speed -= 0.1f;
				}

				if (speed < -2.5)speed = -2.5f; 
			}
			else if (!reverse_input) { //�t�����ւ̓��͂��Ȃ����@
				time++;
				if (speed < 5.0f) {
					if (time > 33)acceleration = 0.1f;
					speed = acceleration * time + 0.1;
				}
				else if (speed >= 5.0f) {
					speed = 5.0f;
				}
			}
		}
		else if (now_speed == 0.0f) {

			time++;


			if (iNowKey & PAD_INPUT_RIGHT) {
				if (speed < 5.0f) {
					if (time > 33.0f)acceleration = 0.1f;
					speed = acceleration * time + 0.1f;
					DrawFormatString(0, 340, GetColor(255, 255, 255), "531�s�ڂ̉�������");
				}
				else if (speed >= 5.0f) {
					speed = 5.0f;
				}
				moveX = 1.0f;
			}
			else if (iNowKey & PAD_INPUT_LEFT) {
				if (speed < 5.0f) {
					if (time > 33.0f)acceleration = 0.1f;
					speed = acceleration * time + 0.1f;
					DrawFormatString(0, 340, GetColor(255, 255, 255), "542�s�ڂ̉�������");
				}
				else if (speed >= 5.0f) {
					speed = 5.0f;
				}
				moveX = -1.0f;
			}
		}

		if (!dont_move/* && (iNowKey & PAD_INPUT_LEFT) || (iNowKey & PAD_INPUT_RIGHT)*/) {
			p_posX = p_posX + (speed * moveX); //�v���C���[�̈ړ�
			now_posX = p_posX;
		}
	}
	else {
		if (!jump)Stop();
	}

	/*if (speed >= 5.0f) {
		speed = 5.0f;
	}*/
	
}

void Player::Stop() { //�~�܂鎞�̏���
	time = 0.0f;

	if (idle == false && !slide_turn) {
		idle = true; //�~�܂鎞�̃t���O��true�ɂ���

		now_speed = speed;
		stop_max = speed * 5.3f;
		acceleration = 0.05f;

		cnt = 0;
		array_num = 0;
	}

	if (idle) {
		if (stop_time < stop_max && speed > 0.0f) {
			++stop_time;


			//if (stop_time > 20)acceleration = 0.05f;
			speed = speed - (acceleration)*stop_time + 0.1f;

			p_posX = p_posX + (speed * moveX); //�v���C���[�̈ړ�
			now_posX = p_posX;
		}

		else {
			speed = 0.0f;
			player_num = 0;
		}
	}
	/*if (!slide_turn) {
		
	}*/
		
}

void Player::SlideTurn() {
	time = 0;
	

	if (!jump && slide_turn == false) {
		slide_turn = true;
		//speed = 5.0f;
		now_speed = speed;
		stop_max = speed * 5.3f;
		acceleration = 0.05f;
		//speed = now_speed;
		if (after_slide) {
			if (turn) {
				turn = false;
			}
			else {
				turn = true;
			}
			//after_slide = false;
		}
	}
	
	else if (slide_turn) {
		if (speed > 0.0f) {
			player_num = 4;
		}
		if (slid_time < stop_max && speed > 0.0f) {
			++slid_time;


			//if (stop_time > 20)acceleration = 0.05f;
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
		DrawFormatString(0, 300, GetColor(255, 255, 255), "������");
		if (++fall_time < 24) {
			if (fall_time < 8) {
				//jump_power = (j_accel * fall_time) + 0.1;
				jump_power += 0.6f;
			}
			
			//else {
			//	if (p_posY + (p_h * 0.5) < 256/* ����ł����͒n�ʂƂ̓����蔻��ɂȂ� */) {
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
	//p_posY = p_posY - (-jump_power * moveY);



	/*�y�����蔻��f�o�b�O�z*/
	//DrawCircle(p_posX, p_posY,5,1,1); //Player�̒��S���W

	//DrawCircle(p_posX - p_w/2, p_posY, 5, GetColor(255, 255, 255), 1); //Player�̍��[
	//DrawCircle(p_posX + p_w / 2, p_posY, 5, GetColor(255, 255, 255), 1); //Player�̉E�[
	//DrawCircle(p_posX, p_posY - p_h / 2, 5, GetColor(255, 255, 255), 1); //Player�̏��
	//DrawCircle(p_posX, p_posY + p_h / 2, 5, GetColor(255, 255, 255), 1); //Player�̒��
}

//double Player::CircOut(double t, double totaltime, double min, double max)
//{
//	max -= min;
//	t = t / totaltime - 1;
//	return max * sqrt(1 - t * t) + min;
//}
//
//bool Player::Continue() {
//	bool cont;
//	if (iNowKey != iOldKey) {
//		if (++continue_time >= 10) {
//			cont = true;
//			return cont;
//		}
//		else {
//			continue_time = 0;
//			cont = false;
//			return cont;
//		}
//	}
//}