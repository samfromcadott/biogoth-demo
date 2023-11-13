#include <iostream>
#include <raylib-cpp.hpp>

#include "audio.hh"
#include "globals.hh"
#include "components.hh"
#include "weapon.hh"
#include "util.hh"

Bite::Bite(entt::entity owner, toml::value data) {
	this->owner = owner;
	this->damage = toml::find<unsigned int>(data, "damage");
	this->range = toml::find<float>(data, "range");
	this->action = BITE;
}

void Bite::fire() {
	auto& position = *registry.try_get<Position>(owner);
	auto& velocity = *registry.try_get<Velocity>(owner);
	auto& collider = *registry.try_get<Collider>(owner);
	auto& facing = *registry.try_get<Facing>(owner);

	RayCast ray;
	ray.start = position.value + vec2( (collider.width/2+0.5)*facing.direction, -collider.height/2 );
	ray.end = ray.start + vec2(range) * facing.direction;

	// Loop over potential targets
	auto target_view = registry.view<Position, Character, Velocity, Collider, Health, AnimationState>();
	for ( auto [target, target_position, character, target_velocity, target_collider, target_health, target_animation] : target_view.each() ) {
		if (target == owner) continue; // Skip self
		if (!character.active) continue; // Skip disabled character
		if (!target_collider.enabled) continue; // Skip disabled colliders
		if ( !ray.intersect( target_collider.get_rectangle(target_position.value) ) ) continue; // Skip non-interected entities
		if ( target_health.now <= 0 ) continue; // Skip dead enemies

		// Disable movement
		registry.get<Movement>(owner).can_move = false;

		velocity.value.x = 0.0;

		// Play the guard scream
		play_sound(character.bite_sound, 0.7);

		target_animation.set_state(BITE);

		// Disable the character
		character.active = false;
		character.bitten = true;

		target_collider.enabled = false; // Disble the collider

		this->target = target;
		active = true;
		has_target = true;
		break; // Only suck blood from one enemy
	}
}

void Bite::update() {
	if (!active) return;
	if (!has_target) return;

	timer -= GetFrameTime(); // Count down the timer
	if (timer > 0.0) return;

	auto owner_health = registry.try_get<Health>(owner);
	auto target_health = registry.try_get<Health>(target);
	auto& position = *registry.try_get<Position>(owner);
	auto& target_position = *registry.try_get<Position>(target);
	auto& facing = *registry.try_get<Facing>(owner);

	if (target_health->now <= 0) end();

	// Move the target to the biter
	target_position.value.x = position.value.x + facing.direction * range / 2;
	target_position.value.y = position.value.y;

	// Suck blood
	int drain = std::min(damage, target_health->now);
	target_health->now -= drain;
	owner_health->now += drain;
	owner_health->now = std::min(owner_health->now, owner_health->max); // Clamp health

	timer = 0.1; // Reset timer
}

void Bite::end() {
	active = false;

	// Reenable movementr
	registry.get<Movement>(owner).can_move = true;

	if (!has_target) return;

	auto& velocity = *registry.try_get<Velocity>(target);
	auto& collider = *registry.try_get<Collider>(target);
	auto& character = *registry.try_get<Character>(target);

	has_target = false;

	// Enable the target
	registry.emplace_or_replace<Stun>(target, 1.0f);


	velocity.value.y -= 3.0; // Stops them from attacking for a bit
	collider.enabled = true; // Enable collider if target is alive
	character.bitten = false;
}
