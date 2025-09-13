#include "ProceduralGameWorldGenerator.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#define STB_PERLIN_IMPLEMENTATION
#include "stb_perlin.h"

bool WorldInfoTopView2D_Validate(const WorldInfoTopView2D* config)
{
    assert(config->lowElevationPercent < 1 || config->lowElevationPercent > 99); //WaterPercent cannot be smaller than 1 or bigger than 99
    assert(config->beachPercent < 1 || config->beachPercent > 99);  //BeachPercent cannot be smaller than 1 or bigger than 99

    return true;
}

void GenerateTopView2DWorld(WorldInfoTopView2D* info, int seed)
{
    AllocateTiles(info);

    NoiseLayer layers[] = {
        {1.5f * info->zoom, 0.35f, 0.f},  // Large-scale continental shapes
        {1.5f * info->zoom, 0.35f, 1.f},
        {7.f * info->zoom, 0.25f, 0.f},   // Medium-scale features
        {30.f * info->zoom, 0.05f, 0.f}    // Small-scale details
    };
    int layerCount = sizeof(layers) / sizeof(layers[0]);

    NoiseLayer temperatureLayers[] = {
        {1.5f * info->zoom, 0.4f, 0.f},  // Large-scale continental shapes
        {1.5f * info->zoom, 0.4f, 1.f},   // Medium-scale features
        {5.f * info->zoom, 0.1f, 2.f},
        {20.f * info->zoom, 0.1f, 0.f}    // Small-scale details
    };
    int temperatureLayerCount = sizeof(layers) / sizeof(layers[0]);

    const int totalSamples = info->width * info->height;
    float* elevationValues = malloc(totalSamples * sizeof(float));
    float* sortedElevationValues = NULL;

    float* temperatureValues = malloc(totalSamples * sizeof(float));
    float* sortedTemperatureValues = NULL;

    if (info->assurePercentages)
    {
        sortedElevationValues = malloc(totalSamples * sizeof(float));
        sortedTemperatureValues = malloc(totalSamples * sizeof(float));
    }

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

            for (int i = 0; i < layerCount; i++) 
            {
                noise = ZoomablePerlinNoise3Seed(layers[i].zoom, layers[i].variation, nx, ny, 0, 0, 0, 0, seed); // From -0.5 to 0.5
                total += noise * layers[i].weight;
                weightSum += layers[i].weight;
            }

            if(elevationValues)
                elevationValues[y * info->width + x] = total / weightSum;

            if(info->assurePercentages && sortedElevationValues)
                sortedElevationValues[y * info->width + x] = total / weightSum;

            //Temperature
            total = 0.0;
            weightSum = 0.0;

            for (int i = 0; i < temperatureLayerCount; i++)
            {
                noise = ZoomablePerlinNoise3Seed(temperatureLayers[i].zoom, 0, nx + 1 + temperatureLayers[i].variation, ny + 1 + temperatureLayers[i].variation, 0, 0, 0, 0, seed); // From -0.5 to 0.5
                total += noise * temperatureLayers[i].weight;
                weightSum += temperatureLayers[i].weight;
            }

            if (temperatureValues)
                temperatureValues[y * info->width + x] = total / weightSum;

            if (info->assurePercentages && sortedTemperatureValues)
                sortedTemperatureValues[y * info->width + x] = total / weightSum;
        }
    }

    float lowElevationThreshold = 0.f;
    float midElevationThreshold = 0.f;
    float lowTempThreshold = 0.f;
    float midTempThreshold = 0.f;
    float beachThreshold = 0.f;
    if (info->assurePercentages && sortedElevationValues)
    {
        QuickSort(sortedElevationValues, 0, totalSamples - 1);
        int lowElevationSortPos = (int)(totalSamples * info->lowElevationPercent / 100.0);
        lowElevationThreshold = sortedElevationValues[lowElevationSortPos];
        midElevationThreshold = sortedElevationValues[(int)(lowElevationSortPos + ((totalSamples - lowElevationSortPos) * info->midElevationPercent / 100.0))];
        beachThreshold = sortedElevationValues[(int)(lowElevationSortPos + ((totalSamples - lowElevationSortPos) * info->beachPercent / 100.0))];

        QuickSort(sortedTemperatureValues, 0, totalSamples - 1);
        int lowTemperatureSortPos = (int)(totalSamples * info->lowTempPercent / 100.0);
        lowTempThreshold = sortedTemperatureValues[lowTemperatureSortPos];
        midTempThreshold = sortedTemperatureValues[(int)(lowTemperatureSortPos + ((totalSamples - lowTemperatureSortPos) * info->midTempPercent / 100.0))];
    }
    else
    {
        lowElevationThreshold = (info->lowElevationPercent / 100.f);
        beachThreshold = lowElevationThreshold + (info->beachPercent / 100.f) * (1 - lowElevationThreshold);
        lowElevationThreshold -= 0.5f;
        beachThreshold -= 0.5f;
    }

    int waterTiles = 0, landTiles = 0;

    for (int y = 0; y < info->height; ++y)
    {
        for (int x = 0; x < info->width; ++x)
        {
            BiomeTemperature temp = TEMP_LOW;
            BiomeHeight elevation = ELEV_LOW;

            if (temperatureValues)
            {
                if (temperatureValues[y * info->width + x] > midTempThreshold)
                {
                    temp = TEMP_HIGH; //Mountain
                }
                if (temperatureValues[y * info->width + x] <= lowTempThreshold)
                {
                    temp = TEMP_LOW;
                }
                if (temperatureValues[y * info->width + x] > lowTempThreshold && temperatureValues[y * info->width + x] < midTempThreshold)
                {
                    temp = TEMP_MEDIUM;
                }
            }

            if (elevationValues)
            {
                if (elevationValues[y * info->width + x] > midElevationThreshold)
                {
                    elevation = ELEV_HIGH;
                }
                if (elevationValues[y * info->width + x] <= lowElevationThreshold)
                {
                    elevation = ELEV_LOW;
                }
                if (elevationValues[y * info->width + x] > lowElevationThreshold && elevationValues[y * info->width + x] < midElevationThreshold)
                {
                    elevation = ELEV_MEDIUM;
                }
            }

            info->tiles[y * info->width + x].biomeId = info->biomes[elevation][temp].id;
            info->tiles[y * info->width + x].tileId = info->biomes[elevation][temp].tileId;

            if (elevationValues[y * info->width + x] > lowElevationThreshold && elevationValues[y * info->width + x] < beachThreshold)
            {
                if (info->addBeach)
                {
                    if (elevationValues[y * info->width + x] < beachThreshold)
                    {
                        info->tiles[y * info->width + x].biomeId = 8; //Hardcoded
                        info->tiles[y * info->width + x].tileId = 8; //Hardcoded
                    }
                }
            }
        }
    }

    if(sortedElevationValues)
        free(sortedElevationValues);

    if(elevationValues)
        free(elevationValues);

    if (temperatureValues)
        free(temperatureValues);
}

void AddBiome(WorldInfoTopView2D* worldInfo, Biome biome, BiomeHeight height, BiomeTemperature temperature)
{
    worldInfo->biomes[height][temperature] = biome;
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