#include "pch.h"
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <mutex>
#include <future>
#include <Windows.h>

// [][][][][][]			[][][][][][]			[][][][][][]			[][][][][][]

int32 buffer[10000][10000];

int main()
{
	// 값이 차이가 나는 이유는 캐시가 시간 지역성, 공간 지역성 때문이다.
	// 캐시 hit
	// 첫 번째는 시간 지역성, 공간 지역성이 적용되지만
	// 두 번째는 j부터 데이터가 입력이 되기 때문에 띄엄띄엄 저장이 되어
	// RAM까지 갖다가 오기 때문에 시간이 첫 번째보다 오래걸린다

	memset(buffer, 0, sizeof(buffer));

	{
		uint64 start = GetTickCount64();

		int64 sum = 0;

		for (int32 i = 0; i < 10'000; i++)
		{
			for (int32 j = 0; j < 10'000; j++)
			{
				sum += buffer[i][j];
			}
		}
		uint64 end = GetTickCount64();

		cout << "Elapsed Tick" << (end - start) << endl;
	}

	{
		uint64 start = GetTickCount64();

		int64 sum = 0;

		for (int32 i = 0; i < 10'000; i++)
		{
			for (int32 j = 0; j < 10'000; j++)
			{
				sum += buffer[j][i];
			}
		}
		uint64 end = GetTickCount64();

		cout << "Elapsed Tick" << (end - start) << endl;
	}
}