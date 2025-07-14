#pragma once
#ifndef PROCEDURAL_GAME_WORLD_GENERATOR_LIB
#define PROCEDURAL_GAME_WORLD_GENERATOR_LIB

typedef struct Tile;
typedef struct Biome;
typedef struct WorldInfoFrontSideView2D;
typedef struct WorldInfoTopView2D;

typedef struct Tile
{
	int tileId; //Represents the type of tile
	int r, g, b; //Used in testing, will usually be a texture
	int biomeId;
};

typedef struct Biome
{
	int id;
	float biomeSurfaceVariation; //Specific Biome Surface Variation
};

typedef struct WorldInfoFrontSideView2D
{
	Tile* tiles; //Tiles present in the world

	int width, height; //Width and Height of the map in tiles

	int surfaceAverageHeight; //Average height of the terrain, counting from bottom
	float surfaceVariation; //Variation in tiles of the surface, this will create higher or lower surface variation

	Biome* biomes;


};

typedef struct WorldInfoTopView2D
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

#endif // PROCEDURAL_GAME_WORLD_GENERATOR_LIB