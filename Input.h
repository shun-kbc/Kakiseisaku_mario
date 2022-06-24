#pragma once

class Input {

public:

	static int	iOldKey;	// 前回の入力キー
	static int	iNowKey;	// 今回の入力キー
	static int	iKeyFlg;	// 入力キー情報

	//キー入力取得関数
	void InputKey(void);

};



