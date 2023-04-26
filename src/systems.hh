#pragma once

#include "components.hh"

void player_move(); // Gets movement input for player
void player_jump();
void move_collide(); // Moves a body and applies collisions
void gravity();
void death();
void render_colliders();
void collider_overlap(); // Pushes colliders apart if they overlap
void player_attack(); // Checks if player is attacking
void player_bite(); // Checks if player is biting
void melee_attack(); // Handles melee attacks
void bite_attack();
void bullets(); // Updates bullets
void render_bullets();
void enemy_think();
void camera_update();

// UI Elements
void health_bar();
