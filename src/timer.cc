#include <raylib/raylib-cpp.hpp>

#include "timer.hh"

Timer::Timer( float time, void(& function)() ) {
	this->time = time;
	this->function = &function;
	active = true;
}

void Timer::update() {
	if (!active) return;

	time -= GetFrameTime(); // Count down
	if (time > 0.0) return;

	function(); // Call function when time runs out
	active = false; // Deactivate timer
}

float Timer::time_left() {
	return time;
}

bool Timer::is_active() {
	return active;
}
