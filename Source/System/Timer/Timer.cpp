#include "Timer.h"

void Timer::Init()
{
    startTime_ = std::chrono::high_resolution_clock::now();
}

float Timer::GetElapsedTime()
{
    std::chrono::steady_clock::time_point now = std::chrono::high_resolution_clock::now();
    float elapseTime = std::chrono::duration<float>(now - startTime_).count();
    return elapseTime;
}
