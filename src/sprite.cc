#include "sprite.hh"

Sprite::Sprite(std::string filename) {
	texture = raylib::Texture(filename);
	rate = 1;
	width = 32;
	height = 32;
	lengths[IDLE] = 2;
	offset[IDLE] = 0;
	lengths[WALK] = 2;
	offset[WALK] = 1;
}

void Sprite::render(float x, float y, const Action action, float timer, int direction, float rotation) {
	// Find the origin of the frame
	int rx = int(timer*rate) % lengths[action] * width;
	int ry = action * offset[action] * height;

	raylib::Rectangle source(rx, ry, width, height);
	raylib::Rectangle dest(x, y, width, height);
	raylib::Vector2 origin(width/2, height/2);

	texture.Draw(source, dest, origin, rotation);
}

void Sprite::render(raylib::Vector2 position, const Action action, float timer, int direction, float rotation) {
	render(position.x, position.y, action, timer, direction, rotation);
}
