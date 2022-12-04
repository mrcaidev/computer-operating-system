#include "thread_hdr.h"

int interval = 0;

int first_time_ticks = 0;

int second_time_ticks = 0;

void add_ready_thread(thread *ready_thread)
{
    ready_thread->clock_times = 0;
    ready_thread->max_clock_times = first_time_ticks;
    first_ready_queue.push_back(ready_thread);
}

void current_thread_finished()
{
    if (!first_ready_queue.empty())
    {
        current_thread = first_ready_queue.front();
        first_ready_queue.pop_front();
    }
    else if (!second_ready_queue.empty())
    {
        current_thread = second_ready_queue.front();
        second_ready_queue.pop_front();
    }
    else
    {
        current_thread = &idle_thread;
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
        current_thread_finished();
        return;
    }

    if ((current_thread->max_clock_times == second_time_ticks) && (!first_ready_queue.empty()))
    {
        current_thread->clock_times = 0;
        second_ready_queue.push_back(current_thread);
        current_thread = first_ready_queue.front();
        first_ready_queue.pop_front();
        return;
    }

    if (current_thread->clock_times + interval < current_thread->max_clock_times)
    {
        current_thread->clock_times += interval;
    }
    else
    {
        current_thread->clock_times = 0;
        current_thread->max_clock_times = second_time_ticks;
        second_ready_queue.push_back(current_thread);
        current_thread_finished();
    }
}

void set_first_time_ticks(unsigned int ticks)
{
    first_time_ticks = ticks;
}

void set_second_time_ticks(unsigned int ticks)
{
    second_time_ticks = ticks;
}

void set_time_interval(unsigned int interval)
{
    ::interval = interval;
}
