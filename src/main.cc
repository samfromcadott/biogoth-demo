#include <iostream>
#include <raylib/raylib-cpp.hpp>

#include "globals.hh"
#include "components.hh"
#include "systems.hh"
#include "tilemap.hh"
#include "level.hh"
#include "sprite.hh"

using namespace raylib;

int screen_width = 1280;
int screen_height = 720;

entt::registry registry;
Tilemap tilemap(40, 40);
raylib::Camera2D camera( raylib::Vector2(screen_width/2, screen_height/2), raylib::Vector2(0.0, 0.0) );

const float G = 13.0;

int main() {
	Window window(screen_width, screen_height, "raylib-cpp - basic window");

	SetTargetFPS(60);

	tilemap = Tilemap("assets/levels/test.json");

	// Sprite test
	Sprite sprite("assets/graphics/sprites/sprite_test.png");

	while ( !window.ShouldClose() ) {
		// Player actions
		player_move();
		player_jump();
		jump_buffer();
		player_attack();
		player_bite();

		// Enemy actions
		enemy_think();

		// Combat
		melee_attack();
		bite_attack();
		bullets();

		// Physics
		gravity();
		collider_overlap();
		move_collide();

		death();

		BeginDrawing();
		camera.BeginMode();

			window.ClearBackground(raylib::RAYWHITE);

			render_colliders();
			render_bullets();

			camera_update();

			tilemap.draw();

			// Sprite test
			sprite.render(100, 200, IDLE, GetTime(), +1, GetTime()*180);

		camera.EndMode();

		// UI
		health_bar();

		EndDrawing();

	}

	return 0;
}
