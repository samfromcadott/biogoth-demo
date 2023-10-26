#include <iostream>
#include <raylib-cpp.hpp>

#include "typedefs.hh"
#include "globals.hh"
#include "components.hh"
#include "systems.hh"
#include "tilemap.hh"
#include "sprite.hh"
#include "audio.hh"
#include "timer.hh"
#include "controls.hh"
#include "entities.hh"
#include "particle.hh"
#include "camera.hh"

using namespace raylib;

int screen_width = 1280;
int screen_height = 720;

entt::registry registry;
Tilemap tilemap;
raylib::Camera2D camera( vec2(screen_width/2, screen_height/2), vec2(0.0, 0.0) );
entt::entity player; // Reference to the player character entity

bool player_died;
bool show_help;

Timer death_timer; // Counts down when player dies
Timer help_timer; // Shows help text for limited time

raylib::Font title_font, normal_font;

const float G = 13.0;

void game_update();
void game_start();

int main() {
	Window window(screen_width, screen_height, "HEMOGLOBIN - MVP");

	SetTargetFPS(60);

	load_control_config();

	// Load sprites
	sprite_list["guard"] = Sprite("guard");
	sprite_list["vampire"] = Sprite("vampire");
	sprite_list["sprite_test"] = Sprite("sprite_test");
	sprite_list["bullet"] = Sprite("bullet");

	// Load sounds
	InitAudioDevice();
	load_sound("gun");
	load_sound("guard_bitten");
	load_sound("guard_death");
	load_sound("sword_swing");
	load_sound("sword_hit");

	// Load fonts
	title_font = raylib::Font("assets/graphics/fonts/UnifrakturCook-Bold.ttf", 128);
	normal_font = raylib::Font("assets/graphics/fonts/PermanentMarker-Regular.ttf", 128);

	// Load entity definitions
	load_entities();

	game_start();

	// Display help message
	show_help = true;
	help_timer = Timer( 3.0, [](){show_help = false;} ); // Hide help after a few seconds

	while ( !window.ShouldClose() ) {
		game_update();
		render_game(window);
	}

	// Unload sprites
	for (auto& [name, sprite] : sprite_list) {
		sprite.unload();
	}

	return 0;
}

void game_start() {
	registry.clear();

	// Load the level
	tilemap = Tilemap("assets/levels/test.json");

	// Get a reference to the player
	auto player_view = registry.view<const Player>();
	for ( auto [entity, p] : player_view.each() ) {
		player = entity;
		break;
	}

	// Reset the camera
	// camera.target = registry.get<const Position>(player).value;
	CameraSystem::init();
	player_died = false;

	// Start the music
	set_music("assets/audio/music/theme.mp3");
}

void game_update() {
	// Player actions
	if ( registry.get<Health>(player).now > 0 ) { // Check is the player is alive
		// player_move();
		// player_jump();
		jump_buffer();
		// player_attack();
		// player_bite();
	} else if ( !player_died ) {
		player_died = true;
		death_timer = Timer( 1.0, &game_start ); // Restart if the player is dead
	} else { // When player is dead
		death_timer.update();
	}

	stun();
	character_think();
	death_by_pitfall();

	animate_character();
	particle_update();

	// Combat
	weapon_update();
	bullets();

	// Physics
	character_movement();
	gravity();
	collider_overlap();
	move_collide();

	death();

	// Audio
	play_music();

	if ( IsKeyPressed(KEY_R) ) game_start(); // Voluntary reset
	if ( IsKeyPressed(KEY_M) ) stop_music();

	// camera_update();
	CameraSystem::update();

	help_timer.update();
}
