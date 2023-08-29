#include <iostream>
#include <raylib-cpp.hpp>

#include "audio.hh"
#include "globals.hh"
#include "components.hh"
#include "weapon.hh"
#include "util.hh"

Shield::Shield(entt::entity owner, toml::value data) {
	this->owner = owner;
	this->width = toml::find<float>(data, "width");
	this->rate = toml::find<float>(data, "rate");
	this->length = toml::find<float>(data, "length");
	this->deflect = toml::find<bool>(data, "deflect");
}

void Shield::fire() {
	active = true;
	timer = rate;
}

void Shield::update() {
	if (!active) return;

	timer -= GetFrameTime();
	if (timer <= 0.0) end();
	if (timer < length) return; // Don't do anything pass the blocking window

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
