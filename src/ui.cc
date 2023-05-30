#include <iostream>
#include <string>
#include <raylib/raylib-cpp.hpp>

#include "systems.hh"
#include "globals.hh"
#include "components.hh"

const raylib::Vector2 bar_origin(32, 550);
const float max_height = 150;
const float bar_width = 32;
const float bar_speed = 5.0;
float current_bar_height = max_height;

void health_bar() {
	auto view = registry.view<const Player, const Health>();
	for ( auto [entity, player, health] : view.each() ) {
		float target_height = ( float(health.now) / float(health.max) ) * max_height;
		current_bar_height += (target_height - current_bar_height) * bar_speed * GetFrameTime();
		float height_loss = max_height - current_bar_height;

		DrawRectangle(int(bar_origin.x), int(bar_origin.y+height_loss), bar_width, current_bar_height, RED);
	}
}

void help_text() {

}

void death_text() {
	DrawRectangle( 0, 0, screen_width, screen_height, raylib::Color(0, 0, 0, 127) );
	raylib::WHITE.DrawText("YOU DIED", 256, screen_height/2, 128);
}
