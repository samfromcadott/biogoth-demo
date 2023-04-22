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

void create_level() {

}
