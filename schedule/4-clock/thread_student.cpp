#include "thread_hdr.h"

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
