#pragma once

class Timer {
private:
	float time; // Time remaining
	void(* function)(); // Function called when time runs out
	bool active = false;

public:
	void update(); // Counts down the timer
	float time_left();
	bool is_active();

	Timer( float time, void(* function)() );
	Timer(){}
	virtual ~Timer(){}
};
