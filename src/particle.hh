#pragma once

#include <vector>
#include <raylib-cpp.hpp>

#include "typedefs.hh"
#include "sprite.hh"

class ParticleSystem {
private:
	struct Particle {
		vec2 position, direction;
		float age;
	};

	std::vector<Particle> particles;

	void start(Particle& particle); // Initializes particle
	void check_if_done(); // Checks if the particle system has finished

public:
	vec2 position; // Emitter position
	vec2 direction; // Direction particles are emmited
	vec2 spread; // Randomization of direction

	float length; // Time from particle birth to death
	float gravity_scale;
	int count;
	bool loop;
	bool collision;
	bool done = false;
	Sprite* sprite = nullptr;

	float size_start, size_end;
	float speed_start, speed_end;
	rgba color_start, color_end;

	void start();
	void update();
	void draw();
};
