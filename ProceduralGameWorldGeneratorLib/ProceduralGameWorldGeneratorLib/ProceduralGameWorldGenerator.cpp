#include "ProceduralGameWorldGenerator.h"
#include <stdlib.h>
#include <string.h>

#define STB_PERLIN_IMPLEMENTATION
#include "stb_perlin.h"

void GenerateFrontSideView2DWorld(WorldInfoFrontSideView2D& worldInfo, int seed)
{
	//Biome distribution

	//Ground level

	//Caves systems and holes
}

void GenerateTowView2DWorld(WorldInfoTopView2D& info, int seed)
{
    if (info.tiles) {
        free(info.tiles);
    }

    info.tiles = (Tile**)malloc(info.height * sizeof(Tile*));
    if (!info.tiles) {
        free(info.tiles);
        return;
    }

    for (int y = 0; y < info.height; y++) {
        info.tiles[y] = (Tile*)malloc(info.width * sizeof(Tile));
        if (!info.tiles[y]) {
            // Cleanup already allocated rows if allocation fails
            for (int x = 0; x < y; x++) {
                free(info.tiles[x]);
            }
            free(info.tiles);
            return;
        }
        // Initialize with zeros
        memset(info.tiles[y], 0, info.width * sizeof(Tile));
    }

    //Biome Distribution
        //Create Biomes
        //If rules make "beach" area arround big bodies of water

    float num = 0;
    for (int y = 0; y < info.width; ++y)
    {
        for (int x = 0; x < info.height; ++x)
        {
            num = stb_perlin_noise3(x * 0.1, y * 0.1,0,0,0,0);

            if (num > 0.2 || num < -0.2)
                info.tiles[x][y].tileId = 1;

            if (num < 0.2 && num >= 0)
                info.tiles[x][y].tileId = 2;

            if (num > -0.2 && num <= 0)
                info.tiles[x][y].tileId = 3;
        }
    }
	//Bodies of Water
    

	//Terrain Elevation


}
