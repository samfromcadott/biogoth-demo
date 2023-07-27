#include <raylib.h>

#include "controls.hh"

// This will be moved to a config file later
const int input_map[8][2] = {
	{KEY_NULL, GAMEPAD_BUTTON_UNKNOWN},

	// Directions
	{KEY_UP, GAMEPAD_BUTTON_LEFT_FACE_UP},
	{KEY_DOWN, GAMEPAD_BUTTON_LEFT_FACE_DOWN},
	{KEY_LEFT, GAMEPAD_BUTTON_LEFT_FACE_LEFT},
	{KEY_RIGHT, GAMEPAD_BUTTON_LEFT_FACE_RIGHT},

	// Actions
	{KEY_SPACE, GAMEPAD_BUTTON_RIGHT_FACE_DOWN},
	{KEY_LEFT_CONTROL, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT},
	{KEY_V, GAMEPAD_BUTTON_RIGHT_FACE_UP},
};

bool command_down(const Command command) {
	return IsKeyDown( input_map[command][0] ) || IsGamepadButtonDown( 0, input_map[command][1] );
}

bool command_pressed(const Command command) {
	return IsKeyPressed( input_map[command][0] ) || IsGamepadButtonPressed( 0, input_map[command][1] );
}

bool command_released(const Command command) {
	return IsKeyReleased( input_map[command][0] ) || IsGamepadButtonReleased( 0, input_map[command][1] );
}
