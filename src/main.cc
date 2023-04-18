#include <raylib/raylib-cpp.hpp>

#include "globals.hh"
#include "components.hh"
#include "systems.hh"
#include "tilemap.hh"

using namespace raylib;

entt::registry registry;
Tilemap tilemap(10, 10);

const float G = 13.0;

int main() {
	int screen_width = 1280;
	int screen_height = 720;

	Window window(screen_width, screen_height, "raylib-cpp - basic window");

	SetTargetFPS(60);

	const auto player = registry.create();
	registry.emplace<Player>(player);
	registry.emplace<Gravity>(player);
	registry.emplace<Position>( player, raylib::Vector2(100, 100) );
	registry.emplace<Velocity>( player, raylib::Vector2(0, 0) );
	registry.emplace<Collider>( player, 64.0f, 128.0f, false, 0 );

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
	tilemap(9, 8) = 1;

	tilemap(5, 5) = 1;
	tilemap(5, 6) = 1;

	while ( !window.ShouldClose() ) {
		player_move();
		gravity();
		move_collide();

		BeginDrawing();

			window.ClearBackground(raylib::RAYWHITE);

			auto view = registry.view<const Player, const Position, const Collider>();
			for ( auto [player, position, collider] : view.each() ) {
				collider.get_rectangle(position.value).Draw(raylib::VIOLET);
			}

			tilemap.draw();

		EndDrawing();
	}

	return 0;
}
