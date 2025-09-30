#pragma warning(push, 0)
#include <stdint.h>
#include <stdlib.h>

#define WIN32_LEAN_AND_MEAN
#define UNICODE
#include <windows.h>
#pragma(pop)

#pragma warning(disable :5045)

typedef struct
{
  uint32_t x;
  uint32_t y;
} start_coord_t;

// conway's game of life
// return: pointer to buffer that holds the state of the grid
// use OutputDebugString
// 1 byte per cell
// call malloc only once
// never free
// if a cell goes to the end of the cell, it wraps around to the other side
uint8_t *simulate_life(uint32_t grid_dim, start_coord_t *initial_points, uint32_t initial_point_count)
{
  static uint8_t *grid = NULL;
  static uint32_t stored_dim = 0;
  size_t cells = (size_t)grid_dim * (size_t)grid_dim;

  if (grid == NULL)
  {
    stored_dim = grid_dim;

    grid = (uint8_t *)malloc(cells * 2);
    if (grid == NULL)
    {
      OutputDebugStringA("Memory allocation failed\n");
      return NULL;
    }

    for (uint32_t i = 0; i < grid_dim * grid_dim; i++)
      grid[i] = 0;

    for (uint32_t i = 0; i < initial_point_count; i++)
    {
      uint32_t x = initial_points[i].x % grid_dim;
      uint32_t y = initial_points[i].y % grid_dim;
      grid[y * grid_dim + x] = 1;
    }

    OutputDebugStringA("Initialized grid once\n");
    return grid;
  }

  uint8_t *new_grid = grid + (grid_dim * grid_dim);
  for (uint32_t y = 0; y < grid_dim; y++)
  {
    for (uint32_t x = 0; x < grid_dim; x++)
    {
      uint32_t live_neighbors = 0;

      for (int32_t dy = -1; dy <= 1; dy++)
      {
        for (int32_t dx = -1; dx <= 1; dx++)
        {
          if (dx == 0 && dy == 0)
            continue;
          uint32_t neighbor_x = (x + dx + grid_dim) % grid_dim;
          uint32_t neighbor_y = (y + dy + grid_dim) % grid_dim;
          live_neighbors += grid[neighbor_y * grid_dim + neighbor_x];
        }
      }

      if (grid[y * grid_dim + x] == 1)
      {
        if (live_neighbors < 2 || live_neighbors > 3)
          new_grid[y * grid_dim + x] = 0;
        else
          new_grid[y * grid_dim + x] = 1;
      }
      else
      {
        if (live_neighbors == 3)
          new_grid[y * grid_dim + x] = 1;
        else
          new_grid[y * grid_dim + x] = 0;
      }
    }
  }

  for (uint32_t i = 0; i < grid_dim * grid_dim; i++)
    grid[i] = new_grid[i];

  return grid;
}
