#pragma once

#include <array>
#include <string>
#include <map>
#include <memory>
#include <raylib-cpp.hpp>

enum Action {
	IDLE,
	WALK,
	ATTACK,
	AIR_ATTACK,
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

public:
	int rate; // Frame rate
	int direction_offset;
	std::array<int, ACTION_COUNT> length; // Length of each action
	std::array<int, ACTION_COUNT> offset; // Offset of each action
	unsigned int width, height; // Size of sprite

	void render(float x, float y, const Action action, float timer, int direction, float rotation=0.0, float scale=1.0, Color color=WHITE);
	void render(raylib::Vector2 position, const Action action, float timer, int direction, float rotation=0.0, float scale=1.0, Color color=WHITE);

	void unload(); // Deletes the texture

	Sprite(){}
	Sprite(std::string filename);
	virtual ~Sprite(){}
};

extern std::map< std::string, Sprite > sprite_list;
