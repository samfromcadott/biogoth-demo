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
	auto view = registry.view<const MeleeAttack, const RayCast>();

	for ( auto [entity, attack, ray] : view.each() ) {
		// Loop over potential targets
		auto target_view = registry.view<const Position, const Collider, Health>();
		for ( auto [target, position, collider, health] : target_view.each() ) {
			if ( ray.intersect( collider.get_rectangle(position.value) ) )
				std::cout << "SLASH!" << '\n';
				health.now -= attack.damage;
		}

		registry.remove<RayCast>(entity); // Delete the ray cast
	}
}
