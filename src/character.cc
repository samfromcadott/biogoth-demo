#include <iostream>
#include <raylib-cpp.hpp>

#include "systems.hh"
#include "globals.hh"
#include "components.hh"
#include "util.hh"
#include "audio.hh"
#include "controls.hh"

void character_think() {
	for ( auto [entity, character] : registry.view<const Character>().each() ) {
		if (character.active == false) continue;
		if (character.brain == nullptr) continue;

		character.brain->think();
	}
}

void stun() {
	for ( auto [entity, character, stun] : registry.view<Character, Stun>().each() ) {
		stun.timer -= GetFrameTime();
		character.active = stun.timer > 0.0? false : true;

		if (stun.timer <= 0.0) registry.remove<Stun>(entity);
	}
}

void death_by_pitfall() {
	for ( auto [entity, character, position, health, collider] : registry.view<Character, Position, Health, Collider>().each() ) {
		// Check if the character's whole collider has fallen out of the map
		if (position.value.y > tilemap.height * tilemap.tile_size + collider.height )
			health.now = 0;
	}
}
