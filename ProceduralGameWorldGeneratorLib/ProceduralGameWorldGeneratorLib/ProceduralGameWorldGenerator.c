#include "ProceduralGameWorldGenerator.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>

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

    // Define noise layers
    NoiseLayer layers[] = {
        {1.f * info->zoom, 0.60f},  // Large-scale continental shapes
        {7.f * info->zoom, 0.35f},   // Medium-scale features
        {30.f * info->zoom, 0.05f}    // Small-scale details
    };

    int layer_count = sizeof(layers) / sizeof(layers[0]);

    const int total_samples = info->width * info->height; // 1. Calculate total samples and how many should be water

    float* values = malloc(total_samples * sizeof(float)); // 2. Create storage for all noise values
    float* sortValues = NULL;

    if(info->assureWaterPercentage)
        sortValues = malloc(total_samples * sizeof(float));

    float biggerSize = info->width;

    if (info->width < info->height)
        biggerSize = info->height;  // Avoid stretching, we normalize to fit the bigger side

    float nx = 0.f, ny = 0.f, total = 0.f, weightSum = 0.f, noise = 0.f;

    for (int y = 0; y < info->height; y++)  // 3. Generate noise values across a grid
    {
        for (int x = 0; x < info->width; x++) 
        {
            nx = (float)x / biggerSize; // Normalize coordinates to [0,1] range
            ny = (float)y / biggerSize;
            
            total = 0.0;
            weightSum = 0.0;

            for (int i = 0; i < layer_count; i++) 
            {
                noise = ZoomablePerlinNoise3Seed(layers[i].zoom, nx, ny, 0, 0, 0, 0, seed); // From -0.5 to 0.5
                total += noise * layers[i].weight;
                weightSum += layers[i].weight;
            }

            if(values)
                values[y * info->width + x] = total / weightSum;

            if(info->assureWaterPercentage)
                sortValues[y * info->width + x] = total / weightSum;
        }
    }

    float waterThreshold = 0.f;
    if (info->assureWaterPercentage)
    {
        QuickSort(sortValues, 0, total_samples - 1); // 4. Sort all noise values from lowest to highest
        
        if(sortValues)
            waterThreshold = sortValues[(int)(total_samples * info->waterPercent / 100.0)]; // 5. Find the threshold value at our target position
    }
    else
    {
        waterThreshold = (info->waterPercent / 100.f) -0.5f;
    }

    int waterTiles = 0, landTiles = 0;

    for (int y = 0; y < info->height; ++y)
    {
        for (int x = 0; x < info->width; ++x)
        {
            //Biome Distribution
                //Create Biomes
                   //If rules make "beach" area arround big bodies of water

            float beachThreshhold = waterThreshold + (info->beachPercent / 100) * (0.5f - waterThreshold);
            if (values)
            {
                if (values[y * info->width + x] > waterThreshold)
                {
                    info->tiles[y*info->width +x].tileId = 1; //Land
                    landTiles++;

                    if (info->addBeach)
                    {

                        if (values[y * info->width + x] < beachThreshhold)
                        {
                            info->tiles[y * info->width + x].tileId = 3; //Beach
                        }
                    }
                }
                if (values[y * info->width + x] <= waterThreshold)
                {
                    info->tiles[y * info->width + x].tileId = 2; //Water
                    waterTiles++;
                }
            }
            

            //Terrain Elevation
        }
    }

    if(sortValues)
        free(sortValues);

    if(values)
        free(values);

    float finalWaterPercent = ((float)waterTiles / (float)(info->width * info->height))* 100.f;
}

void AddBiome(WorldInfoTopView2D* worldInfo, Biome biome)
{
    assert(worldInfo->biomeCount < MAX_BIOMES); //Make sure you don't add more biomes than MAXBIOMES

    worldInfo->biomes[worldInfo->biomeCount] = biome;
    worldInfo->biomeCount++;
}

void AllocateTiles(WorldInfoTopView2D* info)
{
    if (!info) return; // Check for NULL input
    if (info->height <= 0 || info->width <= 0) return;

    if (info->tiles) {
        free(info->tiles);
        info->tiles = NULL;
    }

    info->tiles = (Tile*)calloc(info->height * info->width, sizeof(Tile));
    if (!info->tiles) return;
}

float ZoomablePerlinNoise3Seed(float zoom, float x, float y, float z, int x_wrap, int y_wrap, int z_wrap, int seed)
{
    return stb_perlin_noise3_seed(x * zoom, y * zoom, 0, 0, 0, 0, seed);
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