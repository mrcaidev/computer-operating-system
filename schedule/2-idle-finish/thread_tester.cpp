#include "thread_hdr.h"
#include <iostream>

thread idle_thread = {0};

thread_queue ready_queue;

thread *current_thread = &idle_thread;

void add_ready_thread(thread *ready_thread)
{
	ready_queue.push_back(ready_thread);
}

void schedule()
{
	if (current_thread != &idle_thread)
	{
		add_ready_thread(current_thread);
	}

	current_thread_finished();
}

void current_thread_finished()
{
	if (ready_queue.empty())
	{
		current_thread = &idle_thread;
	}
	else
	{
		current_thread = ready_queue.front();
		ready_queue.pop_front();
	}
}

bool test_three_thread_with_finish()
{
	current_thread = &idle_thread;
	ready_queue.clear();
	// 向ready队列中加入两个线程
	thread thread1 = {1};
	thread thread2 = {2};
	add_ready_thread(&thread1);
	add_ready_thread(&thread2);
	schedule();				   // thread1 is running
	current_thread_finished(); // thread1 finished, thread2 is running
	current_thread_finished(); // thread2 finished
	if (current_thread != &idle_thread)
	{
		return false;
	}
	return true;
}

int main()
{
	bool ret = test_three_thread_with_finish();
	std::cout << ret << std::endl;
	return 0;
}
