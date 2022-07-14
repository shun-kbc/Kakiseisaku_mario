#include "DxLib.h"
#include "SceneMgr.h"
#include "Input.h"
#include "Game.h"
#include "Player.h"
#include "Block.h"

//static int mImageHandle;    //画像ハンドル格納用変数

Player player;
Block block;

//初期化
void Game::Game_Initialize(){
    //mImageHandle = LoadGraph("images/Scene_Game.png");    //画像のロード

    block.Block_Initialize();
    player.Player_Initialize();
    
}

//終了処理
void Game::Game_Finalize(){
    //DeleteGraph(mImageHandle);    //画像の解放
    block.Block_Finalize();
    player.Player_Finalize();
}

//更新
void Game::Game_Update(){
    //if (iKeyFlg == PAD_INPUT_2) { //Xキーが押されていたら
    //    SceneMgr_ChangeScene(eScene_Menu);//シーンをメニューに変更
    //}
    block.Block_Update();
    player.Player_Update();
}

//描画
void Game::Game_Draw(){

    DrawBox(0,0,640,480,GetColor(92, 148, 252),TRUE); /*92,148,252*/
    //DrawGraph(0,0,mImageHandle,FALSE);
    //DrawString(20, 380, "ゲーム画面です。", GetColor(136, 136, 255));/
    block.Block_Draw();
    player.Player_Draw();
}
