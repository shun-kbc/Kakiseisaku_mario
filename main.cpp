#include "DxLib.h"
#include "SceneMgr.h"
#include "Input.h"
#include "FPS.h"

/***********************************************
* �v���O�����̊J�n
***********************************************/
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                                        LPSTR lpCmdLine, int nCmdShow)
{
    ChangeWindowMode(TRUE);		        // �E�B���h�E���[�h�ύX
    if (DxLib_Init() == -1) return -1;	// DX���C�u�����̏���������
    SetDrawScreen(DX_SCREEN_BACK);	    // �`����ʂ𗠂ɂ���
    
    SceneMgr scenemgr;

    scenemgr.SceneMgr_Initialize();  //

    Fps fps;
    Input input;

    while (ProcessMessage() == 0 ) {    // ���b�Z�[�W����

        ClearDrawScreen();	// ��ʏ���

        input.InputKey();         // ����

        scenemgr.SceneMgr_Update();  // �X�V
        scenemgr.SceneMgr_Draw();    // �`��
        fps.Update();	//�X�V
        fps.Draw();		//�`��
        ScreenFlip();	    // ��ʍX�V
        fps.Wait();		//�ҋ@
               
    }

    scenemgr.SceneMgr_Finalize();

    DxLib_End(); // DX���C�u�����I������
    return 0;
}