#include <toml.hpp>

#include "sprite.hh"

std::map< std::string, Sprite > sprite_list;

Sprite::Sprite(std::string filename) {
	// texture = raylib::Texture("assets/graphics/sprites/" + filename + ".png");
	texture = LoadTexture( &("assets/graphics/sprites/" + filename + ".png")[0] );
	const auto data  = toml::parse("assets/graphics/sprites/" + filename + ".toml");

	// Sprite properties
	rate = toml::find<int>(data, "rate");
	width = toml::find<int>(data, "width");
	height = toml::find<int>(data, "height");

	// Action lengths
	const auto& file_lengths = toml::find(data, "length");
	length[IDLE] = toml::find<int>(file_lengths, "IDLE");
	// length[WALK] = toml::find<int>(file_lengths, "WALK");

	// Action offsets
	const auto& file_offsets = toml::find(data, "offset");
	offset[IDLE] = toml::find<int>(file_offsets, "IDLE");
	// offset[WALK] = toml::find<int>(file_offsets, "WALK");
}

void Sprite::render(float x, float y, const Action action, float timer, int direction, float rotation) {
	// Find the origin of the frame
	float rx = int(timer*rate) % length[action] * width;
	float ry = action * offset[action] * height;

	// raylib::Rectangle source(rx, ry, width, height);
	// raylib::Rectangle dest(x, y, width, height);
	// raylib::Vector2 origin(width/2, height/2);
	Rectangle source = {rx, ry, float(width), float(height)};
	Rectangle dest = {float(x), float(y), float(width), float(height)};
	Vector2 origin = { float(width/2), float(height/2) };

	// texture.Draw(source, dest, origin, rotation);
	DrawTexturePro(texture, source, dest, origin, rotation, WHITE);
}

void Sprite::render(raylib::Vector2 position, const Action action, float timer, int direction, float rotation) {
	render(position.x, position.y, action, timer, direction, rotation);
}

void Sprite::unload() {
	UnloadTexture(texture);
}
