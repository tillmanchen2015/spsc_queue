#ifndef spsc_quque_h__
#define spsc_quque_h__

#include <boost/lockfree/spsc_queue.hpp>
#include <atomic>
#include <thread>
template<typename T, int cap>
class spsc_queue
{
public:
	//milliseconds
	spsc_queue(int sleeptime = 50) {}
	~spsc_queue() {}

	void push(T& new_data)
	{
		while (!is_terminate)
		{
			if (!lockfree_queue.push(new_data))
			{
				if (!sleeptime)
				{
					std::this_thread::sleep_for(std::chrono::milliseconds(sleeptime));
				}
			}
			else
			{
				break;//push succeed
			}
		}
	}

	void pop(T& data)
	{
		while (!is_terminate)
		{
			if (!lockfree_queue.pop(data))
			{
				if (!sleeptime)
				{
					std::this_thread::sleep_for(std::chrono::milliseconds(sleeptime));
				}
			}
			else
			{
				break; // pop succeed
			}
		}
	}

	void terminate()
	{
		is_terminate = true;
		std::this_thread::sleep_for(std::chrono::milliseconds(sleeptime));
	}

private:
	int sleeptime = 50;//milliseconds
	std::atomic<bool> is_terminate = false;
	boost::lockfree::spsc_queue<T, boost::lockfree::capacity<cap> > lockfree_queue;

};








































#endif // spsc_quque_h__
