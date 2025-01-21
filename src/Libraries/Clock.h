#ifndef CLOCK_H
#define CLOCK_H

#include <chrono>

class Clock {
public:
	using hours = std::ratio<3600>;
	using minutes = std::ratio<60>;
	using seconds = std::ratio<1>;
	using milliseconds = std::ratio<1, 1'000>;
	using microseconds = std::ratio<1, 1'000'000>;
	
	Clock();
	
	void restart();
	/// Get elapsed time (from creation or last restart)
	template<typename Ratio = seconds, typename ReturnType = double>
	[[nodiscard]] ReturnType getElapsedTime() const;
private:
	std::chrono::time_point<std::chrono::system_clock> startTimePoint;
};

template<typename Ratio, typename ReturnType>
ReturnType Clock::getElapsedTime() const {
	return std::chrono::duration_cast<std::chrono::duration<ReturnType, Ratio>>(std::chrono::system_clock::now() - startTimePoint).count();
}

#endif // CLOCK_H
