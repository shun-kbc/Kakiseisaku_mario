#pragma once
#include "Player.h"

#define BLOCK_WIDTH 32
#define BLOCK_HEIGHT 30 

class Block {

	static int mImageKaraBlock;    //‰æ‘œƒnƒ“ƒhƒ‹Ši”[—p•Ï”

public:
	double b_x;
	double b_y;
	double b_w;
	double b_h;

	double gro_posY;

	void Block_Initialize();
	void Block_Finalize();
	void Block_Update();
	void Block_Draw();
	double HitBoxPlayer();

	
};
//class SetBlock : Block {
//
// };