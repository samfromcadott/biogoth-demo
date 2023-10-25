#pragma once

#include <vector>
#include <raylib-cpp.hpp>

#include "typedefs.hh"

class CameraSystem {
private:
	inline static raylib::Camera2D camera;
	inline static float zoom, min_zoom, max_zoom;
	inline static float close_distance;
	inline static vec2 base, offset;
	inline static float trauma;

	static vec2 find_player();
	static vec2 track_player();
	static vec2 look_ahead();
	static std::vector< vec2 > find_close_characters();
	static vec2 center_close_characters(const std::vector< vec2 >& characters);
	static float zoom_to_characters(const std::vector< vec2 >& characters);
	static void clamp_camera();

public:
	static void init();
	static void update();
	static raylib::Camera2D& get_camera();
	CameraSystem() = delete;
};
