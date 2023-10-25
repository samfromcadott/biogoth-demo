#pragma once

#include <string>
#include <raylib-cpp.hpp>

#include "typedefs.hh"

void load_entities(); // Loads all entites defined in assets/entities
void spawn_entity(const std::string name, const vec2 position = {0,0}, const int direction = -1);
