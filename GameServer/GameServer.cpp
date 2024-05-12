#include "pch.h"
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <mutex>
#include "AccountManager.h"
#include "UserManager.h"

void Func1()
{
	for (int32 i = 0; i < 100; i++)
	{
		UserManager::Instance()->ProcessSave();
	}
}

void Func2()
{
	for (int32 i = 0; i < 100; i++)
	{
		AccountManager::Instance()->ProcessLogin();
	}
}

int main()
{
	thread t1(Func1);
	thread t2(Func2);

	t1.join();
	t2.join();

	cout << "Join Done" << endl;

	// 참고
	mutex m1;
	mutex m2;

	lock(m1, m2); // 내부적으로 일관적인 순서를 정해준다.

	// adopt_lock : 이미 lock된 상태이니까, 나중에 소멸될 때 풀어주기만 해라.
	lock_guard<mutex> g1(m1, adopt_lock);
	lock_guard<mutex> g2(m2, adopt_lock);
}