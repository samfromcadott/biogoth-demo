#include <iostream>
#include <raylib/raylib-cpp.hpp>

#include "globals.hh"
#include "components.hh"
#include "systems.hh"
#include "tilemap.hh"
#include "level.hh"
#include "sprite.hh"
#include "audio.hh"
#include "timer.hh"

using namespace raylib;

int screen_width = 1280;
int screen_height = 720;

entt::registry registry;
Tilemap tilemap(40, 40);
raylib::Camera2D camera( raylib::Vector2(screen_width/2, screen_height/2), raylib::Vector2(0.0, 0.0) );
entt::entity player; // Reference to the player character entity
bool player_died;

Timer death_timer; // Counts down when player dies

const float G = 13.0;

void game_update();
void game_start();

int main() {
	Window window(screen_width, screen_height, "raylib-cpp - basic window");

	SetTargetFPS(60);

	// Load sprites
	sprite_list["guard"] = Sprite("guard");
	sprite_list["vampire"] = Sprite("vampire");

	// Load sounds
	InitAudioDevice();
	load_sound("gun");
	load_sound("guard_bitten");
	load_sound("guard_death");
	load_sound("sword_swing");
	load_sound("sword_hit");

	game_start();

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
	camera.target = registry.get<const Position>(player).value;
	player_died = false;
}

void game_update() {
	// Player actions
	if ( registry.get<Health>(player).now > 0 ) { // Check is the player is alive
		player_move();
		player_jump();
		jump_buffer();
		player_attack();
		player_bite();
	} else if ( !player_died ) {
		player_died = true;
		death_timer = Timer( 1.0, game_start ); // Restart if the player is dead
	} else { // When player is dead
		death_timer.update();
	}

	player_animate();

	// Enemy actions
	enemy_think();

	// Combat
	melee_attack();
	bite_attack();
	bullets();

	// Physics
	gravity();
	collider_overlap();
	move_collide();

	death();

	if ( IsKeyPressed(KEY_R) ) game_start(); // Voluntary reset

	camera_update();
}
