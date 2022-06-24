#pragma once

#include <math.h>
#include "DxLib.h"

class Fps {
	int mStartTime;         //����J�n����
	static const int FPS = 60;	//�ݒ肵��FPS

public:
	int mCount;             //�J�E���^
	float mFps;             //fps
	static const int N = 60;//���ς����T���v����

	Fps() {
		mStartTime = 0;
		mCount = 0;
		mFps = 0;
	}

	bool Update();

	void Draw();

	void Wait();
};



//class Fps {
//	bool Update();
//	void Draw();
//	void Wait();
//};
