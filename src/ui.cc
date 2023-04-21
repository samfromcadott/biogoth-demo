#include <iostream>
#include <string>
#include <raylib/raylib-cpp.hpp>

#include "systems.hh"
#include "globals.hh"
#include "components.hh"

void health_bar() {
	auto view = registry.view<const Player, const Health>();
	for ( auto [entity, player, health] : view.each() ) {
		raylib::Vector2 bar_origin(32, 550);
		float max_height = 150;
		float bar_width = 32;
		float height_loss = max_height - ( (float(health.now) / float(health.max)) * max_height );

		DrawRectangle(int(bar_origin.x), int(bar_origin.y+height_loss), bar_width, max_height-height_loss, RED);
	}
}
