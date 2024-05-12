#include "pch.h"
#include "CorePch.h"

#include <thread>

void HelloThread()
{
	cout << "Hello Thread" << endl;
}

void HelloThread_2(int32 num)
{
	cout << num << endl;
}

int main()
{
	vector<thread> v;

	for (int32 i = 0; i < 10; i++)
	{
		// HelloThread_2 함수에 10 인자를 넣겠다.
		v.push_back(thread(HelloThread_2, i)); 
	}

	for (int32 i = 0; i < 10; i++)
	{
		if (v[i].joinable())
			v[i].join();
	}

	cout << "Hello Main" << endl;
}