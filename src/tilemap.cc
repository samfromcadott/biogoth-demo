#include <cmath>
#include <iostream>
#include <tileson.hpp>

#include "tilemap.hh"
#include "level.hh"

Tilemap::Tilemap(const std::string filename) {
	// FileData map_file_data = File::open(filename);

	tson::Tileson t;
	std::unique_ptr<tson::Map> map = t.parse( fs::path(filename) );

	if (map->getStatus() != tson::ParseStatus::OK) return;

	// Get the terrain layer
	tson::Layer* terrain = map->getLayer("Main");
	if (terrain->getType() != tson::LayerType::TileLayer) return;

	width = terrain->getSize().x;
	height = terrain->getSize().y;
	tiles.resize(width*height);

	// Setup the rectangle vector
	rects.resize(18);

	// Load the texture
	this->texture = LoadTexture("assets/graphics/tilesets/bricks.png");

	// Loop through tiles
	std::map<std::tuple<int, int>, tson::Tile*> tile_data = terrain->getTileData();
	for (const auto& [id, tile] : tile_data) {
		if (tile == nullptr) continue;
		Tile new_tile = tile->getGid();

		// Get tile position
		auto [cx, cy] = id;
		TileCoord coord = {cx, cy};

		// Add it to the vector of tiles
		tiles[ tile_index(coord) ] = new_tile;

		// Add the drawing rectangle to the rects vector
		auto file_rect = tile->getDrawingRect();
		rects[new_tile].x = file_rect.x;
		rects[new_tile].y = file_rect.y;
		rects[new_tile].width = file_rect.width;
		rects[new_tile].height = file_rect.height;
	}

	// Get the object layer
	tson::Layer* object_layer = map->getLayer("Objects");
	if (object_layer->getType() != tson::LayerType::ObjectGroup) return;

	for ( auto& object : object_layer->getObjects() ) {
		tson::Vector2i position = object.getPosition();
		std::string type = object.getType();

		if (type == "Player") make_player(position.x, position.y, +1);
		else if (type == "Enemy") make_enemy(position.x, position.y, +1);
	}
}

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
		// DrawRectangleLines(x*tile_size, y*tile_size, tile_size, tile_size, SKYBLUE);
		if (t == empty_tile) continue;

		// DrawRectangle(x*tile_size, y*tile_size, tile_size, tile_size, GRAY);
		DrawTexturePro(
			texture,
			rects[t],
			{ float(x*tile_size), float(y*tile_size), float(tile_size), float(tile_size) },
			{0.0, 0.0},
			0.0,
			WHITE
		);
	}

}

TileCoord Tilemap::world_to_tile(const Vector2 position) { // Gets the tile coordinate from world coordinate
	int x = floor(position.x/tile_size);
	int y = floor(position.y/tile_size);

	return TileCoord {x, y};
}

TileCoord Tilemap::world_to_tile(float x, float y) {
	return world_to_tile( Vector2 {x, y} );
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
