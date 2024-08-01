#include "Pipeline.hpp"

void Pipeline::addStep(std::function<void()> step) {
    steps.push_back(std::move(step));
}

void Pipeline::execute() const {
    for (const auto& step : steps) {
        step();
    }
}
