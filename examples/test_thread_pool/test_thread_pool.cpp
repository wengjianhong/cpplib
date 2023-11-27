#include <stdio.h>
#include "../src/thread_pool.hpp"

int fun(int taskid, int n)
{
    for (size_t i = 0; i < n; i++)
    {
        usleep(1);
    }
    printf("task %d end\n", taskid);
    return taskid * 100000 + n;
}

int main()
{
    ThreadPool pool(5);

    std::future<int> ret[13];
    for (int i = 0; i < 13; i++)
    {
        ret[i] = pool.enqueue(fun, i, 3);
    }

    printf("run...\n");
    pool.run();
    usleep(1);

    printf("stop...\n");
    pool.stop();
    usleep(5);

    printf("run...\n");
    pool.run();

    for (int i = 0; i < 13; i++)
    {
        printf("%d\n", ret[i].get());
    }
}