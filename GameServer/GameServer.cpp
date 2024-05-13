#include "pch.h"
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <mutex>
#include <Windows.h>

mutex m;
queue<int32> q;
HANDLE/*이벤트가 여러개 일텐데 번호를 부여(식별자)*/ handle;
void Producer()
{
	while (true)
	{
		{
			unique_lock<mutex> lock(m);
			q.push(100);
		}

		// 커널 오브젝트를 Signal 상태로 바꿔줘라는 뜻.
		::SetEvent(handle);

		this_thread::sleep_for(100ms);
	}
}

void Consumer()
{
	while (true)
	{
		/* bManualReset가 FALSE이면 */
		// Signal || Non-Signal을 확인해서 Sleep여부를 판단.
		::WaitForSingleObject(handle, INFINITE);
		// 만약 Signal이면 Non-Signal 상태로 바뀜
		
		/* bManualReset가 TRUE이면 */
		// ::ResetEvent(handle); // 강제로 Signal 끄기

		unique_lock<mutex> lock(m);
		if(!q.empty())
		{
			int32 data = q.front();
			q.pop();
			cout << data << endl;
		}
	}
}

int main()
{
	// 커널 오브젝트
	// Usage Count
	// Signal(파란불) / Non-Signal(빨간불) << bool
	// Auto / Manual << bool
	handle = ::CreateEvent(NULL/*보안속성*/, FALSE/*bManualReset*/, FALSE/*binitialState*/, NULL);

	thread t1(Producer);
	thread t2(Consumer);

	t1.join();
	t2.join();

	::CloseHandle(handle);
}