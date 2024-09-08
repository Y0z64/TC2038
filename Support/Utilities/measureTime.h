#ifndef EXECUTION_TIMER_H
#define EXECUTION_TIMER_H

#include <chrono>
#include <utility>

class ExecutionTimer {
public:
    template <typename Func, typename... Args>
    static double measureExecutionTime(Func func, Args&&... args) {
        auto start = std::chrono::high_resolution_clock::now();
        func(std::forward<Args>(args)...);
        auto end = std::chrono::high_resolution_clock::now();
        
        std::chrono::duration<double, std::milli> duration = end - start;
        return duration.count();
    }
};

#endif
