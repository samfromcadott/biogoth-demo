#include <iostream>
#include <cmath>
#include <entt/entt.hpp>

#include "globals.hh"
#include "particle.hh"
#include "util.hh"
#include "systems.hh"

void ParticleSystem::start(Particle& particle) {
	particle.position = position;
	particle.direction = ( direction + raylib::Vector2(random_spread(), random_spread()) * spread).Normalize();
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
		// Tilemap collision
		if ( collision && tilemap( tilemap.world_to_tile(particle.position) ) != 0 )
			particle.age = length + 1;

		// Restart particles
		if (loop && particle.age > length) start(particle);

		// Update velocity
		float speed = ease(particle.age/length, speed_start, speed_end);
		raylib::Vector2 velocity = particle.direction * speed;

		// Apply gravity
		velocity.y += G * particle.age * gravity_scale * GetFrameTime();

		// Update position and age
		particle.position += velocity * GetFrameTime();
		particle.direction = velocity.Normalize();
		particle.age += GetFrameTime();

	}
}

void ParticleSystem::draw() {
	for (auto& particle : particles) {
		// Update size and color
		float size = ease(particle.age/length, size_start, size_end);
		float rotation = atan2(particle.direction.y, particle.direction.x) * (180/PI);
		Color color = {
			(unsigned char)ease(particle.age/length, color_start.r, color_end.r),
			(unsigned char)ease(particle.age/length, color_start.g, color_end.g),
			(unsigned char)ease(particle.age/length, color_start.b, color_end.b),
			(unsigned char)ease(particle.age/length, color_start.a, color_end.a)
		};

		if (sprite) sprite->render(particle.position, IDLE, particle.age, +1, rotation, size, color); // Draw sprite
		else DrawCircle(particle.position.x, particle.position.y, size, color);
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
