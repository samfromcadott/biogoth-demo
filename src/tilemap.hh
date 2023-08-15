#pragma once

#include <vector>
#include <string>
#include <raylib.h>
#include <tileson.hpp>

typedef char Tile;
const Tile empty_tile = 0;

struct TileCoord {
	int x, y;
};

enum class LayerType {
	TILE,
	IMAGE,
};

class MapLayer {
private:
	LayerType type;
	std::vector<Tile> tiles;
	raylib::Vector2 parallax, offset, scroll_speed;
	Texture2D texture;
	bool reapeat_x, reapeat_y;
	std::vector<Rectangle> rects; // Vector of drawing rects

	void draw_tile() const;
	void draw_image() const;

public:
	int width, height;
	int tile_size = 32;

	MapLayer() = default;
	MapLayer(const std::string filename, tson::Layer& layer);
	void draw();

	int tile_index(const int x, const int y) const;
	int tile_index(const TileCoord t) const;
	TileCoord tile_coord(const int i) const;

	Tile operator()(const int x, const int y) const;
	Tile operator()(const TileCoord t) const;
};

class Tilemap {
private:
	std::vector<MapLayer> layers;
	int main_layer; // Index of main layer

public:
	int width, height;
	int tile_size = 32;

	int tile_index(const int x, const int y) const;
	int tile_index(const TileCoord t) const;
	TileCoord tile_coord(const int i) const;
	void draw();

	TileCoord world_to_tile(const Vector2 position) const; // Gets the tile coordinate from world coordinate
	TileCoord world_to_tile(float x, float y) const;
	Vector2 tile_to_world(const int x, const int y) const; // Gets the world coordinate from tile coordinate
	Vector2 tile_to_world(const TileCoord t) const;
	bool tile_in_map(TileCoord tile) const; // Returns true if the given TileCoord is in the map
	bool tile_in_map(const int x, const int y) const;

	Tile operator()(const int x, const int y) const; // Getter
	Tile operator()(const TileCoord t) const;

	Tilemap() = default;
	Tilemap(const std::string filename);
	virtual ~Tilemap () {
		// UnloadTexture(texture);
	}
};
