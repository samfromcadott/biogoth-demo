#include <raylib-cpp.hpp>

#include "globals.hh"
#include "components.hh"
#include "systems.hh"
#include "tilemap.hh"
#include "level.hh"
#include "entities.hh"

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

	registry.emplace<WeaponSet>(enemy);
	registry.get<WeaponSet>(enemy).push_back( new Gun(enemy, 1, 20, 0.5, 15.0, 0.8333) );
}

void make_player(float x, float y, int direction) {
	spawn_entity("player", {x, y});
}
