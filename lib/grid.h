#pragma once

#include <string>
#include <vector>

using namespace std;

class Grid
{
public:
    Grid(int _width, int _height);

    const vector<string>& GetTiles() const;

    void SetTile(int x, int y, char c = empty);
    void SetTiles(int x, int y, const string&& str);
    void SetRow(int y, const string& str);
    void SetColumn(int x, const string& str);

    void DrawRectangle(int x1, int y1, int x2, int y2, char c);

    bool IsOutOfBounds(int x, int y);
    bool IsCollision(int x, int y, char c);

    int GetWidth() const;
    int GetHeight() const;
    int GetMidWidth() const;
    int GetMidHeight() const;

    static constexpr char empty = ' ';

private:
    int width{};
    int height{};
    int midWidth{};
    int midHeight{};
    vector<string> tiles;
};

extern Grid grid;