#include <string>
#include <iostream>
#include <raylib.h>
#include <toml.hpp>

// The default range for Magic Enum is [-128, 128]. Keys outside that range aren't read correctly
#define MAGIC_ENUM_RANGE_MIN 0
#define MAGIC_ENUM_RANGE_MAX 1024
#include <magic_enum.hpp>

#include "controls.hh"

int input_map[COMMAND_COUNT][2];
const int KEYBOARD = 0;
const int CONTROLLER = 1;

void load_control_config() {
	std::cout << "Loading config.cfg" << '\n';
	const auto data = toml::parse("config.cfg");

	const auto& keyboard = toml::find(data, "Keyboard");
	const auto& controller = toml::find(data, "Controller");

	// Load config for keyboard
	for (int command = COMMAND_NONE + 1; command < COMMAND_COUNT; command++) {
		// Get the string of command enum
		auto command_name = std::string{ magic_enum::enum_name( Command(command) ) };

		// Check if the command is defined in the file
		if ( !keyboard.contains( command_name ) ) continue;

		// Get the key defing for this command
		std::string key_name = toml::find<std::string>( keyboard, command_name );

		KeyboardKey key = magic_enum::enum_cast<KeyboardKey>(key_name).value_or(KEY_NULL);
		input_map[command][KEYBOARD] = key;
	}

	// Repeat for the controller
	for (int command = COMMAND_NONE + 1; command < COMMAND_COUNT; command++) {
		auto command_name = std::string{ magic_enum::enum_name( Command(command) ) };

		if ( !controller.contains( command_name ) ) continue;

		std::string button_name = toml::find<std::string>( controller, command_name );

		GamepadButton button = magic_enum::enum_cast<GamepadButton>(button_name).value_or(GAMEPAD_BUTTON_UNKNOWN);
		input_map[command][CONTROLLER] = button;
	}
}

bool command_down(const Command command) {
	return IsKeyDown( input_map[command][KEYBOARD] ) ||
		IsGamepadButtonDown( 0, input_map[command][CONTROLLER] );
}

bool command_pressed(const Command command) {
	return IsKeyPressed( input_map[command][KEYBOARD] ) ||
		IsGamepadButtonPressed( 0, input_map[command][CONTROLLER] );
}

bool command_released(const Command command) {
	return IsKeyReleased( input_map[command][KEYBOARD] ) ||
		IsGamepadButtonReleased( 0, input_map[command][CONTROLLER] );
}
