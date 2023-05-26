#include <iostream>
#include <raylib/raylib-cpp.hpp>

#include "globals.hh"
#include "components.hh"
#include "systems.hh"
#include "tilemap.hh"
#include "level.hh"
#include "sprite.hh"
#include "audio.hh"

using namespace raylib;

int screen_width = 1280;
int screen_height = 720;

entt::registry registry;
Tilemap tilemap(40, 40);
raylib::Camera2D camera( raylib::Vector2(screen_width/2, screen_height/2), raylib::Vector2(0.0, 0.0) );

const float G = 13.0;

int main() {
	Window window(screen_width, screen_height, "raylib-cpp - basic window");

	SetTargetFPS(60);

	tilemap = Tilemap("assets/levels/test.json");

	// Sprite test
	Sprite sprite("sprite_test");
	sprite_list["guard"] = Sprite("guard");
	sprite_list["vampire"] = Sprite("vampire");

	// Load sounds
	InitAudioDevice();
	load_sound("gun");
	load_sound("guard_bitten");
	load_sound("sword_swing");
	load_sound("sword_hit");

	// Get a reference to the player
	auto player_view = registry.view<const Player>();
	entt::entity player;
	for ( auto [entity, p] : player_view.each() ) {
		player = entity;
		break;
	}

	while ( !window.ShouldClose() ) {
		// Player actions
		if ( registry.get<Health>(player).now > 0 ) { // Check is the player is alive
			player_move();
			player_jump();
			jump_buffer();
			player_attack();
			player_bite();
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

		BeginDrawing();
		camera.BeginMode();

			window.ClearBackground( raylib::Color(111, 133, 163, 255) );

			tilemap.draw();

			render_colliders();
			render_collider_sprites();
			render_bullets();

			camera_update();

			// Sprite test
			sprite.render(100, 200, IDLE, GetTime(), +1, GetTime()*180);

		camera.EndMode();

		// UI
		health_bar();

		EndDrawing();

	}

	// Unload sprites
	for (auto& [name, sprite] : sprite_list) {
		sprite.unload();
	}

	return 0;
}
