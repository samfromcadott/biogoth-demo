#pragma once

#include <map>
#include <string>
#include <raylib-cpp.hpp>

void load_sound(std::string name);
void play_sound(std::string name, float volume=1.0, float pitch=1.0);
void play_music(); // Updates the music stream
void stop_music();
void set_music(const std::string filename); // Changes the current music
