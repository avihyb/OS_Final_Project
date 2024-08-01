#ifndef ACTIVEOBJECT_HPP
#define ACTIVEOBJECT_HPP

#include <functional>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

class ActiveObject {
public:
    ActiveObject();
    ~ActiveObject();
    void enqueueTask(std::function<void()> task);

private:
    std::queue<std::function<void()>> tasks;
    std::mutex mtx;
    std::condition_variable cv;
    std::thread worker;
    bool running;
};

#endif // ACTIVEOBJECT_HPP
