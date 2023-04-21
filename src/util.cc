#include <cmath>

#include "util.hh"

float sign(float n) {
	return (n > 0) - (n < 0);
}

float move_towards(float n, float t, float d) {
	float x;

	if ( std::abs(t - n) <= d ) x = t;
	else  x = n + sign(t - n) * d;

	return x;
}
