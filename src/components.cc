#include <toml.hpp>

#include "components.hh"

void Player::from_toml(const toml::value& v) {
	max_speed = toml::find<float>(v, "max_speed");
	ground_acceleration = toml::find<float>(v, "ground_acceleration");
	air_acceleration = toml::find<float>(v, "air_acceleration");
	ground_deceleration = toml::find<float>(v, "ground_deceleration");
	air_deceleration = toml::find<float>(v, "air_deceleration");
	ground_turn_speed = toml::find<float>(v, "ground_turn_speed");
	air_turn_speed = toml::find<float>(v, "air_turn_speed");
	can_move = toml::find<bool>(v, "can_move");
}

void Enemy::from_toml(const toml::value& v) {
	float aggro_range; // Distance they will charge player
	float attack_range; // Distance they will attack player
	float max_speed;
	bool active;
}

void Gravity::from_toml(const toml::value& v) {
	float scale = 1.0;
}

void Position::from_toml(const toml::value& v) {
	raylib::Vector2 value;
}

void Velocity::from_toml(const toml::value& v) {
	raylib::Vector2 value;
}

void Facing::from_toml(const toml::value& v) {
	int direction;
}

void Collider::from_toml(const toml::value& v) {
	float width, height;
	bool on_floor;
	int wall_direction;
	bool enabled; // Disabled collision with other entities
}

void Health::from_toml(const toml::value& v) {
	int now, max;
}

void DebugColor::from_toml(const toml::value& v) {
	raylib::Color color;
}

void MeleeAttack::from_toml(const toml::value& v) {
	Melee melee;
}

void BiteAttack::from_toml(const toml::value& v) {

}

void GunAttack::from_toml(const toml::value& v) {

}

// void WeaponSet::from_toml(const toml::value& v) {
//
// }

void RayCast::from_toml(const toml::value& v) {
	raylib::Vector2 start, end;
}

void Bullet::from_toml(const toml::value& v) {
	int damage;
}

void Jump::from_toml(const toml::value& v) {
	float speed;
	float gravity_scale;
	float coyote_length; // Max time player can jump after leaving floor
	float buffer_length; // Max time for jump buffer
	float coyote_timer = 0; // Current coyote time
	float buffer_timer = 0; // Current buffer time
	bool can_jump = false;
	bool wish_jump = false;
}

void AnimationState::from_toml(const toml::value& v) {
	Action state;
	Sprite* sprite;
	float timer;
}
