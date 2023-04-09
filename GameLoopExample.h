#pragma once
#include <Windows.h>

namespace global
{
	namespace time
	{
		ULONGLONG previousTime;
		ULONGLONG currentTime;
		ULONGLONG deltaTime;

		int updateCount;
		int fixedUpdateCount;

		int updateUpdateCountPerSecond;
		int updateFixedUpdateCountPerSecond;

		void InitTime();
		void UpdateTime();
		ULONG GetDeltaTime();
	}
}
void StartGame();
bool IsGameRun();

void ProcessInput();

void FixedUpdate();
void Update();
void PrintCountsPerSecond();