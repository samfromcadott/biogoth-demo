#pragma once

enum Command {
	COMMAND_NONE = 0,

	// Directions
	COMMAND_UP,
	COMMAND_DOWN,
	COMMAND_LEFT,
	COMMAND_RIGHT,

	// Actions
	COMMAND_JUMP,
	COMMAND_ATTACK,
	COMMAND_BITE,

	COMMAND_COUNT
};

void load_control_config();

bool command_down(const Command command);
bool command_pressed(const Command command);
bool command_released(const Command command);
