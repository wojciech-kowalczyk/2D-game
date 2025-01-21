#include "Clock.h"

Clock::Clock() : startTimePoint(std::chrono::system_clock::now()) {}

void Clock::restart() { startTimePoint = std::chrono::system_clock::now(); }
