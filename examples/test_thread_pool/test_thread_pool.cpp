#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include "thread_pool.hpp"

int fun(int taskid, int n)
{
    for (int i = 0; i < n; i++)
    {
        usleep(1000);
    }
    printf("task %d end\n", taskid);
    return taskid * 100000 + n;
}

int main()
{
    ThreadPool pool(10);

    std::future<int> ret[13];
    for (int i = 0; i < 13; i++)
    {
        ret[i] = pool.enqueue(fun, i, i % 3);
    }

    printf("run...\n");
    pool.run();
    sleep(1);

    printf("stop...\n");
    pool.stop();
    sleep(5);

    printf("run...\n");
    pool.run();

    for (int i = 0; i < 13; i++)
    {
        printf("%d\n", ret[i].get());
    }
}