#include <cmath>
#include <iostream>
#include <tileson.hpp>

#include "globals.hh"
#include "tilemap.hh"
#include "entities.hh"

Tilemap::Tilemap(const std::string filename) {
	// FileData map_file_data = File::open(filename);

	tson::Tileson t;
	std::unique_ptr<tson::Map> map = t.parse( fs::path(filename) );

	if (map->getStatus() != tson::ParseStatus::OK) return;

	// Loop over layers
	auto file_layers = map->getLayers();
	for (auto layer = file_layers.begin(); layer != file_layers.end(); ++layer) {
		// Check if the map is a tile or image map
		if ( layer->getType() != tson::LayerType::TileLayer && layer->getType() != tson::LayerType::ImageLayer )
			continue;

		layers.push_back( MapLayer(filename, *layer) ); // Add the layer to the map

		// If the layer is named "Main" then set its index as the main layer
		if ( layer->getName() == "Main" )
			main_layer = std::distance( file_layers.begin(), layer );
	}

	width = layers[main_layer].width;
	height = layers[main_layer].height;

	// Get the object layer
	tson::Layer* object_layer = map->getLayer("Objects");
	if (object_layer->getType() != tson::LayerType::ObjectGroup) return;

	for ( auto& object : object_layer->getObjects() ) {
		tson::Vector2i position = object.getPosition();
		std::string type = object.getType();

		spawn_entity(type, {(float)position.x, (float)position.y});
	}
}

int Tilemap::tile_index(const int x, const int y) const {
	return width * y + x;
}

int Tilemap::tile_index(const TileCoord t) const {
	return width * t.y + t.x;
}

TileCoord Tilemap::tile_coord(const int i) const {
	return {i%width, i/width};
}

void Tilemap::draw() const {
	for (auto& layer : layers) layer.draw();
}

TileCoord Tilemap::world_to_tile(const Vector2 position) const { // Gets the tile coordinate from world coordinate
	int x = floor(position.x/tile_size);
	int y = floor(position.y/tile_size);

	return TileCoord {x, y};
}

TileCoord Tilemap::world_to_tile(float x, float y) const {
	return world_to_tile( Vector2 {x, y} );
}

Vector2 Tilemap::tile_to_world(const int x, const int y) const { // Gets the world coordinate from tile coordinate
	Vector2 v;
	v.x = x*tile_size;
	v.y = y*tile_size;

	return v;
}

Vector2 Tilemap::tile_to_world(const TileCoord t) const { // Gets the world coordinate from tile coordinate
	return tile_to_world(t.x, t.y);
}

bool Tilemap::tile_in_map(TileCoord tile) const {
	bool horizontal = tile.x >= 0 && tile.x < width;
	bool vertical = tile.y >= 0 && tile.y < height;

	return horizontal && vertical;

}

bool Tilemap::tile_in_map(const int x, const int y) const {
	return tile_in_map({x, y});

}

Tile Tilemap::operator()(const int x, const int y) const {
	return layers[main_layer](x, y);
}

Tile Tilemap::operator()(const TileCoord t) const {
	return layers[main_layer](t);
}

void MapLayer::draw_tile() const {
	for (int y = 0; y < height; y++)
	for (int x = 0; x < width; x++) {
		const Tile& t = tiles[ tile_index(x, y) ];
		if (t == empty_tile) continue;

		Rectangle dest = {
			offset.x + (parallax.x * camera.target.x) + (x * tile_size),
			offset.y + (parallax.y * camera.target.y) + (y * tile_size),
			float(tile_size),
			float(tile_size)
		};

		DrawTexturePro(
			texture,
			rects[t],
			dest,
			{0.0, 0.0},
			0.0,
			WHITE
		);
	}
}

void MapLayer::draw_image() const {
	Vector2 origin = {
		camera.target.x - float(GetScreenWidth() / 2),
		camera.target.y - float(GetScreenHeight() / 2)
	};

	Rectangle source = {
		offset.x + (parallax.x * origin.x),
		offset.y + (parallax.y * origin.y),
		(float)GetScreenWidth(),
		(float)GetScreenHeight()
	};

	Rectangle dest = {
		origin.x,
		origin.y,
		(float)GetScreenWidth(),
		(float)GetScreenHeight()
	};

	DrawTexturePro(
		texture,
		source,
		dest,
		(Vector2) { 0, 0 },
		0,
		WHITE
	);
}

MapLayer::MapLayer(const std::string filename, const tson::Layer& layer) {
	auto map_path = filename.substr( 0, filename.find_last_of("\\/")+1 );

	parallax.x = layer.getParallax().x;
	parallax.y = layer.getParallax().y;

	offset.x = layer.getOffset().x;
	offset.y = layer.getOffset().y;

	if ( layer.getType() == tson::LayerType::ImageLayer ) {
		type = LayerType::IMAGE;
		this->texture = LoadTexture( ( map_path + layer.getImage() ).c_str() );
		return;
	}

	type = LayerType::TILE;

	// Invert parallax for tile layers
	parallax.x = 1.0 - layer.getParallax().x;
	parallax.y = 1.0 - layer.getParallax().y;

	// Set the size
	width = layer.getSize().x;
	height = layer.getSize().y;
	tiles.resize(width*height);

	// Setup the rectangle vector
	rects.resize(19);

	// Loop through tiles
	bool image_loaded = false;

	std::map<std::tuple<int, int>, tson::Tile*> tile_data = layer.getTileData();
	for (const auto& [id, tile] : tile_data) {
		if (tile == nullptr) continue;

		// Load the tileset texture
		if (!image_loaded) {
			auto texture_path = map_path + tile->getTileset()->getImage().generic_string();
			this->texture = LoadTexture( texture_path.c_str() );
			image_loaded = true;
		}

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
}

void MapLayer::draw() const {
	if (type == LayerType::TILE) draw_tile();
	else if (type == LayerType::IMAGE) draw_image();
}

int MapLayer::tile_index(const int x, const int y) const {
	return width * y + x;
}

int MapLayer::tile_index(const TileCoord t) const {
	return width * t.y + t.x;
}

TileCoord MapLayer::tile_coord(const int i) const {
	return {i%width, i/width};
}
Tile MapLayer::operator()(const int x, const int y) const { // Getter
	return tiles[ tile_index(x, y) ];
}
Tile MapLayer::operator()(const TileCoord t) const { // Getter
	return tiles[ tile_index(t) ];
}
