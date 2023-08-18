#include "brain.hh"
#include "globals.hh"
#include "components.hh"
#include "controls.hh"

void PlayerBrain::move() {
	auto& movement = *registry.try_get<Movement>(owner);
	auto& facing = *registry.try_get<Facing>(owner);

	if (!movement.can_move) return;

	int dx = 0; // Input direction
	movement.direction.x = 0;

	if ( command_down(COMMAND_RIGHT) )  dx = +1;
	if ( command_down(COMMAND_LEFT) ) dx = -1;
	if ( dx != 0 ) facing.direction = dx;

	movement.direction.x = dx;
}

void PlayerBrain::jump() {
	// if( command_pressed(COMMAND_JUMP) && player.can_move ) jump.wish_jump = true;
	//
	// // Floor jump
	// if ( jump.wish_jump && collider.on_floor ) {
	// 	velocity.value.y -= jump.speed;
	// 	gravity.scale = jump.gravity_scale;
	// }
	//
	// // Reset gravity scale when going down or on jump release
	// if ( command_released(COMMAND_JUMP) || velocity.value.y > 0.0 ) {
	// 	gravity.scale = 1.0;
	// }
	//
	// // Find if the player is touching a wall
	// collider.wall_direction = 0;
	//
	// TileCoord left_side = tilemap.world_to_tile( position.value.x - collider.width/2 - 4, position.value.y - 1 );
	// TileCoord right_side = tilemap.world_to_tile( position.value.x + collider.width/2 + 4, position.value.y - 1 );
	//
	// if ( tilemap(left_side) != empty_tile ) collider.wall_direction = -1;
	// if ( tilemap(right_side) != empty_tile ) collider.wall_direction = +1;
	//
	// if ( collider.wall_direction != 0 && !collider.on_floor ) facing.direction = -collider.wall_direction;
	//
	// // Wall jump
	// if ( jump.wish_jump && collider.wall_direction != 0 && !collider.on_floor ) {
	// 	velocity.value.y = -1.0;
	// 	velocity.value.x = -collider.wall_direction * 2.0;
	// 	velocity.value = velocity.value.Normalize() * jump.speed;
	// 	gravity.scale = jump.gravity_scale;
	// }
}

void PlayerBrain::attack() {
	auto& weapon_set = *registry.try_get<WeaponSet>(owner);

	if ( !command_down(COMMAND_ATTACK) ) return;
	if (weapon_set[0]->active) return; // Don't attack if the player is already attacking
	// if (!player.can_move) return;

	weapon_set[0]->fire();
}

void PlayerBrain::bite() {
	auto& weapon_set = *registry.try_get<WeaponSet>(owner);

	if ( command_pressed(COMMAND_BITE) )
		weapon_set[1]->fire();

	if ( command_released(COMMAND_BITE) )
		weapon_set[1]->end();
}

void PlayerBrain::think() {
	move();
	jump();
	attack();
	bite();
}

PlayerBrain::PlayerBrain(entt::entity owner) {
	this->owner = owner;
}

PlayerBrain::~PlayerBrain() {

}
