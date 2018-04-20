#include "stdafx.h"
#include "spsc_quque.h"
#include <mutex>
spsc_queue<int, 3> q;
std::mutex mu;
static void producer()
{
	int i = 0;
	while (true)
	{
		i++;
		std::this_thread::sleep_for(std::chrono::seconds(6));
		q.push(i);
		std::lock_guard<std::mutex> lk(mu);
		printf("%d\n",i);
	}
}

static void consumer()
{
	while (true)
	{
		std::this_thread::sleep_for(std::chrono::seconds(1));
		int v;
		q.pop(v);
		std::lock_guard<std::mutex> lk(mu);
		printf("                     %d\n", v);
	}
}


void main()
{

	std::thread th1(producer);
	std::this_thread::sleep_for(std::chrono::seconds(5));

	std::thread th2(consumer);
	th1.join();
	th2.join();
}
