#pragma once
#include "Player.h"
#include "Block.h"

class Game {

public:

	static void Game_Initialize();//������
	static void Game_Finalize();//�I������
	static void Game_Update();//�X�V
	static void Game_Draw();//�`��

};

extern Player player;
extern Block block;

