#include "grid.h"
#include <vector>

Grid::Grid(int _width, int _height)
{
    width = _width;
    height = _height;
    midWidth = width/2;
    midHeight = height/2;
    tiles = vector(height, string(width, empty));
}   

const vector<string>& Grid::GetTiles() const { return tiles; }

void Grid::SetTile(int x, int y, char c)
{
    tiles[y][x] = c;
}

void Grid::SetTiles(int x, int y, const string&& str)
{
    str.copy(tiles[y].data(), str.size(), x);
}

void Grid::SetRow(int y, const string& str)
{
    tiles[y] = str;
}

void Grid::SetColumn(int x, const string& str)
{
    for (int y = 0; y < height; y++)
        tiles[y][x] = str[y];
}

void Grid::DrawRectangle(int x1, int y1, int x2, int y2, char c)
{
    for (int x = x1; x <= x2; x++)
    {
        grid.SetTile(x, y1, c);
        grid.SetTile(x, y2, c);
    }

    for (int y = y1+1; y < y2; y++)
    {
        grid.SetTile(x1, y, c);
        grid.SetTile(x2, y, c);
    }
}

bool Grid::IsOutOfBounds(int x, int y)
{
    return x < 0 || x >= width || y < 0 || y >= height;
}

bool Grid::IsCollision(int x, int y, char c)
{
    return tiles[y][x] == c;
}

int Grid::GetWidth() const { return width; }
int Grid::GetHeight() const { return height; }
int Grid::GetMidWidth() const { return midWidth; }
int Grid::GetMidHeight() const { return midHeight; }

Grid grid{0,0};