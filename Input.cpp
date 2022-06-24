#include "Game.h"
#include "DxLib.h"
#include "Input.h"

int	Input::iOldKey = {};				// 前回の入力キー
int	Input::iNowKey = {};				// 今回の入力キー
int	Input::iKeyFlg = {};				// 入力キー情報

/***********************************************
 * キー入力取得関数
 ***********************************************/
void Input::InputKey(void) {
    // 入力キー取得
    iOldKey = iNowKey;
    iNowKey = GetJoypadInputState(DX_INPUT_KEY_PAD1);
    iKeyFlg = iNowKey & ~iOldKey;
}
