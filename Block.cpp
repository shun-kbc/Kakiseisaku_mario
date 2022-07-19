#include "Dxlib.h"
#include "Block.h"
#include "Player.h"
#include "Game.h"

int Block::mImageKaraBlock = {};    //画像ハンドル格納用変数

void Block::Block_Initialize() {
	mImageKaraBlock = LoadGraph("images/1-1/kara_block.png");

	gro_posY = 432;

	b_x = 320;
	b_y = gro_posY - ((double)32.0f * (double)4.0f);
	b_w = 30;
	b_h = 30;

	
}

void Block::Block_Finalize() {
	DeleteGraph(mImageKaraBlock);
}

void Block::Block_Update() {

	/* マリオがブロックの下からぶつかったとき */
	if (HitBoxPlayer()) {
		if ((player.p_posX > b_x - (b_w * 0.5) && player.p_posX < b_x + (b_w * 0.5)) && player.p_posY - (player.p_h * 0.5f) < b_y + (b_h * 0.5f) && player.p_posY - (player.p_h * 0.5f) > b_y - (b_h * 0.5f)) {
			player.fall = true;
			player.jump_power = 2.0f;
			/* ここにブロックが浮いたり、壊れたりする処理を入れる */
		}
	}
	

	//DrawFormatString(0, 320, GetColor(255, 255, 255), "Playerの位置 : %lf", player.p_posX);
	//DrawFormatString(0, 340, GetColor(255, 255, 255), "ブロックの高さ : %lf", b_h);


	
}

void Block::Block_Draw() {
	DrawRotaGraph(b_x, b_y/* + BLOCK_HEIGHT*/,1.0f,0,mImageKaraBlock,true,false);
	DrawBox(0, 480, 500, gro_posY, GetColor(116,80,48), TRUE);


	/*【当たり判定デバッグ】*/
	//DrawCircle(b_x, b_y, 5, 1, 1); //Blockの中心座標

	//DrawCircle(b_x - b_w / 2, b_y, 5, GetColor(255, 255, 255), 1); //Blockの左端
	//DrawCircle(b_x + b_w / 2, b_y, 5, GetColor(255, 255, 255), 1); //Blockの右端
	//DrawCircle(b_x, b_y - b_h / 2, 5, GetColor(255, 255, 255), 1); //Blockの上底
	//DrawCircle(b_x, b_y + b_h / 2, 5, GetColor(255, 255, 255), 1); //Blockの底辺
}

double Block::HitBoxPlayer() {
	 //x,yは中心座標とする
	 double sx1 = player.p_posX - (player.p_w * 0.5f);
	 double sy1 = player.p_posY - (player.p_h * 0.5f);
	 double sx2 = player.p_posX + (player.p_w * 0.5f);
	 double sy2 = player.p_posY + (player.p_h * 0.5f);

	 double dx1 = b_x - (b_w * 0.5f);
	 double dy1 = b_y - (b_h * 0.5f);
	 double dx2 = b_x + (b_w * 0.5f);
	 double dy2 = b_y + (b_h * 0.5f);

	 //短形が重なっていれば当たり
	 if ((sx1 < dx2 && dx1 < sx2) && (sy1 < dy2 && dy1 < sy2)) {
		 return TRUE;
	 }
	 return FALSE;
}
