#include <iostream>
#include <entt/entt.hpp>

#include "globals.hh"
#include "particle.hh"
#include "util.hh"
#include "systems.hh"

void ParticleSystem::start() {
	particles.resize(count);

	for (auto& particle : particles) {
		particle.position = position;
		particle.velocity = raylib::Vector2( random_spread(), random_spread() ).Normalize() * speed_start;
	}
}

void ParticleSystem::update() {
	for (auto& particle : particles) {
		particle.position += particle.velocity * GetFrameTime();

		// float speed =
	}
}

void ParticleSystem::draw() {
	DrawCircle(position.x, position.y, 10, BLUE);
	for (auto& particle : particles) {
		DrawCircle(particle.position.x, particle.position.y, 5, RED);
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
