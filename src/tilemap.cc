#include <cmath>
#include <tileson.hpp>

#include "tilemap.hh"

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

	// Loop through tiles
	std::map<std::tuple<int, int>, tson::Tile*> tileData = terrain->getTileData();
	for (const auto& [id, tile] : tileData) {
		if (tile == nullptr) continue;
		Tile new_tile = tile->getGid();

		// Get tile position
		auto [cx, cy] = id;
		TileCoord coord = {cx, cy};

		// Get texture rect
		// tson::Rect file_rect = tile->getDrawingRect();
		// Rectangle rect = {(float)file_rect.x, (float)file_rect.y, (float)file_rect.width, (float)file_rect.height};
		// new_tile.graphic.x = rect.x / tile_size;
		// new_tile.graphic.y = rect.x / tile_size;

		// Get tile type
		// if (tile->getType() == "plane")
		// 	new_tile.type = PLANE;
		// if (tile->getType() == "water")
		// 	new_tile.type = WATER;


		// Add it to the vector of tiles
		tiles[ tile_index(coord) ] = new_tile;

	}

	// Get the object layer
	// tson::Layer* objectLayer = map->getLayer("objects");
	// if (objectLayer->getType() != tson::LayerType::ObjectGroup) return;
	//
	// for ( auto& object : objectLayer->getObjects() ) {
	// 	tson::Vector2i position = object.getPosition();
	// 	TileCoord coord = {position.x / 16, position.y / 16};
	//
	// 	Unit unit = Unit( File::unit_type_list["soldier"], 0 );
	// 	unit.position = coord;
	//
	// 	unit.id = Match::units.size(); // Index of the unit
	// 	Match::units.push_back( unit );
	// 	tiles[ tile_index(coord) ].unit = unit.id;
	// 	Match::players[0].owned_units.push_back(unit.id);
	// }

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
