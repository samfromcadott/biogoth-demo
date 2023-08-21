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
	auto& jump = *registry.try_get<Jump>(owner);
	auto& collider = *registry.try_get<Collider>(owner);
	auto& position = *registry.try_get<Position>(owner);
	auto& velocity = *registry.try_get<Velocity>(owner);
	auto& facing = *registry.try_get<Facing>(owner);
	auto& gravity = *registry.try_get<Gravity>(owner);

	if ( command_pressed(COMMAND_JUMP) ) jump.wish_jump = true;

	// Floor jump
	if ( jump.wish_jump && collider.on_floor ) {
		velocity.value.y -= jump.speed;
		gravity.scale = jump.gravity_scale;
	}

	// Reset gravity scale when going down or on jump release
	if ( command_released(COMMAND_JUMP) || velocity.value.y > 0.0 ) {
		gravity.scale = 1.0;
	}

	// Find if the player is touching a wall
	collider.wall_direction = 0;

	TileCoord left_side = tilemap.world_to_tile( position.value.x - collider.width/2 - 4, position.value.y - 1 );
	TileCoord right_side = tilemap.world_to_tile( position.value.x + collider.width/2 + 4, position.value.y - 1 );

	if ( tilemap(left_side) != empty_tile ) collider.wall_direction = -1;
	if ( tilemap(right_side) != empty_tile ) collider.wall_direction = +1;

	if ( collider.wall_direction != 0 && !collider.on_floor ) facing.direction = -collider.wall_direction;

	// Wall jump
	if ( jump.wish_jump && collider.wall_direction != 0 && !collider.on_floor ) {
		velocity.value.y = -1.0;
		velocity.value.x = -collider.wall_direction * 2.0;
		velocity.value = velocity.value.Normalize() * jump.speed;
		gravity.scale = jump.gravity_scale;
	}
}

void PlayerBrain::attack() {
	auto& weapon_set = *registry.try_get<WeaponSet>(owner);
	auto& weapon_map = *registry.try_get<WeaponMap>(owner);
	auto& collider = *registry.try_get<Collider>(owner);

	if ( !command_down(COMMAND_ATTACK) ) return;
	if (weapon_set[0]->active) return; // Don't attack if the player is already attacking
	// if (!player.can_move) return;

	// Check input direction
	AttackModifier modifier = AttackModifier::NONE;
	if ( command_down(COMMAND_LEFT) || command_down(COMMAND_RIGHT) ) modifier = AttackModifier::SIDE;
	else if ( command_down(COMMAND_UP) ) modifier = AttackModifier::UP;
	else if ( command_down(COMMAND_DOWN) ) modifier = AttackModifier::DOWN;

	// Determine air state
	AirState air_state = collider.on_floor? AirState::ON_FLOOR : AirState::IN_AIR;

	// Get the index of the weapon
	WeaponInput input = {modifier, air_state};
	size_t index = weapon_map.count(input)? weapon_map[input] : weapon_map[{AttackModifier::NONE, AirState::ON_FLOOR}];

	weapon_set[index]->fire();
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
