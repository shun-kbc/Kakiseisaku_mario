#include "DxLib.h"
#include "SceneMgr.h"
#include "Input.h"
#include "Menu.h"

static int mImageHandle;    //画像ハンドル格納用変数
static int MenuNumber = 0;  //メニューカーソル管理用変数

SceneMgr scenemgr;

//初期化
void Menu::Menu_Initialize(){
    mImageHandle = LoadGraph("images/Scene_Menu.png");    //画像のロード
}

//終了処理
void Menu::Menu_Finalize(){
    DeleteGraph(mImageHandle);    //画像の解放
}

//更新
void Menu::Menu_Update(){
    //メニューカーソル移動処理
    if (iKeyFlg & PAD_INPUT_DOWN) {
        if (++MenuNumber > 2) MenuNumber = 0;
    }
    if (iKeyFlg & PAD_INPUT_UP) {
        if (--MenuNumber < 0) MenuNumber = 2;
    }

    if (iKeyFlg == PAD_INPUT_1) { //Zキーが押されていたら
        scenemgr.SceneMgr_ChangeScene(eScene_Game);//シーンをゲーム画面に変更
    }
    if (iKeyFlg == PAD_INPUT_4) { //Aキーが押されていたら
        scenemgr.SceneMgr_ChangeScene(eScene_Config);//シーンを設定画面に変更
    }
}

//描画
void Menu::Menu_Draw(){
    //タイトル画像表示
    DrawGraph(0, 0, mImageHandle, FALSE);

    //メニューカーソル（三角形）の表示
    DrawString(20, 400, "Zキーでゲーム画面", GetColor(136, 136, 255));

}
