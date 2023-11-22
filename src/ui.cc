#include <iostream>
#include <string>
#include <raylib-cpp.hpp>

#include "systems.hh"
#include "globals.hh"
#include "components.hh"

const vec2 bar_origin(32, 550);
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

		unsigned char redness = ( target_height / max_height ) * 255;

		raylib::Rectangle bar(int(bar_origin.x), int(bar_origin.y+height_loss), bar_width, current_bar_height);
		raylib::Rectangle bar_back(int(bar_origin.x), int(bar_origin.y), bar_width, max_height);

		bar_back.Draw(BLACK);
		bar.DrawGradientV( rgba(redness, 0, 0, 255), BLACK );
		blood_bar.Draw( bar_origin - vec2(18, 4) );
	}
}

void help_text() {
	intro_screen.Draw();
}

void death_text() {
	death_screen.Draw();
}

void end_text() {
	outro_screen.Draw();
}
