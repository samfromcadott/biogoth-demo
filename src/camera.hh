#pragma once

#include <vector>
#include <raylib-cpp.hpp>

class CameraSystem {
private:
	inline static raylib::Camera2D camera;
	inline static float min_zoom, max_zoom;
	inline static raylib::Vector2 base, offset;
	inline static float trauma;

	static raylib::Vector2 find_player();
	static raylib::Vector2 track_player();
	static raylib::Vector2 look_ahead();
	static std::vector< raylib::Vector2 > find_close_characters();
	static raylib::Vector2 center_close_characters();
	static void clamp_camera();

public:
	static void init();
	static void update();
	static raylib::Camera2D& get_camera();
	CameraSystem() = delete;
};
