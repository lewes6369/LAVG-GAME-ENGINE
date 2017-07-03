#include "GameManager.h"

const double GameManager::mfixedDeltaTime = 1/30.f;

GameManager::GameManager()
{
	mTimeList = TimerList();
	mCurTime = 0;
	mDeltaTime = 0;
	mLastTime = 0;
}

int GameManager::AddTimer(float ttl, bool bOnce, int cb, int cbparam)
{
	return mTimeList.AddTimer(ttl, bOnce, cb, cbparam);
}

void GameManager::RemoveTimer(int id)
{
	mTimeList.RemoveTimer(id);
}

void GameManager::ResetTimer(int id)
{
	mTimeList.ResetTimer(id);
}

void GameManager::SetTimer(int id, float fNextPlayTime)
{
	mTimeList.SetTimer(id, fNextPlayTime);
}

#include <iostream>
void GameManager::Update()
{
	mCurTime = glfwGetTime();
	mDeltaTime += mCurTime - mLastTime;
	mLastTime = mCurTime;

	while (mDeltaTime >= mfixedDeltaTime) {
		FixedUpdate();
		mDeltaTime -= mfixedDeltaTime;
	}

}

void GameManager::FixedUpdate()
{
	if (mTimeList.total_count > 0)
		mTimeList.Tick(mDeltaTime);
}
