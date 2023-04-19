#ifndef GRID_H
#define GRID_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "Player.h"
#include <math.h>

struct Coordinates{

    int xPos;
    int yPos;

    Coordinates(){
        
        xPos = 0;
        yPos = 0;

    }

    Coordinates(int xPos_, int yPos_){

        xPos = xPos_;
        yPos = yPos_;

    }

};

struct Cell{

    Coordinates coords;

    char character;

};

class Grid{

    public:

        Grid();

        void loadMap(std::string fileName);
        void loadMap(std::string fileName, std::vector<Cell> anomolies);

        void printMap(bool visionCone);

        std::string getCell(int xPos, int yPos);
        std::string getCell(std::string direction);

        std::string getPlayerPosition();
        std::string getCurrMap();

        void setCell(int xPos, int yPos, char filler);

        void changePlayerPosition(int xPos, int yPos);

        bool drawPlayer(Player currentPlayer);

        void setCurrMap(std::string currMap_);

        bool movePlayer(std::string direction, Player &currentPlayer);

        int distToWall(std::string direction, int xPos, int yPos);

        bool checkForNearbyItems();

        bool checkForNearbyAliens();

        bool checkForNearbyDoors();

        bool checkForNearbyModules();

        bool modFixCheck(std::string modName);

        Coordinates nearbyCoords(std::string objectCell);

        void printPlrPos();

        void changeMap(Player &currentPlayer, std::vector<std::vector<Cell>> anomolies);

        void changeMap(Player &currentPlayer, std::vector<std::vector<Cell>> anomolies, std::string mapName);

        std::string modGame(Player &currentPlayer);

        int mapToIndex();

        void updateVisibility();

        std::vector<Coordinates> findVisibility(std::string facing);

        bool withinFlareCircle(Coordinates flareCenter, int xPos, int yPos);

        bool throwFlare();

        Cell getRecentFlare();

        bool checkAllModFixed();

    private:

        //Map Size
        int static const rowSize = 20;
        int static const colSize = 50;

        int static const numMaps = 6;
        // 1

        char map[rowSize][colSize];
        bool visiMap[numMaps][rowSize][colSize];

        //Flare Vars
        std::vector<Cell> validFlares;

        //Player Vars
        Coordinates playerPosition;
        std::string direction;

        //Module Vars
        bool beginningAccomplished;
        bool reactorFixed;
        bool scienceDoorsOpen;
        bool cureFound;
        bool weaponsFixed;
        bool armoryUnlocked;
        bool overrideComplete;

        std::string currMap;

};

#endif