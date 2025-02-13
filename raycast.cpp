/* A prototype for making raycast games in a console */

#include <math.h>
#include <vector>
#include "lib/frame.h"
#include "lib/game.h"
#include "lib/grid.h"
#include "lib/input.h"

using namespace std;

class Map
{
    public:
    static const int width = 10;
    static const int height = 10;
    char tiles[width][height]
    {
        {1,1,1,1,0,1,1,1,1,1},
        {1,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,1,0,1},
        {1,0,0,0,0,0,0,0,0,1},
        {1,0,1,1,0,0,0,1,0,1},
        {1,0,1,1,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,1,0,1},
        {1,0,0,0,0,0,0,0,0,1},
        {1,1,1,1,1,1,1,1,1,1},
    };
};

class Player
{
    public:
    double x{4.5};
    double y{5.5};
    double dirX{0}, dirY{-1};
    double planeX{1}, planeY{0}; //the 2d raycaster version of camera plane

    void Move(const Map& worldMap)
    {
        //speed modifiers
        double moveSpeed = 0.1; //the constant value is in squares/second
        double rotSpeed = 0.07; //the constant value is in radians/second
        
        //move forward if no wall in front of you
        if(userInput == UserInput::Up)
        {
            double newX = x + dirX * moveSpeed;
            if(newX >= 0 && newX < worldMap.width && worldMap.tiles[int(y)][int(newX)] == 0) x += dirX * moveSpeed;
            double newY = y + dirY * moveSpeed;
            if(newY >= 0 && newY < worldMap.height && worldMap.tiles[int(newY)][int(x)] == 0) y += dirY * moveSpeed;
        }
        //move backwards if no wall behind you
        if(userInput == UserInput::Down)
        {
            double newX = x - dirX * moveSpeed;
            if(newX >= 0 && newX < worldMap.width && worldMap.tiles[int(y)][int(newX)] == 0) x -= dirX * moveSpeed;
            double newY = y - dirY * moveSpeed;
            if(newY >= 0 && newY < worldMap.height && worldMap.tiles[int(newY)][int(x)] == 0) y -= dirY * moveSpeed;
        }

        if(userInput == UserInput::Left)
        {
            //both camera direction and camera plane must be rotated
            double oldDirX = dirX;
            dirX = dirX * cos(-rotSpeed) - dirY * sin(-rotSpeed);
            dirY = oldDirX * sin(-rotSpeed) + dirY * cos(-rotSpeed);
            double oldPlaneX = planeX;
            planeX = planeX * cos(-rotSpeed) - planeY * sin(-rotSpeed);
            planeY = oldPlaneX * sin(-rotSpeed) + planeY * cos(-rotSpeed);
        }
        
        if(userInput == UserInput::Right)
        {
            //both camera direction and camera plane must be rotated
            double oldDirX = dirX;
            dirX = dirX * cos(rotSpeed) - dirY * sin(rotSpeed);
            dirY = oldDirX * sin(rotSpeed) + dirY * cos(rotSpeed);
            double oldPlaneX = planeX;
            planeX = planeX * cos(rotSpeed) - planeY * sin(rotSpeed);
            planeY = oldPlaneX * sin(rotSpeed) + planeY * cos(rotSpeed);
        }
    }
};

class MapToGrid
{
    public:
    void Translate(const Player& player, const Map& map)
    {
        double rayX = player.x;
        double rayY = player.y;
        double distToSideX = 0;
        double distToSideY = 0;

        for (int x = 0; x < grid.GetWidth(); x++)
        {
            //calculate ray position and direction
            double cameraX = 2 * x / double(grid.GetWidth()-1) - 1; //x-coordinate in camera space
            double rayDirX = player.dirX + player.planeX * cameraX;
            double rayDirY = player.dirY + player.planeY * cameraX;

            //which box of the map we're in
            int mapX = int(player.x);
            int mapY = int(player.y);            

            double ratioDistanceToX = (rayDirX == 0) ? 1e30 : abs(1/rayDirX); // how many x units per 1 raycast distance
            double ratioDistanceToY = (rayDirY == 0) ? 1e30 : abs(1/rayDirY); // how many y units per 1 raycast distance

            double distX = 0;
            double distY = 0;

            int stepX = 0;
            int stepY = 0;

            // offset to align with grid
            if (rayDirX < 0)
            {
                // moving left
                stepX = -1;
                distX = (player.x - mapX) * ratioDistanceToX;
            }
            else
            {
                // moving right
                stepX = 1;
                distX = (1 + mapX - player.x) * ratioDistanceToX;
            }
            
            if (rayDirY < 0)
            {
                // moving up
                stepY = -1;
                distY = (player.y - mapY) * ratioDistanceToY;
            }
            else 
            {
                // moving down    
                stepY = 1;
                distY = (mapY + 1 - player.y) * ratioDistanceToY;
            }

            bool hit = false;
            int side = -1;
            while (!hit)
            {
                if (distX < distY)
                {
                    // will hit X grid first
                    distX += ratioDistanceToX;
                    mapX += stepX;
                    side = 0;
                }
                else
                {
                    // will hit Y grid first
                    distY += ratioDistanceToY;
                    mapY += stepY;
                    side = 1;
                }

                if (mapX < 0 || mapX >= map.width || mapY < 0 || mapY >= map.height)
                    break;

                if (map.tiles[mapY][mapX] > 0)
                    hit = true;
            }

            string str(grid.GetHeight(), ' ');

            if (hit)
            {
                double perpWallDist = 0;
                if(side == 0) perpWallDist = (distX - ratioDistanceToX);
                else          perpWallDist = (distY - ratioDistanceToY);

                //Calculate height of line to draw on screen
                int h = grid.GetHeight();
                int lineHeight = (int)(h / perpWallDist);

                //calculate lowest and highest pixel to fill in current stripe
                int drawStart = -lineHeight / 2 + h / 2;
                if(drawStart < 0)drawStart = 0;
                int drawEnd = lineHeight / 2 + h / 2;
                if(drawEnd >= h)drawEnd = h - 1;

                for (int i = 0; i < drawStart; i++)
                    str[i] = ceiling;

                char c;
                if (side == 0)
                    c = perpWallDist > 7 ? wallEWFar : wallEW;
                else
                    c = perpWallDist > 7 ? wallNSFar : wallNS;
                for(int i = drawStart; i <= drawEnd; i++)
                    str[i] = c;

                for (int i = drawEnd+1; i < h; i++)
                    str[i] = floor;
            }
            else
            {
                for (int i = 0; i < grid.GetHeight()/2; i++)
                    str[i]= ceiling;
                for (int i = grid.GetHeight()/2; i < grid.GetHeight(); i++)
                    str[i]= floor;
            }
                
            grid.SetColumn(x, str);
        }
    }

    private:
    char wallEW = '%';
    char wallEWFar = '/';
    char wallNS = '+';
    char wallNSFar = ':';
    char floor = '.';
    char ceiling = ' ';
};

class Raycast : public Game
{
    public:
    Raycast(int fps) : Game(fps) {}

    protected:
    void Update()
    {
        player.Move(map);
        mapToGrid.Translate(player, map);
    }

    private:
    Player player{};
    Map map{};
    MapToGrid mapToGrid{};
};

int main()
{
    constexpr int FPS{30};
    Raycast raycast{FPS};
    raycast.Start();
    return 0;
}