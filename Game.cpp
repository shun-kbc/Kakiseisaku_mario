#include "DxLib.h"
#include "SceneMgr.h"
#include "Input.h"
#include "Game.h"
#include "Player.h"
#include "Block.h"

//static int mImageHandle;    //�摜�n���h���i�[�p�ϐ�

Player player;
Block block;

//������
void Game::Game_Initialize(){
    //mImageHandle = LoadGraph("images/Scene_Game.png");    //�摜�̃��[�h

    block.Block_Initialize();
    player.Player_Initialize();
    
}

//�I������
void Game::Game_Finalize(){
    //DeleteGraph(mImageHandle);    //�摜�̉��
    block.Block_Finalize();
    player.Player_Finalize();
}

//�X�V
void Game::Game_Update(){
    //if (iKeyFlg == PAD_INPUT_2) { //X�L�[��������Ă�����
    //    SceneMgr_ChangeScene(eScene_Menu);//�V�[�������j���[�ɕύX
    //}
    block.Block_Update();
    player.Player_Update();
}

//�`��
void Game::Game_Draw(){

    DrawBox(0,0,640,480,GetColor(92, 148, 252),TRUE); /*92,148,252*/
    //DrawGraph(0,0,mImageHandle,FALSE);
    //DrawString(20, 380, "�Q�[����ʂł��B", GetColor(136, 136, 255));/
    block.Block_Draw();
    player.Player_Draw();
}
