#include <iostream>
#include <raylib/raylib-cpp.hpp>

#include "systems.hh"
#include "globals.hh"
#include "components.hh"

using namespace raylib;

void player_move() {
	auto view = registry.view<const Player, Velocity>();

	for( auto [player, velocity] : view.each() ) {
		velocity.value = raylib::Vector2(0.0, 0.0);
		const float speed = 5.0;

		if ( IsKeyDown(KEY_RIGHT) )  velocity.value.x += speed;
		if ( IsKeyDown(KEY_LEFT) ) velocity.value.x -= speed;
		if ( IsKeyDown(KEY_UP) ) velocity.value.y -= speed;
		if ( IsKeyDown(KEY_DOWN) ) velocity.value.y += speed;
	}
}
