#pragma once
#include "Input.h"

class Menu : Input{

public:

	static void Menu_Initialize();//初期化
	static void Menu_Finalize();//終了処理
	static void Menu_Update();//更新
	static void Menu_Draw();//描画

};

