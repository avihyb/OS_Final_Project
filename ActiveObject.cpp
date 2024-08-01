#include "ActiveObject.hpp"

ActiveObject::ActiveObject() : running(true), worker([this] {
    while (running) {
        std::function<void()> task;
        {
            std::unique_lock<std::mutex> lock(mtx);
            cv.wait(lock, [this] { return !tasks.empty() || !running; });
            if (!running && tasks.empty()) return;
            task = std::move(tasks.front());
            tasks.pop();
        }
        task();
    }
}) {}

ActiveObject::~ActiveObject() {
    {
        std::unique_lock<std::mutex> lock(mtx);
        running = false;
    }
    cv.notify_all();
    worker.join();
}

void ActiveObject::enqueueTask(std::function<void()> task) {
    {
        std::unique_lock<std::mutex> lock(mtx);
        tasks.push(std::move(task));
    }
    cv.notify_one();
}
