#include <iostream>
#include <raylib/raylib-cpp.hpp>

#include "systems.hh"
#include "globals.hh"
#include "components.hh"

void melee_attack() {
	auto view = registry.view<const MeleeAttack, const RayCast>();

	for ( auto [entity, attack, ray] : view.each() ) {
		// Loop over potential targets
		auto target_view = registry.view<const Position, const Collider>();
		for ( auto [target, position, collider] : target_view.each() ) {
			if ( ray.intersect( collider.get_rectangle(position.value) ) )
				std::cout << "SLASH!" << '\n';
		}

		registry.remove<RayCast>(entity); // Delete the ray cast
	}
}
