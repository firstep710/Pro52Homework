#include <stdio.h>
#include "GameLoopExample.h"
#include "RenderSystem.h"

namespace global
{
	namespace time
	{
		

		void InitTime() 
		{
			previousTime = currentTime = GetTickCount64();
		}

		void UpdateTime()
		{
			previousTime = currentTime;
			currentTime = GetTickCount64();
			deltaTime = currentTime - previousTime;
		}

		ULONG GetDeltaTime()
		{
			return deltaTime;
		}
	}
}
void StartGame()
{
	global::time::previousTime=1;
}

bool IsGameRun()
{
	return true;
}

void ProcessInput()
{

}

void FixedUpdate()
{
	static ULONGLONG elapsedTime; //정적 변수 알아서 초기화 해줌
	//정적 변수는 함수 메모리에 속하지 않기에 함수가 끝나도 메모리에 존재

	elapsedTime += global::time::GetDeltaTime();
	
	while (elapsedTime >= 20)
	{
		global::time::fixedUpdateCount += 1;
		elapsedTime -= 20;
	}
}

void Update()
{
	global::time::updateCount += 1;
}

void PrintCountsPerSecond()
{
	static ULONGLONG elapsedTime;
	elapsedTime += global::time::GetDeltaTime();

	if (elapsedTime >= 1000)
	{
		printf("updateCount : %d \t fixedUpdateCount : %d\n", global::time::updateCount, global::time::fixedUpdateCount);

		global::time::updateUpdateCountPerSecond = global::time::updateCount;
		global::time::updateFixedUpdateCountPerSecond = global::time::fixedUpdateCount;

		elapsedTime = 0;
		global::time::updateCount = 0;
		global::time::fixedUpdateCount = 0;
	}
}
