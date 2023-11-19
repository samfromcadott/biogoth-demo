#include <iostream>
#include <raylib-cpp.hpp>

#include "audio.hh"
#include "globals.hh"
#include "components.hh"
#include "weapon.hh"
#include "systems.hh"
#include "util.hh"

Charge::Charge(entt::entity owner, toml::value data) {
	this->owner = owner;
	this->min_damage = toml::find<int>(data, "min_damage");
	this->max_damage = toml::find<int>(data, "max_damage");
	this->max_speed = toml::find<float>(data, "max_speed");
	this->width = toml::find<float>(data, "width");
	this->height = toml::find<float>(data, "height");
	this->push = toml::find<float>(data, "push");

	auto direction_data = toml::find< std::vector<float> >(data, "direction");
	this->direction = vec2( direction_data[0], direction_data[1] );

	auto offset_data = toml::find< std::vector<float> >(data, "offset");
	this->offset = vec2( offset_data[0], offset_data[1] );
}

void Charge::fire() {
	auto& character = *registry.try_get<Character>(owner);

	active = true;
	done = false;
	character.active = false;
}

void Charge::update() {
	if (!active) return;

	auto& position = *registry.try_get<Position>(owner);
	auto& collider = *registry.try_get<Collider>(owner);
	auto& facing = *registry.try_get<Facing>(owner);
	auto& velocity = *registry.try_get<Velocity>(owner);

	raylib::Rectangle rect;
	rect.width = width; rect.height = height;
	rect.x = facing.direction == -1? position.value.x - offset.x - width : position.value.x + offset.x;
	rect.y = position.value.y - offset.y;

	float speed = velocity.value.Length();
	if (speed < 0.1 && collider.on_floor) end();

	// Look for targets
	auto target_view = registry.view<const Position, const Collider, Velocity, Health>();
	for ( auto [target, target_position, target_collider, target_velocity, target_health] : target_view.each() ) {
		if (!target_collider.enabled) continue;
		if ( !rect.CheckCollision( target_collider.get_rectangle(target_position.value) ) ) continue;
		if (target == owner) continue; // Don't harm self
		if (target_health.now <= 0) continue; // Skip dead enemies

		vec2 facing_vector = vec2( facing.direction, 0.0 );

		// Calculate damage
		int damage = Remap(speed, 0.0, max_speed, min_damage, max_damage);
		damage = Clamp(damage, min_damage, max_damage);
		std::cout << "CHARGE!" <<  " " << damage << '\n';
		deal_damage(target, damage, direction * facing_vector);

		// Push the enemy back
		target_velocity.value += direction.Normalize() * push * facing_vector;

		// Play the sound effect
		play_sound("sword_hit", 0.7 + random_spread() * 0.1, 1.0 + random_spread() * 0.1);

		done = true;
	}

	if (done) end();
}

void Charge::end() {
	auto& character = *registry.try_get<Character>(owner);

	active = false;
	done = true;
	character.active = true;
}
