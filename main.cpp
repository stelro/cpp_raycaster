#include <iostream>
#include <vector>


#include "cglib.hpp"

using namespace CgLib;

const int mapWidth = 24;
const int mapHeight = 24;
const int textureWidth = 64;
const int textureHeight = 64;
#define TEXTURE_SIZE 64
#define NUM_TEXTURES 8
#define TEXTURE_SIZE_BINARY (int)log2(TEXTURE_SIZE)
const int WIDTH = 1024;
const int HEIGHT = 768;

int worldMap[mapWidth][mapHeight]=
{
        {4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,7,7,7,7,7,7,7,7},
        {4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,0,0,0,7},
        {4,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7},
        {4,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7},
        {4,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,0,0,0,7},
        {4,0,4,0,0,0,0,5,5,5,5,5,5,5,5,5,7,7,0,7,7,7,7,7},
        {4,0,5,0,0,0,0,5,0,5,0,5,0,5,0,5,7,0,0,0,7,7,7,1},
        {4,0,6,0,0,0,0,5,0,0,0,0,0,0,0,5,7,0,0,0,0,0,0,8},
        {4,0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,7,7,1},
        {4,0,8,0,0,0,0,5,0,0,0,0,0,0,0,5,7,0,0,0,0,0,0,8},
        {4,0,0,0,0,0,0,5,0,0,0,0,0,0,0,5,7,0,0,0,7,7,7,1},
        {4,0,0,0,0,0,0,5,5,5,5,0,5,5,5,5,7,7,7,7,7,7,7,1},
        {6,6,6,6,6,6,6,6,6,6,6,0,6,6,6,6,6,6,6,6,6,6,6,6},
        {8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4},
        {6,6,6,6,6,6,0,6,6,6,6,0,6,6,6,6,6,6,6,6,6,6,6,6},
        {4,4,4,4,4,4,0,4,4,4,6,0,6,2,2,2,2,2,2,2,3,3,3,3},
        {4,0,0,0,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,0,0,0,2},
        {4,0,0,0,0,0,0,0,0,0,0,0,6,2,0,0,5,0,0,2,0,0,0,2},
        {4,0,0,0,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,2,0,2,2},
        {4,0,6,0,6,0,0,0,0,4,6,0,0,0,0,0,5,0,0,0,0,0,0,2},
        {4,0,0,5,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,2,0,2,2},
        {4,0,6,0,6,0,0,0,0,4,6,0,6,2,0,0,5,0,0,2,0,0,0,2},
        {4,0,0,0,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,0,0,0,2},
        {4,4,4,4,4,4,4,4,4,4,1,1,1,2,2,2,2,2,2,3,3,3,3,3}
};


int main()
{
    //-----------------------------------------------//
    //** ENGINE CONFIGIURATION *//
    Cglib renderer("Stel Demo", 1024, 768, 30);
    renderer.setDebugMode(true);
    renderer.loadFont("assets/FiraCode-Medium.ttf", 18);

    //** ENGINE CONFIGURATION ENDS **//
    //-----------------------------------------------//


    //Player
    Vec2<float> playerPos{22, 12};
    Vec2<float> playerDirection{-1, 0};
    Vec2<float> cameraPlane{0, 0.66f};

    std::vector<Texture*> textures;
    Uint32 buffer[HEIGHT][WIDTH];

    for (auto i = 0; i < 8; i++) {
        //Initialize Texture vector
        textures.push_back( new Texture(renderer.getRenderer(), renderer.getWindow()) );
    }

    textures[0]->loadFromFile("assets/bluestone.png");
    textures[1]->loadFromFile("assets/colorstone.png");
    textures[2]->loadFromFile("assets/eagle.png");
    textures[3]->loadFromFile("assets/greystone.png");
    textures[4]->loadFromFile("assets/mossy.png");
    textures[5]->loadFromFile("assets/purplestone.png");
    textures[6]->loadFromFile("assets/redbrick.png");
    textures[7]->loadFromFile("assets/wood.png");



    while (renderer.isRuning()) {

        renderer.beginRender();

        //renderer.drawRect({playerPos.x, playerPos.y, 10,10});

        for (int x = 0; x < WIDTH; x++) {

            //Ray
            float cameraX = 2 * x / float(WIDTH) - 1;
            Vec2<float> rayPosition = playerPos;
            Vec2<float> rayDirection = {
                    playerDirection.x + cameraPlane.x * cameraX,
                    playerDirection.y + cameraPlane.y * cameraX,
            };
            //----end or ray---

            //variables related to DDA algorithm

            //Which box of the map we are in?
            Vec2<int> gridMap = {
                    static_cast<int>(rayPosition.x),
                    static_cast<int>(rayPosition.y)
            };

            //length of the ray from current position to next x or y-side
            Vec2<float> sideDistance;

            //length of the ray from on x or y-side to next x or y-side
            Vec2<float> deltaDistance = {
                    sqrt(1 + (rayDirection.y * rayDirection.y) / (rayDirection.x * rayDirection.x )),
                    sqrt(1 + (rayDirection.x * rayDirection.x) / (rayDirection.y * rayDirection.y ))
            };


            float perpWallDistance;

            //what direction to step in x or y-direction ( either + 1 or - 1 )
            Vec2<int> step;

            //Was there a wall hit?
            int hit = 0;

            //Was NS or a EW wall hit?
            int side;

            //calculate step and initial sideDist
            if ( rayDirection.x < 0 ) {
                step.x = -1;
                sideDistance.x = (rayPosition.x - gridMap.x) * deltaDistance.x;
            } else {
                step.x = 1;
                sideDistance.x = (gridMap.x + 1.0f - rayPosition.x) * deltaDistance.x;
            }

            if ( rayDirection.y < 0 ) {
                step.y = -1;
                sideDistance.y = (rayPosition.y - gridMap.y) * deltaDistance.y;
            } else {
                step.y = 1;
                sideDistance.y = (gridMap.y + 1.0f - rayPosition.y) * deltaDistance.y;
            }

            //Perform DDA
            while ( hit == 0 ) {
                //Jump to next map square, OR in x-direction, OR in y-direction
                if ( sideDistance.x < sideDistance.y ) {
                    sideDistance.x += deltaDistance.x;
                    gridMap.x += step.x;
                    side = 0;
                } else {
                    sideDistance.y += deltaDistance.y;
                    gridMap.y += step.y;
                    side = 1;
                }

                //Check if ray has hit a wall
                if ( worldMap[gridMap.x][gridMap.y] > 0 ) {
                    hit = 1;
                }
            } // DDA end

            if ( side == 0 ) {
                perpWallDistance = (gridMap.x - rayPosition.x + ( 1 - step.x ) / 2 ) / rayDirection.x;
            } else {
                perpWallDistance = (gridMap.y - rayPosition.y + ( 1 - step.y ) / 2 ) / rayDirection.y;
            }

            //Calculate height of line to draw on screen
            int wallHeight = static_cast<int>( HEIGHT / perpWallDistance );

            //Calculate lowest and highest pixel to fill in current stripe
            int start = -wallHeight / 2 + HEIGHT / 2;

            if ( start < 0 )
                start = 0;

            int end = wallHeight / 2 + HEIGHT / 2;

            if ( end >= HEIGHT )
                end = HEIGHT - 1;



            int textureNumber = worldMap[gridMap.x][gridMap.y] - 1;

            //Calculate value of WallX
            float wallX; //where exactly the wall was hit
            if ( side == 0 )
                wallX = rayPosition.x + ((gridMap.y - rayPosition.y + ((1 - step.y) >> 1)) / rayDirection.y) * rayDirection.x;
            else
                wallX = rayPosition.y + ((gridMap.x - rayPosition.x + ((1 - step.x) >> 1)) / rayDirection.x) * rayDirection.y;

            wallX -= floor((wallX));

            //Calculates x coordinate of texture
            int textureX = (int) (wallX * (double)TEXTURE_SIZE);
            if ((!side &&  rayDirection.x  > 0) || (side && rayDirection.y < 0))
                textureX = TEXTURE_SIZE - textureX - 1;


            //Calculate brightness of this strip of pixels
            double brightness = perpWallDistance / 2;


            textures[textureNumber]->lockTexture();
            auto* pixels = static_cast<Uint32 *>(textures[textureNumber]->getPixels());


            for(int y = start; y < end; ++y)
            {
                int d = y * 256 - HEIGHT * 128 + wallHeight * 128;  //256 and 128 factors to avoid floats
                int texY = ((d * textureHeight) / wallHeight) / 256;
                Uint32 color = pixels[textureHeight * texY + textureX];
                //make color darker for y-sides: R, G and B byte each divided through two with a "shift" and an "and"
                if(side == 1) color = (color >> 1) & 8355711;
                buffer[y][x] = color;

                //Draw the individual pixel. May be faster if a buffer is used
                renderer.drawPoint(x, y, color);

            }

            textures[textureNumber]->unlockTexture();


        }


        //----- raycasting loop ends -----------//

        //speed modifiers


        const auto sngFrame = renderer.getAvgFPS() / 1000.0f;
        const auto moveSpeed = sngFrame * 5.0f; //the constant value is in squares/second
        const auto rotSpeed = sngFrame * 2.3f; //the constant value is in radians/second

        renderer.updateEvents();{
            if (renderer.isKeyDown(SDLK_ESCAPE)) {
                break;
            }

            if (renderer.isKeyDown(SDLK_w))
            {
                if(!worldMap[int(playerPos.x + playerDirection.x * moveSpeed)][int(playerPos.y)])
                    playerPos.x += playerDirection.x * moveSpeed;
                if(!worldMap[int(playerPos.x)][int(playerPos.y + playerDirection.y * moveSpeed)])
                    playerPos.y += playerDirection.y * moveSpeed;
            }
            //move backwards if no wall behind you
            if (renderer.isKeyDown(SDLK_s))
            {
                if(!worldMap[int(playerPos.x - playerDirection.x * moveSpeed)][int(playerPos.y)])
                    playerPos.x -= playerDirection.x * moveSpeed;
                if(!worldMap[int(playerPos.x)][int(playerPos.y - playerDirection.y * moveSpeed)])
                    playerPos.y -= playerDirection.y * moveSpeed;
            }
            //rotate to the right
            if (renderer.isKeyDown(SDLK_d))
            {
                //both camera direction and camera plane must be rotated
                const auto oldDirX = playerDirection.x;
                playerDirection.x = (playerDirection.x * cos(-rotSpeed) - playerDirection.y * sin(-rotSpeed));
                playerDirection.y = (oldDirX * sin(-rotSpeed) + playerDirection.y * cos(-rotSpeed));
                const auto oldPlaneX = cameraPlane.x;
                cameraPlane.x = (cameraPlane.x * cos(-rotSpeed) - cameraPlane.y * sin(-rotSpeed));
                cameraPlane.y = (oldPlaneX * sin(-rotSpeed) + cameraPlane.y * cos(-rotSpeed));
            }
            //rotate to the left
            if (renderer.isKeyDown(SDLK_a))
            {
                //both camera direction and camera plane must be rotated
                const auto oldDirX = playerDirection.x;
                playerDirection.x = (playerDirection.x * cos(rotSpeed) - playerDirection.y * sin(rotSpeed));
                playerDirection.y = ( oldDirX * sin(rotSpeed) + playerDirection.y * cos(rotSpeed));
                const auto oldPlaneX = cameraPlane.x;
                cameraPlane.x = (cameraPlane.x * cos(rotSpeed) - cameraPlane.y * sin(rotSpeed));
                cameraPlane.y = (oldPlaneX * sin(rotSpeed) + cameraPlane.y * cos(rotSpeed));
            }

        }

        renderer.endRender();

    }

    return 0;
}