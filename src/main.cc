#include <raylib/raylib-cpp.hpp>

using namespace raylib;

int main() {
	int screen_width = 1280;
	int screen_height = 720;

	Window window(screen_width, screen_height, "raylib-cpp - basic window");

	SetTargetFPS(60);

	raylib::Rectangle player_rect(100, 100, 64, 128);
	raylib::Rectangle floor_rect(200, 200, 128, 64);

	raylib::Vector2 player_velocity(0.0, 0.0);

	while ( !window.ShouldClose() ) {
		player_velocity = raylib::Vector2(0.0, 0.0);
		const float speed = 5.0;

		if ( IsKeyDown(KEY_RIGHT))  player_velocity.x += speed;
		if ( IsKeyDown(KEY_LEFT) ) player_velocity.x -= speed;
		if ( IsKeyDown(KEY_UP) ) player_velocity.y -= speed;
		if ( IsKeyDown(KEY_DOWN) ) player_velocity.y += speed;

		player_rect.SetPosition( player_rect.GetPosition() + player_velocity );

		BeginDrawing();

			window.ClearBackground(raylib::RAYWHITE);

			player_rect.Draw(raylib::VIOLET);
			floor_rect.Draw(raylib::GRAY);

		EndDrawing();
	}

	return 0;
}
