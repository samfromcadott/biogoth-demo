#include <iostream>
#include <raylib-cpp.hpp>

#include "audio.hh"
#include "globals.hh"
#include "components.hh"
#include "weapon.hh"
#include "util.hh"

Charge::Charge(entt::entity owner, toml::value data) {
	this->owner = owner;
	this->min_damage = toml::find<int>(data, "min_damage");
	this->max_damage = toml::find<int>(data, "max_damage");
	this->max_speed = toml::find<float>(data, "max_speed");
	this->range = toml::find<float>(data, "range");
	auto direction_data = toml::find< std::vector<float> >(data, "direction");
	this->direction = Vector2 { direction_data[0], direction_data[1] };
}

void Charge::fire() {
	active = true;
	done = false;
}

void Charge::update() {
	if (!active) return;

	auto& position = *registry.try_get<Position>(owner);
	auto& collider = *registry.try_get<Collider>(owner);
	auto& velocity = *registry.try_get<Velocity>(owner);
	auto& character = *registry.try_get<Character>(owner);

	RayCast ray;
	ray.start = position.value + raylib::Vector2( 0, -collider.height/2 );
	ray.end = ray.start + direction * range;

	float speed = velocity.value.Length();
	if (speed < 0.1 && collider.on_floor) end();

	// Look for targets
	auto target_view = registry.view<const Position, const Collider, Velocity, Health>();
	for ( auto [target, target_position, target_collider, target_velocity, target_health] : target_view.each() ) {
		if (done) return;

		if (!target_collider.enabled) continue;
		if ( !ray.intersect( target_collider.get_rectangle(target_position.value) ) ) continue;
		if (target == owner) continue; // Don't harm self
		if (target_health.now <= 0) continue; // Skip dead enemies

		// Calculate damage
		int damage = Remap(speed, 0.0, max_speed, min_damage, max_damage);
		std::cout << "CHARGE!" <<  " " << damage << '\n';
		target_health.now -= damage;

		// Play the sound effect
		play_sound("sword_hit", 0.7 + random_spread() * 0.1, 1.0 + random_spread() * 0.1);

		done = true;
	}
}

void Charge::end() {
	active = false;
	done = true;
}
