#include <raylib-cpp.hpp>

#include "systems.hh"
#include "globals.hh"
#include "components.hh"

// Check if they have an active attack
Weapon* get_active_weapon(entt::entity entity) {
	if ( !registry.any_of<WeaponSet>(entity) ) return nullptr;

	for ( auto weapon : registry.get<WeaponSet>(entity) )
		if (weapon->active) return weapon;

	return nullptr;
}

void animate_character() {
	auto view = registry.view<AnimationState, const Character, const Velocity, const Collider, const Health>();

	for ( auto [entity, animation, character, velocity, collider, health] : view.each() ) {
		Weapon* weapon = get_active_weapon(entity);

		if (health.now <= 0) animation.set_state(DIE);
		else if ( weapon && !collider.on_floor && weapon->action == ATTACK ) animation.set_state(AIR_ATTACK);
		else if ( weapon ) animation.set_state(weapon->action);
		else if ( collider.on_floor && velocity.value.x != 0 ) animation.set_state(WALK);
		else if ( collider.wall_direction != 0 && !collider.on_floor ) animation.set_state(WALL_SLIDE);
		else if ( !collider.on_floor ) animation.set_state(FALL);
		else if (character.bitten) animation.set_state(BITE);
		else animation.set_state(IDLE);
	}
}
