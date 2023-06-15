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
	// registry.emplace<DebugColor>( enemy, raylib::LIME);
	registry.emplace<AnimationState>( enemy,
		IDLE,
		&sprite_list["guard"],
		(float)GetRandomValue(0, 100) / 12.0f
	);
	registry.emplace<Health>(enemy, 100, 100);
	registry.emplace<Facing>(enemy, direction);
	registry.emplace<Enemy>(enemy, 700.0f, 400.0f, 3.0f, true);

	Gun gun(enemy, 1, 20, 0.5, 15.0, 0.8333);
	registry.emplace<GunAttack>(enemy, gun);
}

void make_player(float x, float y, int direction) {
	const auto player = registry.create();
	registry.emplace<Player>(player);
	registry.emplace<Gravity>(player);
	registry.emplace<Position>( player, raylib::Vector2(x, y) );
	registry.emplace<Velocity>( player, raylib::Vector2(0, 0) );
	registry.emplace<Collider>( player, 64.0f, 128.0f, false, 0, true );
	// registry.emplace<DebugColor>( player, raylib::VIOLET);
	registry.emplace<AnimationState>( player,
		IDLE,
		&sprite_list["vampire"],
		0.0f
	);
	registry.emplace<MeleeAttack>( player, 100.0f, 50, 0.5f, 0.0f, false);
	registry.emplace<Facing>(player, direction);
	registry.emplace<Health>(player, 100, 100);
	registry.emplace<BiteAttack>(player, 64.0f, 0.0f, 1, false);
	registry.emplace<Jump>(player, 10.0f, 0.5f, 0.1f, 0.1f);
}
