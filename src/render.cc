#include <iostream>
#include <cmath>
#include <raylib/raylib-cpp.hpp>

#include "globals.hh"
#include "components.hh"
#include "systems.hh"

void render_colliders() {
	auto view = registry.view<const Position, const Collider, const DebugColor>();
	for ( auto [entity, position, collider, color] : view.each() ) {
		collider.get_rectangle(position.value).Draw(color.color);
	}
}