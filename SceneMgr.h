#pragma once
#include "Menu.h"
#include "Game.h"

typedef enum {
    eScene_Menu,    //���j���[���
    eScene_Game,    //�Q�[�����
    eScene_Config,  //�ݒ���

    eScene_None,    //����
} eScene ;

class SceneMgr : Menu,Game{

public:

    void SceneMgr_Initialize();//������
    void SceneMgr_Finalize();//�I������
    void SceneMgr_Update();//�X�V
    void SceneMgr_Draw();//�`��

    // ���� nextScene �ɃV�[����ύX����
    void SceneMgr_ChangeScene(eScene nextScene);

    // ����scene���W���[��������������
    static void SceneMgr_InitializeModule(eScene scene);

    // ����scene���W���[���̏I���������s��
    static void SceneMgr_FinalizeModule(eScene scene);
};

