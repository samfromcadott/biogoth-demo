#include <iostream>
#include <string>
#include <raylib-cpp.hpp>

#include "systems.hh"
#include "globals.hh"
#include "components.hh"

const raylib::Vector2 bar_origin(32, 550);
const float max_height = 150;
const float bar_width = 32;
const float bar_speed = 2.0;
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
	const int margin_left = 64;
	const int margin_top = 32;

	const int title_size = 128;
	const int name_size = 32;
	const int help_size = 64;

	DrawRectangle( 0, 0, screen_width, screen_height, raylib::Color(0, 0, 0, 127) );

	title_font.DrawText("HEMOGLOBIN - MVP", margin_left, margin_top, title_size, raylib::WHITE);
	normal_font.DrawText("Sam Jackson", margin_left, margin_top+title_size+4, name_size, raylib::WHITE);

	normal_font.DrawText(
		"ARROWS to move\nCTRL to bite\nSPACE to jump\nV to bite",
		margin_left,
		margin_top+title_size+name_size+64,
		help_size,
		raylib::WHITE
	);
}

void death_text() {
	DrawRectangle( 0, 0, screen_width, screen_height, raylib::Color(0, 0, 0, 127) );
	title_font.DrawText("YOU DIED", 256, screen_height/2, 128, raylib::WHITE);
}
