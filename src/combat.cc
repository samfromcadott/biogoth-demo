#include <iostream>
#include <raylib/raylib-cpp.hpp>

#include "systems.hh"
#include "globals.hh"
#include "components.hh"

void death() {
	auto view = registry.view<const Health>();
	for ( auto [entity, health] : view.each() ) {
		// Delete entities with 0 health
		if ( health.now <= 0 )
			registry.destroy(entity);
	}
}

void melee_attack() {
	auto view = registry.view<MeleeAttack, const RayCast>();

	for ( auto [entity, attack, ray] : view.each() ) {
		if (!attack.active) continue;

		// Loop over potential targets
		auto target_view = registry.view<const Position, const Collider, Health>();
		for ( auto [target, position, collider, health] : target_view.each() ) {
			if ( ray.intersect( collider.get_rectangle(position.value) ) ) {
				std::cout << "SLASH!" << '\n';
				health.now -= attack.damage;
			}
		}

		registry.remove<RayCast>(entity); // Delete the ray cast
		attack.active = false;
	}
}

void bite_attack() {
	auto view = registry.view<BiteAttack, const RayCast, Health, const Position>();

	for ( auto [entity, bite, ray, health, position] : view.each() ) {
		if (!bite.active) continue; // Skip entities that don't want to bite

		bite.timer -= GetFrameTime(); // Count down the timer

		// Loop over potential targets
		auto target_view = registry.view<Position, Collider, Health, Enemy>();
		for ( auto [target, target_position, target_collider, target_health, enemy] : target_view.each() ) {
			// Skip non-interected entities
			if ( !ray.intersect( target_collider.get_rectangle(target_position.value) ) ) continue;

			enemy.active = false; // Disable the enemy

			// Move the target to the biter
			target_collider.enabled = false;
			target_position.value.x = (ray.end.x + position.value.x) / 2;
			target_position.value.y = position.value.y;

			if (bite.timer <= 0.0) {
				// Suck blood
				target_health.now -= bite.damage;
				health.now += bite.damage;
				if ( health.now > health.max ) health.now = health.max;

				std::cout << "Drained " << bite.damage << " health" << '\n';

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
		if ( tilemap( tilemap.world_to_tile(position.value) ) != 0 )
			registry.destroy(entity); // Destroy the bullet

		// Collider collisions
		auto target_view = registry.view<const Position, const Collider, Health>();
		for ( auto [target, target_position, target_collider, target_health] : target_view.each() ) {
			// Check if the bullet is in a collider
			if ( !target_collider.get_rectangle(target_position.value).CheckCollision(position.value) )
				continue;

			target_health.now -= bullet.damage;
			registry.destroy(entity); // Destroy the bullet
		}

	}
}
