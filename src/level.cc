#include <raylib/raylib-cpp.hpp>

#include "globals.hh"
#include "components.hh"
#include "systems.hh"
#include "tilemap.hh"
#include "level.hh"

void make_enemy(float x, float y, int direction) {
	const auto enemy = registry.create();
	registry.emplace<Gravity>(enemy);
	registry.emplace<Position>( enemy, raylib::Vector2(x, y) );
	registry.emplace<Velocity>( enemy, raylib::Vector2(0, 0) );
	registry.emplace<Collider>( enemy, 64.0f, 128.0f, false, 0, true );
	registry.emplace<DebugColor>( enemy, raylib::LIME);
	registry.emplace<Health>(enemy, 100, 100);
	registry.emplace<Facing>(enemy, direction);
	registry.emplace<Enemy>(enemy, 500.0f, 100.0f, 3.0f, true);
	registry.emplace<GunAttack>(enemy, 1, 5, 0.5f, 15.0f, 1.0f, 0.0f);
}

void make_player(float x, float y, int direction) {
	const auto player = registry.create();
	registry.emplace<Player>(player);
	registry.emplace<Gravity>(player);
	registry.emplace<Position>( player, raylib::Vector2(x, y) );
	registry.emplace<Velocity>( player, raylib::Vector2(0, 0) );
	registry.emplace<Collider>( player, 64.0f, 128.0f, false, 0, true );
	registry.emplace<DebugColor>( player, raylib::VIOLET);
	registry.emplace<MeleeAttack>( player, 64.0f, 40, false);
	registry.emplace<Facing>(player, direction);
	registry.emplace<Health>(player, 100, 100);
	registry.emplace<BiteAttack>(player, 64.0f, 0.0f, 10, false);
}

void make_level() {
	tilemap(0, 0) = 1;
	tilemap(0, 1) = 1;
	tilemap(0, 2) = 1;
	tilemap(0, 3) = 1;
	tilemap(0, 4) = 1;
	tilemap(0, 5) = 1;
	tilemap(0, 6) = 1;
	tilemap(0, 7) = 1;
	tilemap(0, 8) = 1;
	tilemap(0, 9) = 1;

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
}
