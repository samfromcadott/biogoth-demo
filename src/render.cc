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

		// Render the sprite
		animation.sprite->render(
			position.value.x, position.value.y - collider.height/2,
			animation.state,
			animation.timer,
			facing.direction
		);
	}
}
