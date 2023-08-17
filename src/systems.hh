#pragma once

#include "components.hh"

// Player actions
void player_move(); // Gets movement input for player
void player_jump();
void jump_buffer();
void player_attack(); // Checks if player is attacking
void player_bite(); // Checks if player is biting

// Physics
void character_movement();
void move_collide(); // Moves a body and applies collisions
void gravity();
void collider_overlap(); // Pushes colliders apart if they overlap

// Combat
void weapon_update(); // Runs the update function for all weapons
void bullets(); // Updates bullets

// Enemies
void enemy_think();

// General
void death();
void camera_update();

// Render
void render_colliders();
void render_bullets();
void render_collider_sprites();
void render_game(raylib::Window& window);
void animate_character();

// UI Elements
void health_bar();
void help_text();
void death_text();
