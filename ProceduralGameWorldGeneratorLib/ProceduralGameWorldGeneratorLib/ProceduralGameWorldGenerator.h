#pragma once

struct Tile
{
	int r, g, b;

};

struct WorldInfo2D
{
	Tile tiles[]; //Tiles present in the world

	
};

void AddTile(WorldInfo2D& info, Tile tile)
{
	info.tiles;
}

void Generate2dWorld(WorldInfo2D& info);