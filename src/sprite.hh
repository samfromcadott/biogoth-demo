#pragma once

#include <array>
#include <string>
#include <map>
#include <memory>
#include <raylib/raylib-cpp.hpp>

enum Action {
	IDLE,
	WALK,
	ATTACK,
	DIE,
	BITE,
	FALL,
	JUMP,
	WALL_SLIDE,
	WALL_JUMP,
	ACTION_COUNT
};

class Sprite {
private:
	Texture2D texture;
	int rate; // Frame rate
	std::array<int, ACTION_COUNT> length; // Length of each action
	std::array<int, ACTION_COUNT> offset; // Offset of each action

public:
	unsigned int width, height; // Size of sprite

	void render(float x, float y, const Action action, float timer, int direction, float rotation=0.0);
	void render(raylib::Vector2 position, const Action action, float timer, int direction, float rotation=0.0);

	void unload(); // Deletes the texture

	Sprite(){}
	Sprite(std::string filename);
	virtual ~Sprite(){}
};

extern std::map< std::string, Sprite > sprite_list;
