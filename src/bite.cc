#include <iostream>
#include <raylib-cpp.hpp>

#include "audio.hh"
#include "globals.hh"
#include "components.hh"
#include "weapon.hh"
#include "util.hh"

Bite::Bite(entt::entity owner, int damage, float range) {
	this->owner = owner;
	this->damage = damage;
	this->range = range;
}

void Bite::fire() {
	auto& position = *registry.try_get<Position>(owner);
	auto& velocity = *registry.try_get<Velocity>(owner);
	auto& collider = *registry.try_get<Collider>(owner);
	auto& facing = *registry.try_get<Facing>(owner);

	RayCast ray;
	ray.start = position.value + raylib::Vector2( (collider.width/2+0.001)*facing.direction, -collider.height/2 );
	ray.end = ray.start + raylib::Vector2(range) * facing.direction;

	// Loop over potential targets
	auto target_view = registry.view<Position, Velocity, Collider, Health, AnimationState>();
	for ( auto [target, target_position, target_velocity, target_collider, target_health, target_animation] : target_view.each() ) {
		// Skip non-interected entities
		if (!target_collider.enabled) continue;
		if ( !ray.intersect( target_collider.get_rectangle(target_position.value) ) ) continue;

		if ( target_health.now <= 0 ) continue; // Skip dead enemies

		// Disable the player
		if ( registry.any_of<Player>(owner) )
			registry.get<Player>(owner).can_move = false;

		velocity.value.x = 0.0;

		// Play the guard scream
		play_sound("guard_bitten", 0.7);

		target_animation.set_state(BITE);

		// Disable the enemy
		if ( registry.any_of<Enemy>(target) )
			registry.get<Enemy>(target).active = false;

		// Move the target to the biter
		target_collider.enabled = false;
		target_position.value.x = position.value.x + (ray.end.x - ray.start.x) / 2;
		target_position.value.y = position.value.y;

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

	if (target_health->now <= 0) end();

	// Suck blood
	int drain = std::min(damage, target_health->now);
	target_health->now -= drain;
	owner_health->now += drain;
	owner_health->now = std::min(owner_health->now, owner_health->max); // Clamp health

	timer = 0.1; // Reset timer
}

void Bite::end() {
	// Reenable the player
	if ( registry.any_of<Player>(owner) )
		registry.get<Player>(owner).can_move = true;

	if (!has_target) return;

	auto& velocity = *registry.try_get<Velocity>(target);
	auto& collider = *registry.try_get<Collider>(target);

	active = false;
	has_target = false;

	// Enable the enemy
	if ( registry.any_of<Enemy>(target) )
		registry.get<Enemy>(target).active = true;


	velocity.value.y -= 3.0; // Stops them from attacking for a bit
	collider.enabled = true; // Enable collider if target is alive
}
