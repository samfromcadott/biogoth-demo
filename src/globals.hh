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
