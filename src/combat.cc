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
		if ( health.now > 0 ) continue;

		velocity.value.x = move_towards(velocity.value.x, 0.0, 1.0);

		if (animation.state == DIE) continue; // Skip dead enemies

		collider.enabled = false;
		enemy.active = false;
		animation.set_state(DIE);

		play_sound("guard_death", 0.4 + random_spread() * 0.1, 1.0 + random_spread() * 0.1);
	}
}

void melee_attack() {
	auto view = registry.view<MeleeAttack, RayCast>();

	for ( auto [entity, attack, ray] : view.each() ) {
		attack.timer -= GetFrameTime(); // Count down the timer

		if (!attack.active) continue;

		if (attack.timer <= 0) {
			attack.active = false;
			registry.remove<RayCast>(entity); // Delete the ray cast
			continue;
		}


		// Loop over potential targets
		auto target_view = registry.view<const Position, const Collider, Velocity, Health>();
		for ( auto [target, position, collider, velocity, health] : target_view.each() ) {
			if ( !ray.intersect( collider.get_rectangle(position.value) ) ) continue;
			if (target == entity) continue; // Don't harm self
			if (health.now <= 0) continue; // Skip dead enemies

			std::cout << "SLASH!" << '\n';
			health.now -= attack.damage;

			// Push the enemy back
			velocity.value.x -= sign(ray.start.x - ray.end.x) * (attack.damage / 8.0);
			velocity.value.y -= 1.0;

			// Play the sound effect
			play_sound("sword_hit", 0.7 + random_spread() * 0.1, 1.0 + random_spread() * 0.1);

			attack.active = false;
		}

		// Put the ray cast out of bounds
		ray.start = raylib::Vector2(0, 0);
		ray.end = raylib::Vector2(0, 0);
	}
}

void bite_attack() {
	auto view = registry.view<BiteAttack, const RayCast, Health, const Position>();

	for ( auto [entity, bite, ray, health, position] : view.each() ) {
		bite.timer -= GetFrameTime(); // Count down the timer

		// Loop over potential targets
		auto target_view = registry.view<Position, Collider, Health, AnimationState, Enemy>();
		for ( auto [target, target_position, target_collider, target_health, target_animation, enemy] : target_view.each() ) {
			// Skip non-interected entities
			if ( !ray.intersect( target_collider.get_rectangle(target_position.value) ) ) continue;
			if ( target_health.now <= 0 ) continue; // Skip dead enemies

			// When entity stops biting
			if (!bite.active) {
				registry.remove<RayCast>(entity); // Delete the ray cast
				enemy.active = true; // Enable the enemy
				target_collider.enabled = true;
				break;
			}

			// Play the guard scream
			if( enemy.active ) // Only play audio when first bitten
				play_sound("guard_bitten", 0.7);

			target_animation.set_state(BITE);

			enemy.active = false; // Disable the enemy

			// Move the target to the biter
			target_collider.enabled = false;
			target_position.value.x = position.value.x + (ray.end.x - ray.start.x) / 2;
			target_position.value.y = position.value.y;

			if (bite.timer <= 0.0) {
				// Suck blood
				int damage = std::min(bite.damage, target_health.now);
				target_health.now -= damage;
				health.now += damage;
				health.now = std::min(health.now, health.max); // Clamp health

				std::cout << "Drained " << damage << " health" << '\n';

				bite.timer = 1.0; // Reset timer
			}

			break; // Only suck blood from one enemy
		}
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
