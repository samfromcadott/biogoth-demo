#include <iostream>
#include <algorithm>
#include <raylib/raylib-cpp.hpp>

#include "systems.hh"
#include "globals.hh"
#include "components.hh"
#include "audio.hh"
#include "util.hh"

void death() {
	auto view = registry.view<const Health, Collider, AnimationState, Enemy, Velocity>();
	for ( auto [entity, health, collider, animation, enemy, velocity] : view.each() ) {
		if ( health.now > 0 ) continue; // Skip living characters

		velocity.value.x = move_towards(velocity.value.x, 0.0, 1.0);

		// Only do right after death
		if (!enemy.active) continue;

		collider.enabled = false;
		enemy.active = false;
		animation.set_state(DIE);

		play_sound("guard_death", 0.4 + random_spread() * 0.1, 1.0 + random_spread() * 0.1);
	}
}

void melee_attack() {
	auto view = registry.view<MeleeAttack>();

	for ( auto [entity, melee_attack] : view.each() ) {
		melee_attack.melee.update();
	}
}

void bite_attack() {
	auto view = registry.view<BiteAttack>();

	for ( auto [entity, bite_attack] : view.each() ) {
		bite_attack.bite.update();
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
