#include <iostream>
#include <cmath>
#include <raylib/raylib-cpp.hpp>

#include "globals.hh"
#include "components.hh"
#include "systems.hh"
#include "util.hh"

void enemy_think() {
	raylib::Vector2 player_position;

	// Find player position
	auto player_view = registry.view<const Player, const Position>();
	for ( auto [entity, player, position] : player_view.each() ) {
		player_position = position.value;
		break;
	}

	auto view = registry.view<const Enemy, Velocity, const Position, Facing, GunAttack, const Collider>();
	for ( auto [entity, enemy, velocity, position, facing, gun, collider] : view.each() ) {
		gun.timer -= GetFrameTime();

		// Get the distance and direction of the player
		float distance = abs( player_position.x - position.value.x );
		int direction = sign( player_position.x - position.value.x );

		// If the player is in aggro_range, set facing and velocity to move toward them
		if ( distance < enemy.aggro_range ) {
			velocity.value.x = enemy.max_speed * direction;
			facing.direction = direction;
		}

		// If the player if in attack_range and the GunAttack timer <= 0, stop moving and attack them
		if ( distance < enemy.attack_range ) {
			std::cout << gun.timer << '\n';
			velocity.value.x = 0.0;

			if (gun.timer <= 0.0) {
				raylib::Vector2 bullet_start =
					position.value + raylib::Vector2((collider.width/2+0.001)*facing.direction, -collider.height/2);

				const auto bullet = registry.create();
				registry.emplace<Position>( bullet, bullet_start );
				registry.emplace<Velocity>( bullet, raylib::Vector2(gun.speed, 0)*facing.direction );
				registry.emplace<Bullet>( bullet, gun.damage );

				gun.timer = gun.rate;
			}
		}
	}
}
