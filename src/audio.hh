#pragma once

#include <map>
#include <string>
#include <raylib/raylib-cpp.hpp>

void load_sound(std::string name);
void play_sound(std::string name, float volume=1.0, float pitch=1.0);
