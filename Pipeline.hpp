#ifndef PIPELINE_HPP
#define PIPELINE_HPP

#include <vector>
#include <functional>

class Pipeline {
public:
    void addStep(std::function<void()> step);
    void execute() const;

private:
    std::vector<std::function<void()>> steps;
};

#endif // PIPELINE_HPP
