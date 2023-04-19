#include <iostream>
#include "GameState.h"
#include <fstream>

GameState::GameState(){

    std::vector<Cell> Cryochamber;
    std::vector<Cell> Reactor;
    std::vector<Cell> Science;
    std::vector<Cell> Weapons;
    std::vector<Cell> Quarters;
    std::vector<Cell> Thrusters;

    for(int i = 0; i < ACCOUNTS_SIZE; i++){

        accounts[i].username = "[EMPTY]";

        accounts[i].anomolies.push_back(Cryochamber);
        accounts[i].anomolies.push_back(Reactor);
        accounts[i].anomolies.push_back(Science);
        accounts[i].anomolies.push_back(Weapons);
        accounts[i].anomolies.push_back(Quarters);
        accounts[i].anomolies.push_back(Thrusters);

    }
    
    currentProfIndex = 0;

}

bool GameState::saveIsValid(){

    std::ifstream readFile;

    readFile.open("saveFile.txt");

    if(readFile.fail()){

        readFile.close();
        return false;

    }

    readFile.close();
    return true;

}

// TODO
bool GameState::readSave(){

    if(!saveIsValid()){

        return false;

    }

    std::ifstream readFile;
    
    int lineCount = 0;
    std::string line = "";
    int lineLength = 0;

    int accountIndex = -1;
    int afterIndex = 0;

    int segIterator = 0;

    int prevComma = 0;

    readFile.open("saveFile.txt");

    while(getline(readFile, line)){

        lineCount++;
        afterIndex++;

        segIterator = 0;
        prevComma = 0;

        lineLength = line.length();

        if(line == "1"){ // Start of each profile

            accountIndex = 0;
            afterIndex = 0;

        }else if(line == "2"){

            accountIndex = 1;
            afterIndex = 0;

        }else if(line == "3"){

            accountIndex = 2;
            afterIndex = 0;

        }

        if(afterIndex == 1){ // Username

            accounts[accountIndex].username = line;

        }else if(afterIndex == 2){ // Player Data

            for(int i = 0; i < lineLength; i++){

                if(line[i] == ','){

                    if(segIterator == 0){ // Player Health

                        accounts[accountIndex].currPlayer.setHealth(stoi(line.substr(prevComma, i)));
                        prevComma = i;
                        segIterator++;

                    }else if(segIterator == 1){ // Player Speed

                        accounts[accountIndex].currPlayer.setSpeed(line.substr(prevComma + 1, i - prevComma - 1));
                        prevComma = i;
                        segIterator++;

                    }else if(segIterator == 2){ // Player Direction

                        accounts[accountIndex].currPlayer.setDirection(line.substr(prevComma + 1, i - prevComma - 1));
                        prevComma = i;
                        segIterator++;

                    }else if(segIterator == 3){ // Player Steps

                        accounts[accountIndex].currPlayer.setSteps(stoi(line.substr(prevComma + 1, i - prevComma - 1)));
                        prevComma = i;
                        segIterator++;

                    }if(segIterator == 4){ // Player X Position

                        accounts[accountIndex].currPlayer.setXPos(stoi(line.substr(prevComma + 1, i - prevComma - 1)));
                        prevComma = i;
                        segIterator++;

                    }
                }

                if(segIterator == 5){ // Player Y Position

                    accounts[accountIndex].currPlayer.setYPos(stoi(line.substr(prevComma + 1, lineLength - prevComma - 1)));

                    i = lineLength;

                }

            }

        }else if(afterIndex == 3){ // Game Map

            for(int i = 0; i < lineLength; i++){

                if(line[i] == ','){

                    if(segIterator == 0){ // Current Map

                        accounts[accountIndex].currMap.setCurrMap(line.substr(prevComma, i));
                        prevComma = i;
                        segIterator++;

                    }else if(segIterator == 1){ // visitedMaps

                        accounts[accountIndex].visitedMaps.push_back(line.substr(prevComma + 1, i - prevComma - 1));
                        prevComma = i;

                    }else if(segIterator == 2){ // Anomolies map 1

                        // Find a way to print anomolies to save file

                    }

                }else if(line[i] == ']'){

                    segIterator++;

                }

            }

        }

    }

    readFile.close();

    if(lineCount == 0){

        return false;

    }

    return true;

}

// TODO
// - Meant to write profile data into save file using accounts array.
bool GameState::writeSave(){


    return false;

}

// TODO
// - Meant to load driver variables with data from profile index
bool GameState::loadGame(Player &gamePlayer, Grid &gameGrid){

    if(accounts[currentProfIndex].username != "[EMPTY]"){

        gamePlayer.setHealth(accounts[currentProfIndex].currPlayer.getHealth());
        gamePlayer.setSpeed(accounts[currentProfIndex].currPlayer.getSpeed());
        gamePlayer.setDirection(accounts[currentProfIndex].currPlayer.getDirection());
        gamePlayer.setSteps(accounts[currentProfIndex].currPlayer.getSteps());
        gamePlayer.setXPos(accounts[currentProfIndex].currPlayer.getXPos());
        gamePlayer.setYPos(accounts[currentProfIndex].currPlayer.getYPos());

        gameGrid.setCurrMap(accounts[currentProfIndex].currMap.getCurrMap());
        gameGrid.drawPlayer(gamePlayer);

        return true;

    }

    return false;

}

// TODO
// - Meant to save driver variables into profile using index
bool GameState::saveGame(Player gamePlayer, Grid gameGrid){

    if(accounts[currentProfIndex].username != "[EMPTY]"){

        accounts[currentProfIndex].currPlayer.setHealth(gamePlayer.getHealth());
        accounts[currentProfIndex].currPlayer.setSpeed(gamePlayer.getSpeed());
        accounts[currentProfIndex].currPlayer.setDirection(gamePlayer.getDirection());
        accounts[currentProfIndex].currPlayer.setSteps(gamePlayer.getSteps());
        accounts[currentProfIndex].currPlayer.setXPos(gamePlayer.getXPos());
        accounts[currentProfIndex].currPlayer.setYPos(gamePlayer.getYPos());

        accounts[currentProfIndex].currMap.setCurrMap(gameGrid.getCurrMap());
        accounts[currentProfIndex].currMap.drawPlayer(gamePlayer);

        return true;

    }

    return false;    

}

// TODO
// - Print all profiles' username, modules fixed, and days remaining.
void GameState::printProfiles(){

    return;

}

void GameState::setProfileIndex(int index){

    currentProfIndex = index;

}

std::string GameState::getUsername(){

    return accounts[currentProfIndex].username;

}

void GameState::setUsername(std::string username){

    accounts[currentProfIndex].username = username;

}

int GameState::getDaysRemaining(){

    return accounts[currentProfIndex].daysLeft;

}

void GameState::setDaysRemaining(int daysRemaining){

    accounts[currentProfIndex].daysLeft = daysRemaining;

}

std::vector<std::string> GameState::getVisited(){

    return accounts[currentProfIndex].visitedMaps;

}

bool GameState::mapExists(std::string mapName){

    for(int i = 0; i < accounts[currentProfIndex].visitedMaps.size(); i++){

        if(accounts[currentProfIndex].visitedMaps[i] == mapName){

            return true;

        }

    }

    return false;

}

void GameState::addVisited(std::string mapName){

    accounts[currentProfIndex].visitedMaps.push_back(mapName);

}

int GameState::getMapIndex(std::string mapName){

    if(mapName == "Cryochambers.txt"){

        return 0;

    }

    if(mapName == "Reactor.txt"){

        return 1;

    }

    if(mapName == "Science.txt"){

        return 2;

    }

    if(mapName == "Weapons.txt"){

        return 3;

    }

    if(mapName == "Quarters.txt"){

        return 4;

    }

    if(mapName == "Thrusters.txt"){

        return 5;

    }

    return -1;
}

void GameState::addAnomoly(Cell newCell, std::string mapName){

    accounts[currentProfIndex].anomolies[getMapIndex(mapName)].push_back(newCell);

}

std::vector<std::vector<Cell>> GameState::getAnomolies(){

    return accounts[currentProfIndex].anomolies;

}

void GameState::printVisited(std::string currMap){

    std::cout << "[Current Room] " << currMap.substr(0, currMap.length()-4) << std::endl;

    std::cout << std::endl;

    //Top Line
    std::cout << "         [Quarters]" << std::endl;
    std::cout << "            |" << std::endl;

    //Mid Line
   std::cout << "[Cryo] -> [Weapons] -> [Thrusters]" << std::endl;

    //Bottom Line
   std::cout << "  |" << std::endl;
   std::cout << "[Reactor] -> [Science Lab]" << std::endl;

   std::cout << std::endl;
   std::cout << std::endl;

}