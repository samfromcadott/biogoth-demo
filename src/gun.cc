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

	auto offset_data = toml::find< std::vector<float> >(data, "offset");
	this->offset = vec2( offset_data[0], offset_data[1] );
}

void Gun::fire() {
	active = true;

	auto& position = *registry.try_get<Position>(owner);
	auto& collider = *registry.try_get<Collider>(owner);
	auto& facing = *registry.try_get<Facing>(owner);

	vec2 bullet_start = position.value + offset * vec2(facing.direction, 1.0);

	// Create bullets
	for (int i = 0; i < count; i++) {
		vec2 v;
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
