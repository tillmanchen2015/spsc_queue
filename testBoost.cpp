#include "stdafx.h"
#include <boost/thread/thread.hpp>
#include <boost/lockfree/spsc_queue.hpp>
#include <iostream>

#include <boost/atomic.hpp>
#include <thread>
#include "tinyformat.h"
#include <mutex>

int producer_count = 0;
boost::atomic_int consumer_count(0);

boost::lockfree::spsc_queue<int, boost::lockfree::capacity<3> > spsc_queue;
std::mutex mut;

const int iterations = 10000000;

void producer(void)
{
	for (int i = 0; i != iterations; ++i) {
		std::this_thread::sleep_for(std::chrono::seconds(1));
		while (!spsc_queue.push(i))
			;
		std::lock_guard<std::mutex> lk(mut);

		tfm::printfln("-->%s", i);
	}
}

boost::atomic<bool> done(false);

void consumer(void)
{
	int value;
	while (true)
	{
		while (spsc_queue.pop(value))
		{
			std::this_thread::sleep_for(std::chrono::seconds(6));

			std::lock_guard<std::mutex> lk(mut);
			tfm::printfln("%s<--", value);
		};
	}
}

int main4(int argc, char* argv[])
{
	using namespace std;
	cout << "boost::lockfree::queue is ";
	if (!spsc_queue.is_lock_free())
		cout << "not ";
	cout << "lockfree" << endl;

	boost::thread producer_thread(producer);

	boost::thread consumer_thread(consumer);

	producer_thread.join();
	done = true;
	consumer_thread.join();

	cout << "produced " << producer_count << " objects." << endl;
	cout << "consumed " << consumer_count << " objects." << endl;
}