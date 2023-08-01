#include <toml.hpp>

#include "components.hh"

std::string component_names[] = {
	"Player",
	"Enemy",
	"Gravity",
	"Position",
	"Velocity",
	"Facing",
	"Collider",
	"Health",
	"DebugColor",
	"MeleeAttack",
	"BiteAttack",
	"GunAttack",
	"WeaponSet",
	"RayCast",
	"Bullet",
	"Jump",
	"AnimationState",
};

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
	float aggro_range;
	float attack_range;
	float max_speed;
	bool active;
}

void Gravity::from_toml(const toml::value& v) {
	scale = toml::find<float>(v, "scale");
}

void Position::from_toml(const toml::value& v) {
	raylib::Vector2 value;
}

void Velocity::from_toml(const toml::value& v) {
	raylib::Vector2 value;
}

void Facing::from_toml(const toml::value& v) {
	direction = +1;
}

void Collider::from_toml(const toml::value& v) {
	width = toml::find<int>(v, "width");
	height = toml::find<int>(v, "height");
	on_floor = false;
	wall_direction = 0;
	enabled = true;
}

void Health::from_toml(const toml::value& v) {
	max = toml::find<int>(v, "max");
	now = max;
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
	speed = toml::find<float>(v, "speed");
	gravity_scale = toml::find<float>(v, "gravity_scale");
	coyote_length = toml::find<float>(v, "coyote_length");
	buffer_length = toml::find<float>(v, "buffer_length");
}

void AnimationState::from_toml(const toml::value& v) {
	state = IDLE;
	sprite = &sprite_list[ toml::find<std::string>(v, "sprite") ];
	timer = 0.0f;
}
