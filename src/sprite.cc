#include <toml.hpp>
#include <iostream>

#include "sprite.hh"

std::map< std::string, Sprite > sprite_list;

Sprite::Sprite(std::string filename) {
	// Initialize length and offset
	length.fill(1);
	offset.fill(0);

	// Load the texture
	texture = LoadTexture( &("assets/graphics/sprites/" + filename + ".png")[0] );
	const auto data  = toml::parse("assets/graphics/sprites/" + filename + ".toml");

	// Sprite properties
	rate = toml::find<int>(data, "rate");
	width = toml::find<int>(data, "width");
	height = toml::find<int>(data, "height");
	if ( data.contains("direction_offset") ) direction_offset =  toml::find<int>(data, "direction_offset");

	const auto& file_lengths = toml::find(data, "length"); // Action lengths
	const auto& file_offsets = toml::find(data, "offset"); // Action offsets

	// Names of actions
	static const std::vector<std::string> action_names = {
		"IDLE",
		"WALK",
		"ATTACK",
		"AIR_ATTACK",
		"DIE",
		"BITE",
		"FALL",
		"JUMP",
		"WALL_SLIDE",
		"WALL_JUMP"
	};

	// Get data for actions
	for (int action = IDLE; action < ACTION_COUNT; action++) {
		// Check if the action is defined in the file
		if ( !file_lengths.contains( action_names[action] ) ) continue;
		if ( !file_offsets.contains( action_names[action] ) ) continue;

		length[action] = toml::find<int>( file_lengths, action_names[action] );
		offset[action] = toml::find<int>( file_offsets, action_names[action] );
	}
}

void Sprite::render(float x, float y, const Action action, float timer, int direction, float rotation, float scale, Color color) {
	// Use direction_offset if the sprite is facing left
	int off = 0;
	if (direction == -1) off = direction_offset;

	// Find the origin of the frame
	float rx = int(timer*rate) % length[action] * width;
	float ry = (offset[action] + off) * height;

	Rectangle source = {rx, ry, float(width), float(height)};
	Rectangle dest = {float(x), float(y), float(width)*scale, float(height)*scale};
	Vector2 origin = { float(width/2)*scale, float(height/2)*scale };

	// texture.Draw(source, dest, origin, rotation);
	DrawTexturePro(texture, source, dest, origin, rotation, color);
}

void Sprite::render(vec2 position, const Action action, float timer, int direction, float rotation, float scale, Color color) {
	render(position.x, position.y, action, timer, direction, rotation, scale, color);
}

void Sprite::unload() {
	UnloadTexture(texture);
}
