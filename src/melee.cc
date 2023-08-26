#include <iostream>
#include <raylib-cpp.hpp>

#include "audio.hh"
#include "globals.hh"
#include "components.hh"
#include "weapon.hh"
#include "util.hh"

Melee::Melee(entt::entity owner, toml::value data) {
	this->owner = owner;
	this->damage = toml::find<unsigned int>(data, "damage");
	this->range = toml::find<float>(data, "range");
	this->rate = toml::find<float>(data, "rate");
	this->push = toml::find<float>(data, "push");
}

void Melee::fire() {
	auto& position = *registry.try_get<Position>(owner);
	auto& collider = *registry.try_get<Collider>(owner);
	auto& facing = *registry.try_get<Facing>(owner);

	RayCast ray;
	ray.start = position.value + raylib::Vector2( (collider.width/2+0.001)*facing.direction, -collider.height/2 );
	ray.end = ray.start + raylib::Vector2(range) * facing.direction;

	play_sound("sword_swing", 0.6 + random_spread() * 0.1, 1.0 + random_spread() * 0.2);

	active = true;
	timer = rate;

	// Look for targets
	auto target_view = registry.view<const Position, const Collider, Velocity, Health>();
	for ( auto [target, target_position, target_collider, target_velocity, target_health] : target_view.each() ) {
		if (!target_collider.enabled) continue;
		if ( !ray.intersect( target_collider.get_rectangle(target_position.value) ) ) continue;
		if (target == owner) continue; // Don't harm self
		if (target_health.now <= 0) continue; // Skip dead enemies

		std::cout << "SLASH!" << '\n';
		target_health.now -= damage;

		// Push the enemy back
		target_velocity.value -= (ray.start - ray.end).Normalize() * push;

		// Play the sound effect
		play_sound("sword_hit", 0.7 + random_spread() * 0.1, 1.0 + random_spread() * 0.1);
	}
}

void Melee::update() {
	timer -= GetFrameTime();
	if (timer <= 0.0 && active) end();
}

void Melee::end() {
	active = false;
}
