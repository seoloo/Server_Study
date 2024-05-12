#include "pch.h"
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <mutex>

vector<int32> v;

// Mutual Exclusive : 상호배타적
mutex m; // 자물쇠 개념 

// RAII (Resource Acquisition Is Initialization)
template<typename T>
class LockGuard
{
public:
	LockGuard(T& m)
	{
		_mutex = &m;
		_mutex->lock();
	}
	~LockGuard()
	{
		_mutex->unlock();
	}
private:
	T* _mutex;
};

void Push()
{
	for (int32 i = 0; i < 10'000; i++)
	{
		// 자물쇠 잠그기
		//LockGuard<mutex> lockGuard(m); // lock_guard<mutex> lockGuard(m);
		/*unique_lock<mutex> uniqueLock(m, defer_lock);

		uniqueLock.lock();*/

		// m.lock(); 
		// m.lock(); -> 재귀적으로 호출할 수 없음

		v.push_back(i);

		if (i == 5000)
			break;

		// 자물쇠 풀기
		//m.unlock(); 
		//m.unlock(); -> 재귀적으로 호출할 수 없음
	}
}

int main()
{
	v.reserve(20'000);
	thread t1(Push);
	thread t2(Push);

	t1.join();
	t2.join();

	cout << v.size() << endl;
}