#pragma once

#include <math.h>
#include "DxLib.h"

class Fps {
	int mStartTime;         //測定開始時刻
	static const int FPS = 60;	//設定したFPS

public:
	int mCount;             //カウンタ
	float mFps;             //fps
	static const int N = 60;//平均を取るサンプル数

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
