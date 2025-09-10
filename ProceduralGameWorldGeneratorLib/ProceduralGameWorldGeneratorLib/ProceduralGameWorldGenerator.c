#include "ProceduralGameWorldGenerator.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <time.h>

#define STB_PERLIN_IMPLEMENTATION
#include "stb_perlin.h"

bool WorldInfoTopView2D_Validate(const WorldInfoTopView2D* config)
{

    assert(config->waterPercent < 1 || config->waterPercent > 99); //WaterPercent cannot be smaller than 1 or bigger than 99

    assert(config->beachPercent < 1 || config->beachPercent > 99);  //BeachPercent cannot be smaller than 1 or bigger than 99

    return true;
}

void GenerateTopView2DWorld(WorldInfoTopView2D* info, int seed)
{
    AllocateTiles(info);

    NoiseLayer layers[] = {
        {1.f * info->zoom, 0.6f, 0.f},  // Large-scale continental shapes
        {5.f * info->zoom, 0.35f, 0.f},   // Medium-scale features
        {30.f * info->zoom, 0.05f, 0.f}    // Small-scale details
    };
    int layer_count = sizeof(layers) / sizeof(layers[0]);

    const int totalSamples = info->width * info->height;
    float* values = malloc(totalSamples * sizeof(float));
    float* sortValues = NULL;

    if(info->assurePercentages)
        sortValues = malloc(totalSamples * sizeof(float));

    float biggerSize = info->width;

    if (info->width < info->height)
        biggerSize = info->height;  // Avoid stretching, we normalize to fit the bigger side

    float nx = 0.f, ny = 0.f, total = 0.f, weightSum = 0.f, noise = 0.f;

    for (int y = 0; y < info->height; y++)
    {
        for (int x = 0; x < info->width; x++) 
        {
            nx = (float)x / biggerSize; // Normalize coordinates to [0,1] range
            ny = (float)y / biggerSize;
            
            total = 0.0;
            weightSum = 0.0;

            for (int i = 0; i < layer_count; i++) 
            {
                noise = ZoomablePerlinNoise3Seed(layers[i].zoom, layers[i].variation, nx, ny, 0, 0, 0, 0, seed); // From -0.5 to 0.5
                total += noise * layers[i].weight;
                weightSum += layers[i].weight;
            }

            if(values)
                values[y * info->width + x] = total / weightSum;

            if(info->assurePercentages && sortValues)
                sortValues[y * info->width + x] = total / weightSum;
        }
    }

    float waterThreshold = 0.f;
    float beachThreshhold = 0.f;
    if (info->assurePercentages && sortValues)
    {
        QuickSort(sortValues, 0, totalSamples - 1);
        int waterSortPos = (int)(totalSamples * info->waterPercent / 100.0);
        waterThreshold = sortValues[waterSortPos];
        beachThreshhold = sortValues[(int)(waterSortPos + ((totalSamples - waterSortPos) * info->beachPercent / 100.0))];
    }
    else
    {
        waterThreshold = (info->waterPercent / 100.f) -0.5f;
        beachThreshhold = waterThreshold + (info->beachPercent / 100) * (0.5f - waterThreshold);
    }

    int waterTiles = 0, landTiles = 0;

    for (int y = 0; y < info->height; ++y)
    {
        for (int x = 0; x < info->width; ++x)
        {
            //Biome Distribution
                //Create Biomes
                   //If rules make "beach" area arround big bodies of water

            if (values)
            {
                if (values[y * info->width + x] > waterThreshold)
                {
                    info->tiles[y * info->width + x].tileId = 1; //Land
                    landTiles++;

                    if (info->addBeach)
                    {
                        if (values[y * info->width + x] < beachThreshhold)
                        {
                            info->tiles[y * info->width + x].tileId = 3; //Beach
                        }
                    }

                    //if (values[y * info->width + x] > 0.2f)
                    //{
                    //    info->tiles[y * info->width + x].tileId = 4; //Land
                    //}
                    //if (values[y * info->width + x] > 0.20f)
                    //{
                    //    info->tiles[y * info->width + x].tileId = 5; //Land
                    //}
                    //if (values[y * info->width + x] > 0.3f)
                    //{
                    //    info->tiles[y * info->width + x].tileId = 6; //Land
                    //}

                }
                if (values[y * info->width + x] <= waterThreshold)
                {
                    info->tiles[y * info->width + x].tileId = 2; //Water
                    waterTiles++;
                }
            }

        }
    }

    srand(seed);

    for (int i = 0; i < 5; ++i)
    {
        int x = RandomRange(0, info->width);
        int y = RandomRange(0, info->height);

        int rep = RandomRange(50, 100);

        for (int j = 0; j < rep; ++j)
        {
            int width = RandomRange(1, 4);
            for (int yn = 0; yn < width; yn++) {
                for (int xn = 0; xn < width; xn++) {
                    // Calculate distance from center
                    float distance = sqrt(pow((xn+x) - x, 2) + pow((yn +y) - y, 2));

                    // Check if point is inside the circle
                    if (distance <= 5) {
                        info->tiles[(y + yn) * info->width + (x + xn)].tileId = 4; //River// Filled tile
                    }
                }

            }



            //x += RandomRange(-2, 2);
            //y += RandomRange(-2, 2);
        }
    }

    if(sortValues)
        free(sortValues);

    if(values)
        free(values);

    float finalWaterPercent = ((float)waterTiles / (float)(info->width * info->height))* 100.f;
}

//void AddBiome(WorldInfoTopView2D* worldInfo, Biome biome)
//{
//    assert(worldInfo->biomeCount < MAX_BIOMES); //Make sure you don't add more biomes than MAXBIOMES
//
//    worldInfo->biomes[worldInfo->biomeCount] = biome;
//    worldInfo->biomeCount++;
//}

void AllocateTiles(WorldInfoTopView2D* info)
{
    if (!info) return; // Check for NULL input
    if (info->height <= 0 || info->width <= 0) return;

    if (info->tiles) {
        free(info->tiles);
        info->tiles = NULL;
    }

    info->tiles = (Tile*)calloc(info->height * info->width, sizeof(Tile));
}

float ZoomablePerlinNoise3Seed(float zoom, float variation, float x, float y, float z, int x_wrap, int y_wrap, int z_wrap, int seed)
{
    return stb_perlin_noise3_seed(x * zoom + variation, y * zoom + variation, 0, 0, 0, 0, seed);
}

int CompareFloats(const void* a, const void* b)
{
    float fa = *(const float*)a;
    float fb = *(const float*)b;
    return (fa > fb) - (fa < fb); // Returns -1, 0, or 1
}

void Swap(float* a, float* b) // Swap two float elements
{
    float temp = *a;
    *a = *b;
    *b = temp;
}

int Partition(float arr[], int low, int high) // Partition the array and return the pivot index
{
    float pivot = arr[high];  // Choose last element as pivot
    int i = low - 1;          // Index of smaller element

    for (int j = low; j < high; j++) {
        if (arr[j] <= pivot) {  // Compare floats
            i++;
            Swap(&arr[i], &arr[j]);
        }
    }
    Swap(&arr[i + 1], &arr[high]);  // Place pivot in correct position
    return i + 1;                   // Return pivot index
}

void QuickSort(float arr[], int low, int high) // QuickSort function for floats
{
    if (low < high) {
        int pi = Partition(arr, low, high);  // Partition index
        QuickSort(arr, low, pi - 1);        // Sort left subarray
        QuickSort(arr, pi + 1, high);        // Sort right subarray
    }
}

int RandomRange(int min, int max) {
    return rand() % (max - min + 1) + min;
}