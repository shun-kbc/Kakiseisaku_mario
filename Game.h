#pragma once
#include "Player.h"
#include "Block.h"

class Game {

public:

	static void Game_Initialize();//初期化
	static void Game_Finalize();//終了処理
	static void Game_Update();//更新
	static void Game_Draw();//描画

};

extern Player player;
extern Block block;

