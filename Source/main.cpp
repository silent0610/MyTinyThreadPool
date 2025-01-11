#include "ThreadPool.hpp"
#include <iostream>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
using std::cout;
using std::endl;
using std::string;
int a = 0;
std::mutex mtx;
void func()
{
    for (int i = 0; i < 10000; i += 1)
    {
        mtx.lock();
        a += 1;
        mtx.unlock();
    }
}

class Log
{
public:
    Log() {};
    Log(const Log &log) = delete;
    Log &operator=(const Log &log) = delete;
    static Log &GetInstance()
    {
        static Log log;
        return log;
    }
    void PrintLog(string t_msg)
    {
        std::cout << __TIME__ << ' ' << t_msg << std::endl;
    }
};

int main(int argc, char **argv)
{
    Log::GetInstance().PrintLog("error");
    return 0;
}
