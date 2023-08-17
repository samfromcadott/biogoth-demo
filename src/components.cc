#include <toml.hpp>

#include "components.hh"

void Player::from_toml(const toml::value& v) {
	can_move = toml::find<bool>(v, "can_move");
}

void Enemy::from_toml(const toml::value& v) {
	aggro_range = toml::find<float>(v, "aggro_range");
	attack_range = toml::find<float>(v, "attack_range");
	max_speed = toml::find<float>(v, "max_speed");
	active = true;
}

void Character::from_toml(const toml::value& v) {
	active = true;
	brain = nullptr;
	team = 0;
}

void Movement::from_toml(const toml::value& v) {
	direction = (Vector2){0,0};

	max_speed = toml::find<float>(v, "max_speed");
	ground_acceleration = toml::find<float>(v, "ground_acceleration");
	air_acceleration = toml::find<float>(v, "air_acceleration");
	ground_deceleration = toml::find<float>(v, "ground_deceleration");
	air_deceleration = toml::find<float>(v, "air_deceleration");
	ground_turn_speed = toml::find<float>(v, "ground_turn_speed");
	air_turn_speed = toml::find<float>(v, "air_turn_speed");
	can_move = true;
}

void Gravity::from_toml(const toml::value& v) {
	scale = toml::find<float>(v, "scale");
}

void Position::from_toml(const toml::value& v) {

}

void Velocity::from_toml(const toml::value& v) {
	value = Vector2 {0,0};
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
	auto c = toml::find< std::vector<unsigned char> >(v, "color");

	color = Color {
		c[0],
		c[1],
		c[2],
		c[3]
	};
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
