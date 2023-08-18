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
