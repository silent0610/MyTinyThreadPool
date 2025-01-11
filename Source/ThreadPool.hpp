#pragma once
#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <vector>

class ThreadPool
{
public:
    ThreadPool(int t_numThreads) : m_isStop(false)
    {
        for (int i = 0; i < t_numThreads; i++)
        {
            m_threads.emplace_back([this] { // 为每一个线程提供一个lamda函数作为他们的工作
                while (1)
                {
                    std::unique_lock<std::mutex> lock(m_mtx); // 访问任务队列时上锁
                    // 等待, 直到队列非空或者要求停止
                    m_condition.wait(lock, [this] { return !m_tasks.empty() || m_isStop; });
                    // 如果要求停止且队列为空,中止当前线程，否则优先完成剩余工作
                    if (m_isStop && m_tasks.empty())
                    {
                        return;
                    }
                    std::function<void()> task(std::move(m_tasks.front()));
                    m_tasks.pop();
                    lock.unlock();
                    task();
                }
            });
        }
    }
    ~ThreadPool()
    {
        {
            std::unique_lock<std::mutex> lock(m_mtx);
            m_isStop = true;
        }
        m_condition.notify_all(); // 通知所有线程wait函数检查是否结束等待
        for (auto &t : m_threads)
        {
            t.join();
        }
    }
    template <class F, class... Args> void Enqueue(F &&f, Args &&...args) // 引用与转发
    {
        std::function<void()> task =
            std::bind(std::forward<F>(f), std::forward<Args>((args)...)); // 转发

        {
            std::unique_lock<std::mutex> lock(m_mtx);
            m_tasks.emplace(std::move(task));
        }
        m_condition.notify_one();
    }

private:
    std::vector<std::thread> m_threads;
    std::queue<std::function<void()>> m_tasks;

    std::mutex m_mtx;
    std::condition_variable m_condition;

    bool m_isStop;
};