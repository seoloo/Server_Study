#include "pch.h"
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <mutex>

class SpinLock
{
public:
	void lock()
	{
		// CAS (Compare And Swap)

		bool expected = false;
		bool desired = true;

		while (_locked.compare_exchange_strong(expected, desired) == false)
		{
			expected = false;

			// sleep_for : 언제까지 자라, 그 시간동안 재스케쥴링이 되지않고 
			// 대기하다가 시간이되면 다시 스케쥴링 대상이 되어 실행된다.
			//  this_thread::sleep_for(chrono::microseconds(100));
			this_thread::sleep_for(0ms);

			// yield : 언제든지 스케줄링 될 수 있지만 
			// 양보를 해서 커널 모드로 가서 스케쥴링을 넘기는 것
			//  this_thread::yield();
		}
	}

	void unlock()
	{
		_locked.store(false);
	}

private:
	atomic<bool> _locked = false;
};

int32 sum = 0;
mutex m;
SpinLock spinLock;

void Add()
{
	for (int32 i = 0; i < 10'0000; i++)
	{
		lock_guard<SpinLock> guard(spinLock);
		sum++;
	}
}

void Sub()
{
	for (int32 i = 0; i < 10'0000; i++)
	{
		lock_guard<SpinLock> guard(spinLock);
		sum--;
	}
}

int main()
{
	thread t1(Add);
	thread t2(Sub);

	t1.join();
	t2.join();

	cout << sum << endl;
}