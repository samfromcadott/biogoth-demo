#pragma once

#include <entt/entt.hpp>
#include <raylib-cpp.hpp>

#include "typedefs.hh"
#include "tilemap.hh"

extern entt::registry registry;
extern Tilemap tilemap;
extern raylib::Camera2D camera;

extern const float G; // Gravity acceleration

extern int screen_width;
extern int screen_height;

extern entt::entity player;
extern bool player_died;
extern bool player_won;
extern bool show_help;

extern raylib::Font title_font;
extern raylib::Font normal_font;
extern raylib::Texture blood_bar;
extern raylib::Texture intro_screen;
extern raylib::Texture death_screen;
extern raylib::Texture outro_screen;

extern float game_time; // Time since the last restart
