#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <iostream>
#include "Player.h"
#include "Grid.h"
#include <fstream>
#include <vector>
#include <array>

struct Profile{

    Grid currMap;
    Player currPlayer;

    int daysLeft = 10;

    std::vector<std::string> visitedMaps;

    // static const int MAP_COUNT = 4;
    
    //0. Cryochambers
    //1. Reactor
    //2. Science
    //3. Weapons
    //4. Quarters
    //5. Thrusters

    std::vector<std::vector<Cell>> anomolies;
    // std::vector<Cell> anomolies[MAP_COUNT];

    std::string username;

};

class GameState{ // Class used to contain all objects and variables within the game.

    public:

        GameState();
        bool saveIsValid(); // Check if the data in the file is readable (valid)
        bool readSave(); // Reads Save into accounts array
        bool writeSave(); // Writes Accounts Array to Save

        bool loadGame(Player &gamePlayer, Grid &gameGrid); // Loads profile info into the driver

        bool saveGame(Player gamePlayer, Grid gameGrid); // Saves game info into profile

        void printProfiles(); // Used in displaying profile info to driver

        void setProfileIndex(int index);

        std::string getUsername();

        void setUsername(std::string username);

        int getDaysRemaining();

        void setDaysRemaining(int daysRemaining);

        std::vector<std::string> getVisited();

        bool mapExists(std::string mapName);

        void addVisited(std::string mapName);

        int getMapIndex(std::string mapName);

        void addAnomoly(Cell newCell, std::string mapName);

        std::vector<std::vector<Cell>> getAnomolies();

        void printVisited(std::string currMap);

    private:

        static const int ACCOUNTS_SIZE = 3;

        Profile accounts[ACCOUNTS_SIZE];

        int currentProfIndex;

};

#endif