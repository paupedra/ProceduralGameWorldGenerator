#pragma once

struct Tile
{
	int tileId = 0; //Represents the type of tile
	int r, g, b = 0; //Used in testing, will usually be a texture
	int biomeId = 0;
};

struct Biome
{
	int id = 0;
	float biomeSurfaceVariation = 0; //Specific Biome Surface Variation
};

struct WorldInfoFrontSideView2D
{
	Tile tiles[1]; //Tiles present in the world

	int width, height = 0; //Width and Height of the map in tiles

	int surfaceAverageHeight = 0; //Average height of the terrain, counting from bottom
	float surfaceVariation = 0; //Variation in tiles of the surface, this will create higher or lower surface variation

	Biome biomes[1];


};

struct WorldInfoTopView2D
{
	Tile** tiles = nullptr; //Tiles present in the world

	int width, height = 0;

	Biome biomes[1];

	int waterPercent = 50; //In percentage from 0 to 99 the percentage of the world which will be bodies of water


};

//void AddTile(WorldInfoFrontSideView2D& info, Tile& tile)
//{
//
//};

void GenerateFrontSideView2DWorld(WorldInfoFrontSideView2D& worldInfo, int seed); //Generates 2D world from a Front-Side View

void GenerateTowView2DWorld(WorldInfoTopView2D& info, int seed); //Generates 2D world from a Top View