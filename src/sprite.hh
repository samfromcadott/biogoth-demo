#pragma once

#include <array>
#include <string>
#include <map>
#include <memory>
#include <raylib-cpp.hpp>

#include "typedefs.hh"

enum Action {
	IDLE,
	WALK,
	// ATTACK,
	// AIR_ATTACK,
	DIE,
	BITE,
	FALL,
	JUMP,
	WALL_SLIDE,
	WALL_JUMP,

	ATTACK_A, ATTACK_B, ATTACK_C, ATTACK_D, ATTACK_E,
	ATTACK_F, ATTACK_G, ATTACK_H, ATTACK_I, ATTACK_J,
	ATTACK_K, ATTACK_L, ATTACK_M, ATTACK_N, ATTACK_O,
	ATTACK_P, ATTACK_Q, ATTACK_R, ATTACK_S, ATTACK_T,
	ATTACK_U, ATTACK_V, ATTACK_W, ATTACK_X, ATTACK_Y,
	ATTACK_Z,

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
	void render(vec2 position, const Action action, float timer, int direction, float rotation=0.0, float scale=1.0, Color color=WHITE);

	void unload(); // Deletes the texture

	Sprite(){}
	Sprite(std::string filename);
	virtual ~Sprite(){}
};

extern std::map< std::string, Sprite > sprite_list;
