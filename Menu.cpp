#include "DxLib.h"
#include "SceneMgr.h"
#include "Input.h"
#include "Menu.h"

static int mImageHandle;    //�摜�n���h���i�[�p�ϐ�
static int MenuNumber = 0;  //���j���[�J�[�\���Ǘ��p�ϐ�

SceneMgr scenemgr;

//������
void Menu::Menu_Initialize(){
    mImageHandle = LoadGraph("images/Scene_Menu.png");    //�摜�̃��[�h
}

//�I������
void Menu::Menu_Finalize(){
    DeleteGraph(mImageHandle);    //�摜�̉��
}

//�X�V
void Menu::Menu_Update(){
    //���j���[�J�[�\���ړ�����
    if (iKeyFlg & PAD_INPUT_DOWN) {
        if (++MenuNumber > 2) MenuNumber = 0;
    }
    if (iKeyFlg & PAD_INPUT_UP) {
        if (--MenuNumber < 0) MenuNumber = 2;
    }

    if (iKeyFlg == PAD_INPUT_1) { //Z�L�[��������Ă�����
        scenemgr.SceneMgr_ChangeScene(eScene_Game);//�V�[�����Q�[����ʂɕύX
    }
    if (iKeyFlg == PAD_INPUT_4) { //A�L�[��������Ă�����
        scenemgr.SceneMgr_ChangeScene(eScene_Config);//�V�[����ݒ��ʂɕύX
    }
}

//�`��
void Menu::Menu_Draw(){
    //�^�C�g���摜�\��
    DrawGraph(0, 0, mImageHandle, FALSE);

    //���j���[�J�[�\���i�O�p�`�j�̕\��
    DrawString(20, 400, "Z�L�[�ŃQ�[�����", GetColor(136, 136, 255));

}
