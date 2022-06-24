#pragma once
#include "Menu.h"
#include "Game.h"

typedef enum {
    eScene_Menu,    //メニュー画面
    eScene_Game,    //ゲーム画面
    eScene_Config,  //設定画面

    eScene_None,    //無し
} eScene ;

class SceneMgr : Menu,Game{

public:

    void SceneMgr_Initialize();//初期化
    void SceneMgr_Finalize();//終了処理
    void SceneMgr_Update();//更新
    void SceneMgr_Draw();//描画

    // 引数 nextScene にシーンを変更する
    void SceneMgr_ChangeScene(eScene nextScene);

    // 引数sceneモジュールを初期化する
    static void SceneMgr_InitializeModule(eScene scene);

    // 引数sceneモジュールの終了処理を行う
    static void SceneMgr_FinalizeModule(eScene scene);
};

