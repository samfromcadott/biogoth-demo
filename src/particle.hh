#pragma once

#include <vector>
#include <raylib-cpp.hpp>

class ParticleSystem {
private:
	struct Particle {
		raylib::Vector2 position, velocity;
		float age;
	};

	std::vector<Particle> particles;

	void start(Particle& particle); // Initializes particle

public:
	raylib::Vector2 position; // Emitter position
	float length; // Time from particle birth to death
	float gravity_scale;
	int count;
	bool loop;

	float size_start, size_end;
	float speed_start, speed_end;
	raylib::Color color_start, color_end;

	void start();
	void update();
	void draw();
};
