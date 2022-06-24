#include "DxLib.h"
#include "SceneMgr.h"
#include "Input.h"
#include "FPS.h"

/***********************************************
* プログラムの開始
***********************************************/
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                                        LPSTR lpCmdLine, int nCmdShow)
{
    ChangeWindowMode(TRUE);		        // ウィンドウモード変更
    if (DxLib_Init() == -1) return -1;	// DXライブラリの初期化処理
    SetDrawScreen(DX_SCREEN_BACK);	    // 描画先画面を裏にする
    
    SceneMgr scenemgr;

    scenemgr.SceneMgr_Initialize();  //

    Fps fps;
    Input input;

    while (ProcessMessage() == 0 ) {    // メッセージ処理

        ClearDrawScreen();	// 画面消去

        input.InputKey();         // 入力

        scenemgr.SceneMgr_Update();  // 更新
        scenemgr.SceneMgr_Draw();    // 描画
        fps.Update();	//更新
        fps.Draw();		//描画
        ScreenFlip();	    // 画面更新
        fps.Wait();		//待機
               
    }

    scenemgr.SceneMgr_Finalize();

    DxLib_End(); // DXライブラリ終了処理
    return 0;
}