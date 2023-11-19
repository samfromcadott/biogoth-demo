#include <iostream>
#include <cmath>
#include <raylib-cpp.hpp>

#include "globals.hh"
#include "components.hh"
#include "systems.hh"
#include "camera.hh"

void render_game(raylib::Window& window) {
	BeginDrawing();
	CameraSystem::get_camera().BeginMode();

		window.ClearBackground( rgba(111, 133, 163, 255) );

		tilemap.draw();

		render_colliders();
		render_collider_sprites();
		render_bullets();
		render_particles();

	CameraSystem::get_camera().EndMode();

	// UI
	health_bar();
	if (show_help) help_text();
	else if (player_died) death_text();
	else if (player_won) end_text();

	DrawFPS(10, 10);

	EndDrawing();
}

void render_colliders() {
	auto view = registry.view<const Position, const Collider, const DebugColor>();
	for ( auto [entity, position, collider, color] : view.each() ) {
		collider.get_rectangle(position.value).Draw(color.color);
	}
}

void render_bullets() {
	auto view = registry.view<const Position, const Bullet, const Velocity>();
	for ( auto [entity, position, bullet, velocity] : view.each() ) {
		float rotation = atan2(velocity.value.y, velocity.value.x) * (180/PI);

		// Render the bullet sprite
		if (bullet.sprite)
			bullet.sprite->render(position.value, IDLE, 0.0, +1, rotation);
		else
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
