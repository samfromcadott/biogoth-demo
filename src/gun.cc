#include <iostream>
#include <raylib-cpp.hpp>

#include "audio.hh"
#include "globals.hh"
#include "components.hh"
#include "weapon.hh"
#include "util.hh"

Gun::Gun(entt::entity owner, toml::value data) {
	this->owner = owner;
	this->damage = toml::find<int>(data, "damage");
	this->count = toml::find<unsigned int>(data, "count");
	this->spread = toml::find<float>(data, "spread");
	this->speed = toml::find<float>(data, "speed");
	this->rate = toml::find<float>(data, "rate");
}

void Gun::fire() {
	active = true;

	auto& position = *registry.try_get<Position>(owner);
	auto& collider = *registry.try_get<Collider>(owner);
	auto& facing = *registry.try_get<Facing>(owner);

	raylib::Vector2 bullet_start =
		position.value + raylib::Vector2((collider.width/2+0.001)*facing.direction, -collider.height/2);

	// Create bullets
	for (int i = 0; i < count; i++) {
		raylib::Vector2 v;
		v.x = facing.direction;
		v.y = spread * random_spread();
		v = v.Normalize();
		v *= speed;

		const auto bullet = registry.create();
		registry.emplace<Position>( bullet, bullet_start );
		registry.emplace<Velocity>( bullet, v );
		registry.emplace<Bullet>( bullet, damage, &sprite_list["bullet"] );
	}

	timer = rate;
	play_sound("gun", 0.4 + random_spread() * 0.1, 1.0 + random_spread() * 0.1);
}

void Gun::update() {
	timer -= GetFrameTime();
	if (timer <= 0.0 && active) end();
}

void Gun::end() {
	active = false;
}
