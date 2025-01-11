#include "ThreadPool.hpp"
#include <iostream>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
using std::cout;
using std::endl;
using std::string;

void func(int i)
{
    for (int j = 0; j < 10; j++)
        std::cout << i << endl;
}
int main(int argc, char **argv)
{
    ThreadPool pool{4};
    int i = 1;
    pool.Enqueue(func, 1);
    pool.Enqueue(func, 2);
    pool.Enqueue(func, 3);
    pool.Enqueue(func, 4);
    pool.Wait();
    pool.Enqueue(func, 5);

    // pool.Enqueue(func, 5);

    return 0;
}
