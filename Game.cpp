#include "DxLib.h"
#include "SceneMgr.h"
#include "Input.h"
#include "Game.h"
#include "Player.h"

static int mImageHandle;    //�摜�n���h���i�[�p�ϐ�
Player player;

//������
void Game::Game_Initialize(){
    mImageHandle = LoadGraph("images/Scene_Game.png");    //�摜�̃��[�h
    player.Player_Initialize();
}

//�I������
void Game::Game_Finalize(){
    DeleteGraph(mImageHandle);    //�摜�̉��
    player.Player_Finalize();
}

//�X�V
void Game::Game_Update(){
    //if (iKeyFlg == PAD_INPUT_2) { //X�L�[��������Ă�����
    //    SceneMgr_ChangeScene(eScene_Menu);//�V�[�������j���[�ɕύX
    //}
    player.Player_Update();
}

//�`��
void Game::Game_Draw(){
    DrawGraph(0,0,mImageHandle,FALSE);
    DrawString(20, 380, "�Q�[����ʂł��B", GetColor(136, 136, 255));
    DrawString(20, 400, "Esc�L�[�������ƃ��j���[��ʂɖ߂�܂��B", GetColor(136, 136, 255));
    player.Player_Draw();
}
