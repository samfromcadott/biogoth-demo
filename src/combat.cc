#include <iostream>
#include <algorithm>
#include <raylib-cpp.hpp>

#include "systems.hh"
#include "globals.hh"
#include "components.hh"
#include "audio.hh"
#include "util.hh"

void death() {
	auto view = registry.view<const Health, Collider, AnimationState, Character, Movement>();
	for ( auto [entity, health, collider, animation, character, movement] : view.each() ) {
		if ( health.now > 0 ) continue; // Skip living characters

		// Remove stun from stunned characters
		if ( registry.any_of<Stun>(entity) ) {
			registry.remove<Stun>(entity);
			character.active = true;
		}

		// Only do right after death
		if (!character.active) continue;

		collider.enabled = false;
		character.active = false;
		movement.direction.x = 0;

		play_sound(character.death_sound, 0.4 + random_spread() * 0.1, 1.0 + random_spread() * 0.1);
	}
}

void weapon_update() {
	auto view = registry.view<WeaponSet>();

	for ( auto [entity, weapon_set] : view.each() ) {
		for (auto& weapon : weapon_set) weapon->update();
	}
}

void bullets() {
	auto view = registry.view<Position, const Velocity, const Bullet>();
	for ( auto [entity, position, velocity, bullet] : view.each() ) {
		position.value += velocity.value;

		// Check for tile collision
		if ( tilemap( tilemap.world_to_tile(position.value) ) != 0 ) {
			registry.destroy(entity); // Destroy the bullet
			continue;
		}

		// Collider collisions
		auto target_view = registry.view<const Position, const Collider, Health>();
		for ( auto [target, target_position, target_collider, target_health] : target_view.each() ) {
			// Check if the bullet is in a collider
			if ( !target_collider.get_rectangle(target_position.value).CheckCollision(position.value) )
				continue;

			if ( !target_collider.enabled ) continue; // Skip disabled colliders

			target_health.now -= bullet.damage;
			registry.destroy(entity); // Destroy the bullet
			break; // Stop looping over targets
		}

	}
}
