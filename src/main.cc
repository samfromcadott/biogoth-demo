#include <raylib/raylib-cpp.hpp>

#include "globals.hh"
#include "components.hh"
#include "systems.hh"
#include "tilemap.hh"

using namespace raylib;

entt::registry registry;

int main() {
	int screen_width = 1280;
	int screen_height = 720;

	Window window(screen_width, screen_height, "raylib-cpp - basic window");

	SetTargetFPS(60);

	raylib::Rectangle player_rect(100, 100, 64, 128);
	raylib::Rectangle floor_rect(200, 200, 128, 64);

	raylib::Vector2 player_velocity(0.0, 0.0);

	const auto player = registry.create();
	registry.emplace<Player>(player);
	registry.emplace<Position>( player, raylib::Vector2(100, 100) );
	registry.emplace<Velocity>( player, raylib::Vector2(0, 0) );
	registry.emplace<Collider>( player, 64.0f, 128.0f );

	Tilemap tilemap(10, 10);
	tilemap(2, 1) = 1;
	tilemap(3, 1) = 1;

	while ( !window.ShouldClose() ) {
		player_move();
		move_collide();

		BeginDrawing();

			window.ClearBackground(raylib::RAYWHITE);

			auto view = registry.view<const Player, const Position, const Collider>();
			for ( auto [player, position, collider] : view.each() ) {
				collider.get_rectangle(position.value).Draw(raylib::VIOLET);
			}

			floor_rect.Draw(raylib::GRAY);

			tilemap.draw();

		EndDrawing();
	}

	return 0;
}
