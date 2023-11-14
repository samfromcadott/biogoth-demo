#include <iostream>
#include <cmath>
#include <raylib-cpp.hpp>

#include "globals.hh"
#include "components.hh"
#include "systems.hh"
#include "camera.hh"
#include "util.hh"

void camera_update() {
	float map_width = tilemap.width * tilemap.tile_size;
	float map_height = tilemap.height * tilemap.tile_size;

	auto view = registry.view<const Player, const Position, const Velocity>();
	for ( auto [entity, player, position, velocity] : view.each() ) {
		// Scale of camera movement
		float sx = 0.8;
		float sy = 0.3;

		// Track the player faster when near top or bottom
		if (
			position.value.y < camera.target.y - screen_height/4 ||
			position.value.y > camera.target.y + screen_height/4
		) sy = 1.0;

		vec2 look_ahead(96.0, 32.0);
		vec2 target = position.value + (velocity.value * look_ahead); // Look ahead

		camera.target.x += (target.x - camera.target.x) * sx * GetFrameTime();
		camera.target.y += (target.y - camera.target.y) * sy * GetFrameTime();

		// Restrict camera to map
		if (camera.target.x < camera.offset.x) camera.target.x = camera.offset.x; // Left edge
		if (camera.target.x > map_width - camera.offset.x) camera.target.x = map_width - camera.offset.x; // Right edge
		if (camera.target.y < camera.offset.y) camera.target.y = camera.offset.y; // Top edge
		if (camera.target.y > map_height - camera.offset.y) camera.target.y = map_height - camera.offset.y; // Bottom edge
	}
}

vec2 CameraSystem::find_player() {
	return registry.get<Position>(player).value;
}

vec2 CameraSystem::track_player() {
	return find_player() - base;
}

vec2 CameraSystem::look_ahead() {
	vec2 scale(96.0, 32.0);
	return registry.get<Velocity>(player).value * scale;
}

std::vector< vec2 > CameraSystem::find_close_characters() {
	std::vector< vec2 > character_list;

	auto view = registry.view<const Character, const Position>();
	for ( auto [entity, character, position] : view.each() ) {
		if (!character.active) continue;
		if ( position.value.Distance( find_player() ) < close_distance )
			character_list.push_back( position.value );
	}

	return character_list;
}

/// Find the average of all characters near the player
vec2 CameraSystem::center_close_characters(const std::vector< vec2 >& characters) {
	vec2 sum;

	for ( const auto& v : characters ) sum += v;

	return ( sum / characters.size() ) - base;
}

/// Zooms to show all nearby characters
float CameraSystem::zoom_to_characters(const std::vector< vec2 >& characters) {
	std::vector<float> values_x, values_y;

	for (auto p : characters) {
		values_x.push_back(p.x);
		values_y.push_back(p.y);
	}

	// Get the maximum distance of the characters
	float min_x = *std::min_element( values_x.begin(), values_x.end() );
	float min_y = *std::min_element( values_y.begin(), values_y.end() );

	float max_x = *std::max_element( values_x.begin(), values_x.end() );
	float max_y = *std::max_element( values_y.begin(), values_y.end() );

	float distance = Vector2Distance( {min_x, min_y}, {max_x, max_y} );

	float new_zoom = ease( distance / (close_distance * 2), max_zoom, min_zoom );
	return new_zoom - zoom;
}

/// Calculates camera shake and modifies offset
void CameraSystem::shake() {
	const float shake_scale = 4096.0;

	trauma = Clamp(trauma, 0.0, 1.0);

	offset.x += pow(trauma, 2) * ((float)rand() / (float)RAND_MAX - 0.5) * shake_scale;
	offset.y += pow(trauma, 2) * ((float)rand() / (float)RAND_MAX - 0.5) * shake_scale;

	trauma -= 1.0 * GetFrameTime();
	offset *= 0.9 * GetFrameTime();
}

void CameraSystem::clamp_camera() {
	const float map_width = tilemap.width * tilemap.tile_size;
	const float map_height = tilemap.height * tilemap.tile_size;

	float z = 1.0 / zoom;

	if (camera.target.x < camera.offset.x * z)
		camera.target.x = camera.offset.x * z; // Left edge
	else if ( camera.target.x > map_width - camera.offset.x * z )
		camera.target.x = map_width - camera.offset.x * z; // Right edge
	if (camera.target.y < camera.offset.y * z)
		camera.target.y = camera.offset.y * z; // Top edge
	else if ( camera.target.y > map_height - camera.offset.y * z )
		camera.target.y = map_height - camera.offset.y * z; // Bottom edge
}

void CameraSystem::init() {
	zoom = 1.0;
	max_zoom = 1.0;
	min_zoom = 0.8;
	close_distance = 800.0;

	camera = raylib::Camera2D( vec2(screen_width/2, screen_height/2), {0.0, 0.0} );
	base = find_player();
	offset = vec2(0, 0);
}

void CameraSystem::update() {
	const auto characters = find_close_characters();

	vec2 delta =
		track_player() * 0.8 +
		look_ahead() * 0.6 +
		center_close_characters(characters) * 0.8;
	float delta_zoom = zoom_to_characters(characters);

	base += delta * GetFrameTime();
	zoom += delta_zoom * GetFrameTime();

	shake();

	camera.target = base + offset;
	camera.zoom = zoom;
	clamp_camera();
}

raylib::Camera2D& CameraSystem::get_camera() {
	return camera;
}
