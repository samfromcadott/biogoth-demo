#include <iostream>
#include <entt/entt.hpp>

#include "globals.hh"
#include "particle.hh"
#include "util.hh"
#include "systems.hh"

void ParticleSystem::start(Particle& particle) {
	particle.position = position;
	particle.velocity = raylib::Vector2( random_spread(), random_spread() ).Normalize() * speed_start;
	particle.age = ( float( rand() ) / float(RAND_MAX) ) * length; // Randomize age
}

void ParticleSystem::start() {
	particles.resize(count);

	for (auto& particle : particles) {
		start(particle);
	}
}

void ParticleSystem::update() {
	for (auto& particle : particles) {
		if (loop && particle.age > length) start(particle);

		// Update position and age
		particle.position += particle.velocity * GetFrameTime();
		particle.age += GetFrameTime();

		// Update velocity
		float speed = ease(particle.age/length, speed_start, speed_end);
		particle.velocity = particle.velocity.Normalize() * speed;
	}
}

void ParticleSystem::draw() {
	for (auto& particle : particles) {
		// Update size and color
		float size = ease(particle.age/length, size_start, size_end);
		Color color = {
			(unsigned char)ease(particle.age/length, color_start.r, color_end.r),
			(unsigned char)ease(particle.age/length, color_start.g, color_end.g),
			(unsigned char)ease(particle.age/length, color_start.b, color_end.b),
			(unsigned char)ease(particle.age/length, color_start.a, color_end.a)
		};

		DrawCircle(particle.position.x, particle.position.y, size, color);
	}
}

void particle_update() {
	for ( auto [entity, particle_system] : registry.view<ParticleSystem>().each() ) {
		particle_system.update();
	}
}

void render_particles() {
	for ( auto [entity, particle_system] : registry.view<ParticleSystem>().each() ) {
		particle_system.draw();
	}
}
