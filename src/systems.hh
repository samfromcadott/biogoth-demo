#pragma once

#include "components.hh"

// Player actions
void player_move(); // Gets movement input for player
void player_jump();
void jump_buffer();
void player_attack(); // Checks if player is attacking
void player_bite(); // Checks if player is biting
void player_animate();

// Physics
void move_collide(); // Moves a body and applies collisions
void gravity();
void collider_overlap(); // Pushes colliders apart if they overlap

// Combat
void melee_attack(); // Handles melee attacks
void bite_attack();
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

// UI Elements
void health_bar();
