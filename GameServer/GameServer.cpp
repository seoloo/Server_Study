#include "pch.h"
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <mutex>
#include <future>

int64 Calculate()
{
	int64 sum = 0;

	for (int32 i = 0; i < 100'000; i++)
		sum += i;
	
	return sum;
}

void PromiseWorker(promise<string>&& promise)
{
	promise.set_value("Secret Message");
}

void TaskWorker(packaged_task<int64(void)>&& task)
{
	task();
}

int main()
{
	// 동기(synchronos) 실행
	/*int64 sum = Calculate();
	cout << sum << endl;*/

	// std::future
	{
		// 1) deferred -> lazy evaluation  지연해서 실행하세요
		// future<int64> future = async(launch::deferred, Calculate); // Calculate를 나중에 호출한다는 의미.
		// -> 실행 시점을 늦춘다.
		
		// 2) async -> 별도의 쓰레드를 만들어서 실행하세요
		future<int64> future = async(launch::async, Calculate); // Calculate를 나중에 호출한다는 의미.
		// -> 언젠가 미래에 결과물을 줄것이다! get이 있다면

		// 3) deferred || async -> 둘 중 알아서 골라주세요

		// TODO

		int64 sum = future.get(); // 결과물이 이제서야 필요하다!

	}

	// std::promise
	{
		// 미래(future)에 결과물을 반환해줄 것이라 약속(promise) 해줘 (계약서)
		promise<string> promise;
		future<string> future = promise.get_future();

		thread t(PromiseWorker, move(promise));

		string message = future.get();
		cout << message << endl;

		t.join();
	}

	// std::package_task
	{
		//함수와 타입을 맞추어야함
		packaged_task<int64(void)> task(Calculate);
		future<int64> future = task.get_future();

		thread t(TaskWorker, move(task));

		int64 sum = future.get();

		cout << sum << endl;

		t.join();
	}
}