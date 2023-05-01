#pragma once

#include <array>
#include <string>
#include <raylib/raylib-cpp.hpp>

enum Action {
	IDLE,
	WALK,
	ATTACK,
	BITE,
	DIE,
	ACTION_COUNT
};

class Sprite {
private:
	raylib::Texture texture;
	int rate; // Frame rate
	std::array<int, ACTION_COUNT> lengths; // Length of each action

public:
	unsigned int width, height; // Size of sprite

	void render(float x, float y, const Action action, float timer, int direction, float rotation=0.0);
	void render(raylib::Vector2 position, const Action action, float timer, int direction, float rotation=0.0);

	Sprite(){}
	Sprite(std::string filename);
	virtual ~Sprite(){}
};
