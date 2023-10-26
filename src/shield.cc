#include <iostream>
#include <raylib-cpp.hpp>

#include "audio.hh"
#include "globals.hh"
#include "components.hh"
#include "weapon.hh"
#include "systems.hh"
#include "util.hh"

Shield::Shield(entt::entity owner, toml::value data) {
	this->owner = owner;
	this->damage = toml::find<int>(data, "damage");
	this->width = toml::find<float>(data, "width");
	this->rate = toml::find<float>(data, "rate");
	this->length = toml::find<float>(data, "length");
	this->deflect = toml::find<bool>(data, "deflect");
}

void Shield::fire() {
	active = true;
	timer = rate;

	auto& position = *registry.try_get<Position>(owner);
	auto& collider = *registry.try_get<Collider>(owner);
	auto& facing = *registry.try_get<Facing>(owner);
	auto& character = *registry.try_get<Character>(owner);

	// Create a rectangle in front of the character
	raylib::Rectangle rect;
	rect.x = position.value.x + (collider.width/2) * facing.direction;
	rect.y = position.value.y - collider.height;
	rect.width = width;
	rect.height = collider.height;

	play_sound("sword_swing", 0.6 + random_spread() * 0.1, 1.0 + random_spread() * 0.2);

	// Look for targets
	auto target_view = registry.view<const Position, const Collider, Velocity, Health>();
	for ( auto [target, target_position, target_collider, target_velocity, target_health] : target_view.each() ) {
		if (!target_collider.enabled) continue;
		if ( !rect.CheckCollision( target_collider.get_rectangle(target_position.value) ) ) continue;
		if (target == owner) continue; // Don't harm self
		if (target_health.now <= 0) continue; // Skip dead enemies

		std::cout << "SLASH!" << '\n';
		vec2 facing_vector = vec2( facing.direction, 0.0 );
		deal_damage(target, damage, facing_vector);

		// Play the sound effect
		play_sound("sword_hit", 0.7 + random_spread() * 0.1, 1.0 + random_spread() * 0.1);
	}
}

void Shield::update() {
	if (!active) return;

	timer -= GetFrameTime();
	if (timer <= 0.0) end();
	if (timer > length) return; // Don't do anything pass the blocking window

	auto& position = *registry.try_get<Position>(owner);
	auto& collider = *registry.try_get<Collider>(owner);
	auto& facing = *registry.try_get<Facing>(owner);

	// Create a rectangle in front of the character
	raylib::Rectangle rect;
	rect.x = position.value.x + (collider.width/2) * facing.direction;
	rect.y = position.value.y - collider.height;
	rect.width = width;
	rect.height = collider.height;

	// Check for any bullets in the rectangle
	auto view = registry.view<Position, Velocity, const Bullet>();
	for ( auto [bullet, bullet_position, bullet_velocity, bullet_component] : view.each() ) {
		if ( !bullet_position.value.CheckCollision(rect) ) continue;

		// If deflect is false, destroy them
		if (!deflect) {
			registry.destroy(bullet);
			continue;
		}

		// If deflect is true,
		// 	If the bullet is traveling towards the character, reverse its direction
		if ( sign(bullet_velocity.value.x) == sign(position.value.x - bullet_position.value.x) )
			bullet_velocity.value.x *= -1;
	}
}

void Shield::end() {
	active = false;
}
