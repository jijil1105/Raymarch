#pragma once
#include <chrono>

class Timer
{
public:
	void Init();
	float GetElapsedTime();
private:
	std::chrono::steady_clock::time_point startTime_ = {};
};

