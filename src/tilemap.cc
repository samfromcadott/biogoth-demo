#include <cmath>

#include "tilemap.hh"

int Tilemap::tile_index(const int x, const int y) const {
	return width * y + x;
}

int Tilemap::tile_index(const TileCoord t) const {
	return width * t.y + t.x;
}

TileCoord Tilemap::tile_coord(const int i) {
	return {i%width, i/width};
}

void Tilemap::draw() {
	for (int y = 0; y < height; y++)
	for (int x = 0; x < width; x++) {
		Tile& t = tiles[ tile_index(x, y) ];
		DrawRectangleLines(x*tile_size, y*tile_size, tile_size, tile_size, SKYBLUE);
		if (t == empty_tile) continue;

		DrawRectangle(x*tile_size, y*tile_size, tile_size, tile_size, GRAY);
	}

}

TileCoord Tilemap::world_to_tile(const Vector2 position) { // Gets the tile coordinate from world coordinate
	int x = floor(position.x/tile_size);
	int y = floor(position.y/tile_size);

	return TileCoord {x, y};
}

Vector2 Tilemap::tile_to_world(const int x, const int y) { // Gets the world coordinate from tile coordinate
	Vector2 v;
	v.x = x*tile_size;
	v.y = y*tile_size;

	return v;
}

Vector2 Tilemap::tile_to_world(const TileCoord t) { // Gets the world coordinate from tile coordinate
	return tile_to_world(t.x, t.y);
}

bool Tilemap::tile_in_map(TileCoord tile) {
	bool horizontal = tile.x >= 0 && tile.x < width;
	bool vertical = tile.y >= 0 && tile.y < height;

	return horizontal && vertical;

}

bool Tilemap::tile_in_map(const int x, const int y) {
	return tile_in_map({x, y});

}

Tile& Tilemap::operator()(const int x, const int y) { // Setter
	return tiles[ tile_index(x, y) ];
}

Tile Tilemap::operator()(const int x, const int y) const { // Getter
	return tiles[ tile_index(x, y) ];
}

Tile& Tilemap::operator()(const TileCoord t) { // Setter
	return tiles[ tile_index(t) ];
}

Tile Tilemap::operator()(const TileCoord t) const { // Getter
	return tiles[ tile_index(t) ];
}
