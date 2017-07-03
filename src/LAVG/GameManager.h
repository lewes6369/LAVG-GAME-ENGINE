#pragma once
#include "TimerList.h"
#define FPS 60

class  GameManager
{
public:
	static GameManager * GetInstance()
	{
		static GameManager * instance;
		if (!instance)
			instance = new GameManager();

		return instance;
	};

	int AddTimer(float ttl, bool bOnce, int cb, int cbparam);
	void RemoveTimer(int id);
	void ResetTimer(int id);
	void SetTimer(int id, float fNextPlayTime);
	void Update();
	void FixedUpdate();

	double GetCurTime() { return mCurTime; }
	double GetDeltaTime() { return mDeltaTime; }
private:
	GameManager() ;
	GameManager(const GameManager&) ;

	TimerList mTimeList;
	double mCurTime;
	double mDeltaTime;
	double mLastTime;
	static const double mfixedDeltaTime;
};