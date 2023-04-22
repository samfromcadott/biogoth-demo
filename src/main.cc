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
Tilemap tilemap(30, 20);
raylib::Camera2D camera( raylib::Vector2(screen_width/2, screen_height/2), raylib::Vector2(0.0, 0.0) );

const float G = 13.0;

int main() {
	Window window(screen_width, screen_height, "raylib-cpp - basic window");

	SetTargetFPS(60);

	make_player(100, 100, +1);
	make_enemy(200, 100, -1);
	make_enemy(700, 100, -1);

	tilemap(0, 8) = 1;
	tilemap(0, 9) = 1;
	tilemap(1, 9) = 1;
	tilemap(2, 9) = 1;
	tilemap(3, 9) = 1;
	tilemap(4, 9) = 1;
	tilemap(5, 9) = 1;
	tilemap(6, 9) = 1;
	tilemap(7, 9) = 1;
	tilemap(8, 9) = 1;
	tilemap(9, 9) = 1;

	tilemap(10, 9) = 1;
	tilemap(11, 9) = 1;
	tilemap(12, 9) = 1;
	tilemap(13, 9) = 1;
	tilemap(14, 9) = 1;
	tilemap(15, 9) = 1;
	tilemap(16, 9) = 1;
	tilemap(17, 9) = 1;
	tilemap(18, 9) = 1;
	tilemap(19, 9) = 1;

	tilemap(13, 5) = 1;
	tilemap(14, 5) = 1;

	tilemap(20, 9) = 1;
	tilemap(21, 9) = 1;
	tilemap(22, 9) = 1;
	tilemap(23, 9) = 1;
	tilemap(24, 9) = 1;
	tilemap(25, 9) = 1;
	tilemap(26, 9) = 1;
	tilemap(27, 9) = 1;
	tilemap(28, 9) = 1;
	tilemap(29, 9) = 1;
	tilemap(29, 8) = 1;

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

			auto view = registry.view<const Player, const Position, const Collider>();
			render_colliders();
			render_bullets();

			// Camera update
			for ( auto [entity, player, position, collider] : view.each() ) {
				camera.target.x += (position.value.x - camera.target.x) * 0.98 * GetFrameTime();
				camera.target.y += (position.value.y - camera.target.y) * 0.6 * GetFrameTime();
			}

			tilemap.draw();

		camera.EndMode();

		// UI
		health_bar();

		EndDrawing();

	}

	return 0;
}
