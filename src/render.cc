#include <iostream>
#include <cmath>
#include <raylib/raylib-cpp.hpp>

#include "globals.hh"
#include "components.hh"
#include "systems.hh"

void render_game(raylib::Window& window) {
	BeginDrawing();
	camera.BeginMode();

		window.ClearBackground( raylib::Color(111, 133, 163, 255) );

		tilemap.draw();

		render_colliders();
		render_collider_sprites();
		render_bullets();

	camera.EndMode();

	// UI
	health_bar();
	if (player_died) death_text();

	EndDrawing();
}

void render_colliders() {
	auto view = registry.view<const Position, const Collider, const DebugColor>();
	for ( auto [entity, position, collider, color] : view.each() ) {
		collider.get_rectangle(position.value).Draw(color.color);
	}
}

void render_bullets() {
	auto view = registry.view<const Position, const Bullet>();
	for ( auto [entity, position, bullet] : view.each() ) {
		DrawCircleV(position.value, 4, ORANGE);
	}
}

void render_collider_sprites() {
	auto view = registry.view<const Position, const Collider, AnimationState, const Facing>();
	for ( auto [entity, position, collider, animation, facing] : view.each() ) {
		// Update the timer
		animation.timer += GetFrameTime();

		// Pause at end of death animation
		const float death_length = float(animation.sprite->length[DIE]) / float(animation.sprite->rate);
		if ( animation.state == DIE && animation.timer >= death_length ) {
			animation.timer = death_length - GetFrameTime();
		}

		// Render the sprite
		animation.sprite->render(
			position.value.x, position.value.y - collider.height/2,
			animation.state,
			animation.timer,
			facing.direction
		);
	}
}
