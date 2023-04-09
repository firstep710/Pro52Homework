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
	static ULONGLONG elapsedTime; //���� ���� �˾Ƽ� �ʱ�ȭ ����
	//���� ������ �Լ� �޸𸮿� ������ �ʱ⿡ �Լ��� ������ �޸𸮿� ����

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
