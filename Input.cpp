#include "Game.h"
#include "DxLib.h"
#include "Input.h"

int	Input::iOldKey = {};				// �O��̓��̓L�[
int	Input::iNowKey = {};				// ����̓��̓L�[
int	Input::iKeyFlg = {};				// ���̓L�[���

/***********************************************
 * �L�[���͎擾�֐�
 ***********************************************/
void Input::InputKey(void) {
    // ���̓L�[�擾
    iOldKey = iNowKey;
    iNowKey = GetJoypadInputState(DX_INPUT_KEY_PAD1);
    iKeyFlg = iNowKey & ~iOldKey;
}
