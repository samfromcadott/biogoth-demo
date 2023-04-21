#include <iostream>
#include <string>
#include <raylib/raylib-cpp.hpp>

#include "systems.hh"
#include "globals.hh"
#include "components.hh"

void health_bar() {
	auto view = registry.view<const Player, const Health>();
	for ( auto [entity, player, health] : view.each() ) {
		raylib::DrawText(""+std::to_string(health.now)+"/"+std::to_string(health.max), 32, 650, 32, raylib::BLACK);
	}
}
