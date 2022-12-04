#include "thread_hdr.h"
#include <iostream>

thread idle_thread = {0};

thread_queue ready_queue;

thread_queue blocked_queue;

thread *current_thread = &idle_thread;

int ticks = 0;

int interval = 0;

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

void current_thread_blocked()
{
	if (current_thread == &idle_thread)
	{
		return;
	}

	blocked_queue.push_back(current_thread);
	current_thread_finished();
}

void notify()
{
	if (blocked_queue.empty())
	{
		return;
	}

	add_ready_thread(blocked_queue.front());
	blocked_queue.pop_front();
}

void notify_all()
{
	while (!blocked_queue.empty())
	{
		notify();
	}
}

void on_clock()
{
	if (current_thread == &idle_thread)
	{
		schedule();
		return;
	}

	if (current_thread->clock_times + interval < ticks)
	{
		current_thread->clock_times += interval;
	}
	else
	{
		current_thread->clock_times = 0;
		schedule();
	}
}

void set_time_ticks(unsigned int ticks)
{
	::ticks = ticks;
}

void set_time_interval(unsigned int interval)
{
	::interval = interval;
}

bool test_multi_thread()
{
	ready_queue.clear();
	set_time_ticks(40);
	set_time_interval(20);
	current_thread = &idle_thread;

	thread thread1 = {1};
	thread thread2 = {2};
	thread thread3 = {3};
	add_ready_thread(&thread1);
	add_ready_thread(&thread2);
	add_ready_thread(&thread3);

	on_clock();
	on_clock();
	on_clock();
	on_clock();
	if (current_thread != &thread2)
	{
		return false;
	}

	on_clock();
	on_clock();
	if (current_thread != &thread3)
	{
		return false;
	}

	return true;
}

int main()
{
	bool ret = test_multi_thread();
	std::cout << ret << std::endl;
	return 0;
}
