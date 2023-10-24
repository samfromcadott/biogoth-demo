#pragma once

#include <vector>
#include <raylib-cpp.hpp>

#include "sprite.hh"

class ParticleSystem {
private:
	struct Particle {
		raylib::Vector2 position, direction;
		float age;
	};

	std::vector<Particle> particles;

	void start(Particle& particle); // Initializes particle
	void check_if_done(); // Checks if the particle system has finished

public:
	raylib::Vector2 position; // Emitter position
	raylib::Vector2 direction; // Direction particles are emmited
	raylib::Vector2 spread; // Randomization of direction

	float length; // Time from particle birth to death
	float gravity_scale;
	int count;
	bool loop;
	bool collision;
	bool done = false;
	Sprite* sprite = nullptr;

	float size_start, size_end;
	float speed_start, speed_end;
	raylib::Color color_start, color_end;

	void start();
	void update();
	void draw();
};
