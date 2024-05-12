#include "pch.h"
#include "CorePch.h"
#include <thread>
#include <atomic>

// atomic : All or Nothing
// -> 한번에 모두 이루어지거나 한번에 모두 이루어지지 않거나

// DB
// 예시) 집행검 교환
// A라는 유저 인벤에서 집행검 뺴고
// B라는 유저 인벤에 집행검 추가

atomic<int32> sum = 0;

void Add()
{
	for (int32 i = 0; i < 100'0000; i++)
	{
		sum.fetch_add(1);
	}
}

void Sub()
{
	for (int32 i = 0; i < 100'0000; i++)
	{
		sum.fetch_sub(1);
	}
}

int main()
{
	Add();
	Sub();

	cout << sum << endl;

	thread t1(Add);
	thread t2(Sub);

	t1.join();
	t2.join();

	cout << sum << endl;
}