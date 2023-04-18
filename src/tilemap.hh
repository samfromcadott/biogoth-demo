#pragma once

#include <vector>
#include <raylib.h>

typedef char Tile;
const Tile empty_tile = 0;

struct TileCoord {
	int x, y;
};

class Tilemap {
private:
	std::vector<Tile> tiles;

public:
	int width, height;
	int tile_size;

	int tile_index(const int x, const int y) const;
	int tile_index(const TileCoord t) const;
	TileCoord tile_coord(const int i);
	void draw();

	TileCoord world_to_tile(const Vector2 position); // Gets the tile coordinate from world coordinate
	Vector2 tile_to_world(const int x, const int y); // Gets the world coordinate from tile coordinate
	Vector2 tile_to_world(const TileCoord t);
	bool tile_in_map(TileCoord tile); // Returns true if the given TileCoord is in the map

	Tile& operator()(const int x, const int y); // Setter
	Tile operator()(const int x, const int y) const; // Getter
	Tile& operator()(const TileCoord t);
	Tile operator()(const TileCoord t) const;

	Tilemap (const int width, const int height);
	virtual ~Tilemap ();
};
