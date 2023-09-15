#pragma once

#include <vector>
#include <raylib-cpp.hpp>

class CameraSystem {
private:
	inline static raylib::Camera2D camera;
	inline static float zoom, min_zoom, max_zoom;
	inline static float close_distance;
	inline static raylib::Vector2 base, offset;
	inline static float trauma;

	static raylib::Vector2 find_player();
	static raylib::Vector2 track_player();
	static raylib::Vector2 look_ahead();
	static std::vector< raylib::Vector2 > find_close_characters();
	static raylib::Vector2 center_close_characters(const std::vector< raylib::Vector2 >& characters);
	static float zoom_to_characters(const std::vector< raylib::Vector2 >& characters);
	static void clamp_camera();

public:
	static void init();
	static void update();
	static raylib::Camera2D& get_camera();
	CameraSystem() = delete;
};
