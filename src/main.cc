#include <iostream>
#include <raylib/raylib-cpp.hpp>

#include "globals.hh"
#include "components.hh"
#include "systems.hh"
#include "tilemap.hh"
#include "level.hh"

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

	make_player(100, 180, +1);
	// make_enemy(500, 180, -1);
	// make_enemy(900, 352, -1);
	// make_enemy(830, 180, -1);
	// make_enemy(1200, 180, -1);
	// make_enemy(100, 580, +1);
	// make_enemy(600, 780, +1);
	// make_enemy(1200, 1000, -1);
	// make_enemy(70, 1000, -1);
	// make_level();
	tilemap = Tilemap("assets/levels/test.json");

	while ( !window.ShouldClose() ) {
		// Player actions
		player_move();
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

		camera.EndMode();

		// UI
		health_bar();

		EndDrawing();

	}

	return 0;
}
