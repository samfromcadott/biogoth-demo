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
bool player_won;
bool show_help;

Timer death_timer; // Counts down when player dies
Timer help_timer; // Shows help text for limited time
Timer win_timer; // Shows win screen

raylib::Font title_font, normal_font;
raylib::Texture blood_bar;
raylib::Texture intro_screen;
raylib::Texture death_screen;
raylib::Texture outro_screen;

const float G = 32.0;
float game_time = 0.0;

void game_update();
void game_start();

int main() {
	Window window(screen_width, screen_height, "Biogoth - MVP");

	SetTargetFPS(60);

	load_control_config();

	// Load sprites
	sprite_list["guard"] = Sprite("guard");
	sprite_list["vampire"] = Sprite("vampire");
	sprite_list["sprite_test"] = Sprite("sprite_test");
	sprite_list["bullet"] = Sprite("bullet");
	sprite_list["blood"] = Sprite("blood");

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

	// Load UI elements
	blood_bar = raylib::Texture("assets/graphics/ui/blood-bar.png");
	intro_screen = raylib::Texture("assets/graphics/ui/intro.png");
	death_screen = raylib::Texture("assets/graphics/ui/death.png");
	outro_screen = raylib::Texture("assets/graphics/ui/outro.png");

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
	game_time = 0.0;
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
	player_won = false;

	// Start the music
	set_music("assets/audio/music/theme.mp3");
}

void game_update() {
	game_time += GetFrameTime();

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

	// Check for the player getting to the end of the level
	if ( registry.get<Position>(player).value.x > 62000 && !player_won ) {
		player_won = true;
		win_timer = Timer( 2.0, &game_start ); // Restart if the player wins
	}

	if (player_won) win_timer.update();

	stun();
	character_think();
	death_by_pitfall();
	particle_update();

	// Combat
	weapon_update();
	bullets();

	animate_character();

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
