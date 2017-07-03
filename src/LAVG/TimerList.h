#pragma once
#include <list>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
// TimerList
// author by Liu Hao
// 2017/06/29

/// <summary>
/// ¼ÆÊ±Æ÷
/// </summary>
using namespace std;

struct Timer
{
	int id;
	float ttl;
	float total_played;
	float end_time;
	int callback;
	int cbparam;
	bool bOnce;
};

class TimerList
{
	public:
		TimerList();

		int AddTimer(float ttl, bool bOnce, int cb, int cbparam);
		int DelayTimer(int id, float ttl, float maxDurationToRemove = -1/*<0 means not care*/);
		void RemoveTimer(int id);
		void SetTimer(int id, float fNextPlayTime);
		void ResetTimer(int id, float ttl_new = -1/*<0 means not change*/);

		void Tick(double deltaTime);
		void Clear();
	public:
		static int total_count;
	private:
		static int _uniqueid;
		list<Timer> m_List;
		list<Timer> m_TempList;
		list<int> m_TempDelList;
		bool m_bTick;
};
