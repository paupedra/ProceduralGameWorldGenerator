#include "ProceduralGameWorldGenerator.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>

#define STB_PERLIN_IMPLEMENTATION
#include "stb_perlin.h"

bool WorldInfoTopView2D_Validate(const WorldInfoTopView2D* config)
{
    return false;
}

void AddBiome(WorldInfoTopView2D* worldInfo, Biome biome)
{
    assert(worldInfo->biomeCount < MAX_BIOMES); //Make sure you don't add more biomes than MAXBIOMES
    
    worldInfo->biomes[worldInfo->biomeCount] = biome;
    worldInfo->biomeCount++;
}

int compare_floats(const void* a, const void* b) {
    float fa = *(const float*)a; // Cast to float pointer and dereference
    float fb = *(const float*)b;
    return (fa > fb) - (fa < fb); // Returns -1, 0, or 1
}

// Swap two float elements
void swap(float* a, float* b) {
    float temp = *a;
    *a = *b;
    *b = temp;
}

// Partition the array and return the pivot index
int partition(float arr[], int low, int high) {
    float pivot = arr[high];  // Choose last element as pivot
    int i = low - 1;          // Index of smaller element

    for (int j = low; j < high; j++) {
        if (arr[j] <= pivot) {  // Compare floats
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);  // Place pivot in correct position
    return i + 1;                   // Return pivot index
}

// QuickSort function for floats
void quickSort(float arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);  // Partition index
        quickSort(arr, low, pi - 1);        // Sort left subarray
        quickSort(arr, pi + 1, high);        // Sort right subarray
    }
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

    int biggerSize = info->width;

    if (info->width < info->height)
        biggerSize = info->height;  //Avoid stretching, we generate a square with biggest size

    float nx, ny;
    for (int y = 0; y < info->height; y++)  // 3. Generate noise values across a grid
    {
        for (int x = 0; x < info->width; x++) 
        {
            nx = (float)x / biggerSize;   // Normalize coordinates to [0,1] range
            ny = (float)y / biggerSize;
            
            float total = 0.0;
            float weight_sum = 0.0; // Calculate weighted sum of all noise layers

            for (int i = 0; i < layer_count; i++) 
            {
                float noise = ZoomablePerlinNoise3Seed(layers[i].zoom, nx, ny, 0, 0, 0, 0, seed); //From -0.5 to 0.5?
                total += noise * layers[i].weight;
                weight_sum += layers[i].weight;
            }

            values[y * info->width + x] = total / weight_sum;

            if(info->assureWaterPercentage)
                sortValues[y * info->width + x] = total / weight_sum;
        }
    }

    float waterThreshold;
    if (info->assureWaterPercentage)
    {
        quickSort(sortValues, 0, total_samples - 1);// 4. Sort all noise values from lowest to highest
        
        waterThreshold = sortValues[(int)(total_samples * info->waterPercent / 100.0)]; // 5. Find the threshold value at our target position
        free(sortValues);
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
            if (values[y * info->width + x] > waterThreshold)
            {
                info->tiles[y][x].tileId = 1; //Land
                landTiles++;

                if (info->addBeach)
                {
                    
                    if (values[y * info->width + x] < beachThreshhold)
                    {
                        info->tiles[y][x].tileId = 3; //Beach
                    }
                }
            }
            if (values[y * info->width + x] <= waterThreshold)
            {
                info->tiles[y][x].tileId = 2; //Water
                waterTiles++;
            }

            

            //Terrain Elevation
        }
    }

    free(values);

    float finalWaterPercent = ((float)waterTiles / (float)(info->width * info->height))* 100.f;
}

void AllocateTiles(WorldInfoTopView2D* info)
{
    if (info->tiles) {
        free(info->tiles);
    }

    info->tiles = (Tile**)malloc(info->height * sizeof(Tile*));
    if (!info->tiles) {
        free(info->tiles);
        return;
    }

    for (int y = 0; y < info->height; y++) {
        info->tiles[y] = (Tile*)malloc(info->width * sizeof(Tile));
        if (!info->tiles[y]) {
            // Cleanup already allocated rows if allocation fails
            for (int x = 0; x < y; x++) {
                free(info->tiles[x]);
            }
            free(info->tiles);
            return;
        }
        // Initialize with zeros
        memset(info->tiles[y], 0, info->width * sizeof(Tile));
    }
}

float ZoomablePerlinNoise3Seed(float zoom, float x, float y, float z, int x_wrap, int y_wrap, int z_wrap, int seed)
{
    return stb_perlin_noise3_seed(x * zoom, y * zoom, 0, 0, 0, 0, seed);
}