#include <raylib-cpp.hpp>

#include "globals.hh"
#include "components.hh"
#include "systems.hh"
#include "tilemap.hh"
#include "level.hh"
#include "entities.hh"

void make_enemy(float x, float y, int direction) {
	spawn_entity("guard_shotgun", {x, y});
}

void make_player(float x, float y, int direction) {
	spawn_entity("player", {x, y});
}
