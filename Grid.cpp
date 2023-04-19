#include <iostream>
#include <conio.h>
#include "Grid.h"

std::string strLowerGrid(std::string inputString){

    int stringLength = inputString.length();

    std::string result = "";

    for(int i = 0; i < stringLength; i++){

        if(inputString[i] >= 'A' && inputString[i] <= 'Z'){

            result += inputString[i] + 32;

        }else{

            result += inputString[i];

        }
            

    }

    return result;

}

Grid::Grid(){

    //Map
    for(int row = 0; row < rowSize; row++){

        for(int col = 0; col < colSize; col++){

            map[row][col] = '-';

        }
    }

    for(int maps = 0; maps < numMaps; maps++){

        for(int row = 0; row < rowSize; row++){

            for(int col = 0; col < colSize; col++){

                visiMap[maps][row][col] = false;

            }
        }
    }

    //Player Vars
    playerPosition.xPos = 0;
    playerPosition.yPos = 0;

    direction = "South";

    //Module Vars
    beginningAccomplished = false;
    reactorFixed = false;
    scienceDoorsOpen = false;
    cureFound = false;
    weaponsFixed = false;
    armoryUnlocked = false;
    overrideComplete = false;

    currMap = "NULL";

}

void Grid::loadMap(std::string fileName){

    std::ifstream readFile;

    readFile.open(fileName);

    if(readFile.fail()){

        std::cout << "[ERROR] Cannot read file: " << fileName << std::endl;
        return;

    }

    std::string line = "";
    int lineLength = 0;

    int row = 0;

    while(getline(readFile, line)){

        lineLength = line.length();

        for(int lineIterator = 0; lineIterator < lineLength; lineIterator++){

            map[row][lineIterator] = line[lineIterator];

        }

        row++;

    }

    currMap = fileName;

}

void Grid::loadMap(std::string fileName, std::vector<Cell> anomolies){

    std::ifstream readFile;

    readFile.open(fileName);

    if(readFile.fail()){

        std::cout << "[ERROR] Cannot read file: " << fileName << std::endl;
        return;

    }

    std::string line = "";
    int lineLength = 0;

    int row = 0;

    while(getline(readFile, line)){

        lineLength = line.length();

        for(int lineIterator = 0; lineIterator < lineLength; lineIterator++){

            map[row][lineIterator] = line[lineIterator];

        }

        row++;

    }

    for(int i = 0; i < anomolies.size(); i++){

        map[anomolies[i].coords.yPos][anomolies[i].coords.xPos] = anomolies[i].character;

        if(anomolies[i].character == ';'){

            validFlares.push_back(anomolies[i]);

        }

    }

    currMap = fileName;

}

void Grid::printMap(bool vision){

    updateVisibility();

    if(vision){

        for(int row = 0; row < rowSize; row++){ // Row means Y Position

            for(int col = 0; col < colSize; col++){ // Column means X Position

                if(visiMap[mapToIndex()][row][col]){

                    std::cout << map[row][col];

                }else{

                    std::cout << '"';

                }

            }

            std::cout << std::endl;

        }

    }else{

        for(int row = 0; row < rowSize; row++){

            for(int col = 0; col < colSize; col++){

                std::cout << map[row][col];

            }

            std::cout << std::endl;

        }

    }

}

std::string Grid::getCell(int xPos, int yPos){

    std::string cell = "[";

    if(xPos < 0 || xPos >= colSize || yPos < 0 || yPos >= rowSize){

        return "";

    }

    cell += map[yPos][xPos];

    cell += "]";

    return cell;

}

std::string Grid::getCell(std::string direction){

    std::string cell = "[";

    if(direction == "North" && playerPosition.yPos - 1 >= 0){

        cell += map[playerPosition.yPos - 1][playerPosition.xPos];

    }else if(direction == "East" && playerPosition.xPos + 1 < colSize){

        cell += map[playerPosition.yPos][playerPosition.xPos + 1];

    }else if(direction == "South" && playerPosition.yPos + 1 < rowSize){

        cell += map[playerPosition.yPos + 1][playerPosition.xPos];

    }else if(direction == "West" && playerPosition.xPos - 1 >= 0){

        cell += map[playerPosition.yPos][playerPosition.xPos - 1];

    }

    cell += "]";

    return cell;

}

std::string Grid::getCurrMap(){

    return currMap;

}

void Grid::setCell(int xPos, int yPos, char filler){

    if(xPos < 0 || xPos >= colSize || yPos < 0 || yPos >= rowSize){

        return;

    }

    map[yPos][xPos] = filler;

}

void Grid::changePlayerPosition(int xPos, int yPos){

    playerPosition.xPos = playerPosition.xPos + xPos;
    playerPosition.yPos = playerPosition.yPos + yPos;

}

bool Grid::drawPlayer(Player currentPlayer){

    if(currentPlayer.getXPos() >= 0 && currentPlayer.getXPos() < colSize){

        if(currentPlayer.getYPos() >= 0 && currentPlayer.getYPos() < rowSize){

            if(map[currentPlayer.getYPos()][currentPlayer.getXPos()] == ' '){

                map[currentPlayer.getYPos()][currentPlayer.getXPos()] = '+';

                playerPosition.xPos = currentPlayer.getXPos();
                playerPosition.yPos = currentPlayer.getYPos();

                direction = currentPlayer.getDirection();

                return true;

            }

        }

    }

    return false;

}

void Grid::setCurrMap(std::string currMap_){

    currMap = currMap_;

}

bool Grid::movePlayer(std::string facing, Player &currentPlayer){

    int xPos = currentPlayer.getXPos();
    int yPos = currentPlayer.getYPos();

    std::string speed = currentPlayer.getSpeed();

    int sneakDist = 1;
    int walkDist = 2;
    int sprintDist = 4;

    int wallDistance = 0;

    if(facing == "North"){

        currentPlayer.setDirection("North");
        direction = "North";

        wallDistance = distToWall("North", xPos, yPos);

        if(speed == "Sneak"){ // [NORTH] SNEAK

            if((yPos - sneakDist) < 0){ // Out of bounds

                std::cout << "[ERROR] Out of Bounds" << std::endl;
                return false;

            }

            if(map[yPos - sneakDist][xPos] == ' '){

                currentPlayer.setYPos(yPos - sneakDist);
                changePlayerPosition(0, -sneakDist);

                map[yPos][xPos] = ' ';
                map[yPos - sneakDist][xPos] = '+';

                currentPlayer.subtractSteps(sneakDist);

                return true;

            }else{

                std::cout << "[ERROR] Spot is not empty!" << std::endl;
                return false;

            }

        }else if(speed == "Walk"){ // [NORTH] WALK

            if(distToWall("North", xPos, yPos) == -1){

                std::cout << "[ERROR] North Wall Dist" << std::endl;
                return false;

            }else{

                if(wallDistance > walkDist){

                    wallDistance = walkDist;

                }

                currentPlayer.subtractSteps(wallDistance);

                currentPlayer.setYPos(yPos - wallDistance);
                changePlayerPosition(0, - wallDistance);

                map[yPos][xPos] = ' ';
                map[yPos - wallDistance][xPos] = '+';

                return true;

            }

            // if(map[yPos - walkDist][xPos] == ' '){

            //     currentPlayer.setYPos(yPos - walkDist);
            //     changePlayerPosition(0, -walkDist);

            //     map[yPos][xPos] = ' ';
            //     map[yPos - walkDist][xPos] = '+';

            //     currentPlayer.subtractSteps(walkDist);

            //     return true;

            // }else{

                

            // }

        }else if(speed == "Sprint"){ // [NORTH] SPRINT

            if(distToWall("North", xPos, yPos) == -1){

                std::cout << "[ERROR] Out of Bounds (dist)" << std::endl;
                return false;

            }else{

                if(wallDistance > sprintDist){

                    wallDistance = sprintDist;

                }

                currentPlayer.subtractSteps(wallDistance);

                currentPlayer.setYPos(yPos - wallDistance);
                changePlayerPosition(0, - wallDistance);

                map[yPos][xPos] = ' ';
                map[yPos - wallDistance][xPos] = '+';


                return true;

            }

            // if(map[yPos - sprintDist][xPos] == ' '){

            //     currentPlayer.setYPos(yPos - sprintDist);
            //     changePlayerPosition(0, -sprintDist);

            //     map[yPos][xPos] = ' ';
            //     map[yPos - sprintDist][xPos] = '+';

            //     currentPlayer.subtractSteps(sprintDist);

            //     return true;

            // }else{

                

            // }

        }else{

            std::cout << "[ERROR] Invalid Speed!" << std::endl;
            return false;

        }

    }else if(facing == "East"){

        currentPlayer.setDirection("East");
        direction = "East";

        wallDistance = distToWall("East", xPos, yPos);

        if(speed == "Sneak"){ // [EAST] SNEAK

            if((xPos + sneakDist) >= colSize){ // Out of bounds

                std::cout << "[ERROR] Out of Bounds" << std::endl;
                return false;

            }

            if(map[yPos][xPos + sneakDist] == ' '){

                currentPlayer.setXPos(xPos + sneakDist);
                changePlayerPosition(sneakDist, 0);

                map[yPos][xPos] = ' ';
                map[yPos][xPos + sneakDist] = '+';

                currentPlayer.subtractSteps(sneakDist);

                return true;

            }else{

                std::cout << "[ERROR] Spot is not empty!" << std::endl;
                return false;

            }

        }else if(speed == "Walk"){ // [EAST] WALK

             if(distToWall("East", xPos, yPos) == -1){

                std::cout << "[ERROR] Out of Bounds (dist)" << std::endl;
                return false;

            }else{

                if(wallDistance > walkDist){

                    wallDistance = walkDist;

                }

                currentPlayer.subtractSteps(wallDistance);

                currentPlayer.setXPos(xPos + wallDistance);
                changePlayerPosition(wallDistance, 0);

                map[yPos][xPos] = ' ';
                map[yPos][xPos + wallDistance] = '+';

                return true;

            }

            // if(map[yPos][xPos + walkDist] == ' '){

            //     currentPlayer.setXPos(xPos + walkDist);
            //     changePlayerPosition(walkDist, 0);

            //     map[yPos][xPos] = ' ';
            //     map[yPos][xPos + walkDist] = '+';

            //     currentPlayer.subtractSteps(walkDist);

            //     return true;

            // }else{ // [East] Dist to wall

               

            // }

        }else if(speed == "Sprint"){ // [EAST] SPRINT

            if(distToWall("East", xPos, yPos) == -1){

                std::cout << "[ERROR] Out of Bounds (dist)" << std::endl;
                return false;

            }else{

                if(wallDistance > sprintDist){

                    wallDistance = sprintDist;

                }

                currentPlayer.subtractSteps(wallDistance);

                currentPlayer.setXPos(xPos + wallDistance);
                changePlayerPosition(wallDistance, 0);

                map[yPos][xPos] = ' ';
                map[yPos][xPos + wallDistance] = '+';

                return true;

            }

            // if(map[yPos][xPos + sprintDist] == ' '){

            //     currentPlayer.setXPos(xPos + sprintDist);
            //     changePlayerPosition(sprintDist, 0);

            //     map[yPos][xPos] = ' ';
            //     map[yPos][xPos + sprintDist] = '+';

            //     currentPlayer.subtractSteps(sprintDist);

            //     return true;

            // }else{

                

            // }

        }else{

            std::cout << "[ERROR] Invalid Speed!" << std::endl;
            return false;

        }

        

    }else if(facing == "South"){

        currentPlayer.setDirection("South");
        direction = "South";

        wallDistance = distToWall("South", xPos, yPos);

        if(speed == "Sneak"){ // [South] SNEAK

            if((yPos + sneakDist) >= rowSize){ // Out of bounds

                std::cout << "[ERROR] Out of Bounds" << std::endl;
                return false;

            }

            if(map[yPos + sneakDist][xPos] == ' '){

                currentPlayer.setYPos(yPos + sneakDist);
                changePlayerPosition(0, sneakDist);

                map[yPos][xPos] = ' ';
                map[yPos + sneakDist][xPos] = '+';

                currentPlayer.subtractSteps(sneakDist);

                return true;

            }else{

                return false;

            }

        }else if(speed == "Walk"){ // [South] WALK

            if(distToWall("South", xPos, yPos) == -1){

                std::cout << "[ERROR] Out of Bounds (dist)" << std::endl;
                return false;

            }else{

                if(wallDistance > walkDist){

                    wallDistance = walkDist;

                }

                currentPlayer.subtractSteps(wallDistance);

                currentPlayer.setYPos(yPos + wallDistance);
                changePlayerPosition(0, wallDistance);

                map[yPos][xPos] = ' ';
                map[yPos + wallDistance][xPos] = '+';

                return true;

            }

            // if(map[yPos + walkDist][xPos] == ' '){

            //     currentPlayer.setYPos(yPos + walkDist);
            //     changePlayerPosition(0, walkDist);

            //     map[yPos][xPos] = ' ';
            //     map[yPos + walkDist][xPos] = '+';

            //     currentPlayer.subtractSteps(walkDist);

            //     return true;

            // }else{

                

            // }

        }else if(speed == "Sprint"){ // [South] SPRINT

            if(distToWall("South", xPos, yPos) == -1){ // Checking distance to wall, going to nearest spot

                std::cout << "[ERROR] Out of Bounds (dist)" << std::endl;
                return false;

            }else{

                if(wallDistance > sprintDist){

                    wallDistance = sprintDist;

                }

                currentPlayer.subtractSteps(wallDistance);

                currentPlayer.setYPos(yPos + wallDistance);
                changePlayerPosition(0, wallDistance);

                map[yPos][xPos] = ' ';
                map[yPos + wallDistance][xPos] = '+';

                return true;

            }

            // if(map[yPos + sprintDist][xPos] == ' '){

            //     currentPlayer.setYPos(yPos + sprintDist);
            //     changePlayerPosition(0, sprintDist);

            //     map[yPos][xPos] = ' ';
            //     map[yPos + sprintDist][xPos] = '+';

            //     currentPlayer.subtractSteps(walkDist);

            //     return true;

            // }else{

                

            // }

        }else{

            std::cout << "[ERROR] Invalid Speed!" << std::endl;
            return false;

        }

    }else if(facing == "West"){

        currentPlayer.setDirection("West");
        direction = "West";

        wallDistance = distToWall("West", xPos, yPos);

        if(speed == "Sneak"){ // [West] SNEAK

            if((xPos - sneakDist) < 0){ // Out of bounds

                std::cout << "[ERROR] Out of Bounds" << std::endl;
                return false;

            }

            if(map[yPos][xPos - sneakDist] == ' '){

                currentPlayer.setXPos(xPos - sneakDist);
                changePlayerPosition(-sneakDist, 0);

                map[yPos][xPos] = ' ';
                map[yPos][xPos - sneakDist] = '+';

                currentPlayer.subtractSteps(sneakDist);

                return true;

            }else{

                std::cout << "[ERROR] Spot is not empty!" << std::endl;
                return false;

            }

        }else if(speed == "Walk"){ // [West] WALK

            if(distToWall("West", xPos, yPos) == -1){

                std::cout << "[ERROR] Out of Bounds (dist)" << std::endl;
                return false;

            }else{

                if(wallDistance > walkDist){

                    wallDistance = walkDist;

                }

                currentPlayer.subtractSteps(wallDistance);

                currentPlayer.setXPos(xPos - wallDistance);
                changePlayerPosition(-wallDistance, 0);

                map[yPos][xPos] = ' ';
                map[yPos][xPos - wallDistance] = '+';

                return true;

            }

            // if(map[yPos][xPos - walkDist] == ' '){

            //     currentPlayer.setXPos(xPos - walkDist);
            //     changePlayerPosition(-walkDist, 0);

            //     map[yPos][xPos] = ' ';
            //     map[yPos][xPos - walkDist] = '+';

            //     currentPlayer.subtractSteps(walkDist);

            //     return true;

            // }else{

                

            // }

        }else if(speed == "Sprint"){ // [West] SPRINT

            if(distToWall("West", xPos, yPos) == -1){

                std::cout << "[ERROR] Out of Bounds (dist)" << std::endl;
                return false;

            }else{

                if(wallDistance > sprintDist){

                    wallDistance = sprintDist;

                }

                currentPlayer.subtractSteps(wallDistance);

                currentPlayer.setXPos(xPos - wallDistance);
                changePlayerPosition(-wallDistance, 0);

                map[yPos][xPos] = ' ';
                map[yPos][xPos - wallDistance] = '+';

                return true;

            }

            // if(map[yPos][xPos - sprintDist] == ' '){

            //     currentPlayer.setXPos(xPos - sprintDist);
            //     changePlayerPosition(-sprintDist, 0);

            //     map[yPos][xPos] = ' ';
            //     map[yPos][xPos - sprintDist] = '+';

            //     currentPlayer.subtractSteps(sprintDist);

            //     return true;

            // }else{

                

            // }

        }else{

            std::cout << "[ERROR] Invalid Speed!" << std::endl;
            return false;

        }

    }else{

        std::cout << "[ERROR] Invalid Direction!" << std::endl;
        return false;

    }

}


int Grid::distToWall(std::string direction, int xPos_, int yPos_){

    int dist = 0;

    int xPos = xPos_;
    int yPos = yPos_;

    bool checking = true;

    if(direction == "North"){

        while(checking){

            if(yPos < 0){

                return -1;

            }

            if(map[yPos - 1][xPos] != ' '){

                return dist;

            }

            yPos--;

            dist++;

        }

    }else if(direction == "East"){

        while(checking){

            if(xPos >= colSize){

                return -1;

            }

            if(map[yPos][xPos + 1] != ' '){

                return dist;

            }

            xPos++;

            dist++;

        }

    }else if(direction == "South"){

        while(checking){

            if(yPos >= rowSize){

                return -1;

            }

            if(map[yPos + 1][xPos] != ' '){

                return dist;

            }

            yPos++;

            dist++;

        }

    }else if(direction == "West"){

        while(checking){

            if(xPos < 0){

                return -1;

            }

            if(map[yPos][xPos - 1] != ' '){

                return dist;

            }

            xPos--;

            dist++;

        }

    }

    std::cout << "[ERROR] Invalid Direction! (distToWall)" << std::endl;
    return -1;   

}

bool Grid::checkForNearbyItems(){

    int xPos = playerPosition.xPos;
    int yPos = playerPosition.yPos;

    if(getCell("North") == "[?]"){

        return true;

    }

    if(getCell("East") == "[?]"){

        return true;

    }
    
    if(getCell("South") == "[?]"){

        return true;

    }
    
    if(getCell("West") == "[?]"){

        return true;

    }

    if(getCell(xPos + 1, yPos + 1) == "[?]"){ // Top right

        return true;

    }

    if(getCell(xPos - 1, yPos + 1) == "[?]"){ // Top left

        return true;

    }

    if(getCell(xPos + 1, yPos - 1) == "[?]"){ // Bottom right

        return true;

    }

    if(getCell(xPos - 1, yPos - 1) == "[?]"){ // Bottom Left

        return true;

    }

    return false;

}

bool Grid::modFixCheck(std::string modName){

    if(modName == "cryo"){

        return beginningAccomplished;

    }

    if(modName == "reactor"){

        return reactorFixed;

    }

    if(modName == "doors"){

        return scienceDoorsOpen;

    }

    if(modName == "weapons"){

        return weaponsFixed;

    }

    if(modName == "armory"){

        return armoryUnlocked;

    }

    if(modName == "override"){

        return overrideComplete;

    }

    if(modName == "cure"){

        return cureFound;

    }
    
    return false;

}

Coordinates Grid::nearbyCoords(std::string objectCell){

    int xPos = playerPosition.xPos;
    int yPos = playerPosition.yPos;

    Coordinates coords;

    if(getCell("North") == objectCell ){

        coords.xPos = xPos;
        coords.yPos = yPos - 1;

        return coords;

    }

    if(getCell("East") == objectCell ){

        coords.xPos = xPos + 1;
        coords.yPos = yPos;

        return coords;

    }
    
    if(getCell("South") == objectCell ){

        coords.xPos = xPos;
        coords.yPos = yPos + 1;

        return coords;
    }
    
    if(getCell("West") == objectCell ){

        coords.xPos = xPos - 1;
        coords.yPos = yPos;

        return coords;

    }

    if(getCell(xPos + 1, yPos + 1) == objectCell ){ // Top right

        coords.xPos = xPos + 1;
        coords.yPos = yPos + 1;

        return coords;

    }

    if(getCell(xPos - 1, yPos + 1) == objectCell ){ // Top left

        coords.xPos = xPos - 1;
        coords.yPos = yPos + 1;

        return coords;

    }

    if(getCell(xPos + 1, yPos - 1) == objectCell ){ // Bottom right

        coords.xPos = xPos + 1;
        coords.yPos = yPos - 1;

        return coords;

    }

    if(getCell(xPos - 1, yPos - 1) == objectCell ){ // Bottom Left

        coords.xPos = xPos - 1;
        coords.yPos = yPos - 1;

        return coords;

    }

    coords.xPos = -1;
    coords.yPos = -1;

    return coords;

}

void Grid::printPlrPos(){

    std::cout << "[MAP Plr Pos] = (" << playerPosition.xPos << ", " << playerPosition.yPos << ")" << std::endl;

}

bool Grid::checkForNearbyAliens(){

    int xPos = playerPosition.xPos;
    int yPos = playerPosition.yPos;

    std::string alienCell = "[@]";

    if(getCell("North") == alienCell ){

        return true;

    }

    if(getCell("East") == alienCell ){

        return true;

    }
    
    if(getCell("South") == alienCell ){

        return true;

    }
    
    if(getCell("West") == alienCell ){

        return true;

    }

    if(getCell(xPos + 1, yPos + 1) == alienCell ){ // Top right

        return true;

    }

    if(getCell(xPos - 1, yPos + 1) == alienCell ){ // Top left

        return true;

    }

    if(getCell(xPos + 1, yPos - 1) == alienCell ){ // Bottom right

        return true;

    }

    if(getCell(xPos - 1, yPos - 1) == alienCell ){ // Bottom Left

        return true;

    }

    return false;

}

bool Grid::checkForNearbyDoors(){

    int xPos = playerPosition.xPos;
    int yPos = playerPosition.yPos;

    std::string doorCell = "[#]";

    if(getCell("North") == doorCell ){

        return true;

    }

    if(getCell("East") == doorCell ){

        return true;

    }
    
    if(getCell("South") == doorCell ){

        return true;

    }
    
    if(getCell("West") == doorCell ){

        return true;

    }

    if(getCell(xPos + 1, yPos + 1) == doorCell ){ // Top right

        return true;

    }

    if(getCell(xPos - 1, yPos + 1) == doorCell ){ // Top left

        return true;

    }

    if(getCell(xPos + 1, yPos - 1) == doorCell ){ // Bottom right

        return true;

    }

    if(getCell(xPos - 1, yPos - 1) == doorCell ){ // Bottom Left

        return true;

    }

    return false;

}

bool Grid::checkForNearbyModules(){

    int xPos = playerPosition.xPos;
    int yPos = playerPosition.yPos;

    std::string modCell = "[[]";
    std::string modCell2 = "[]]";

    if(getCell("South") == "[*]" && getCell(xPos + 1, yPos + 1) == "[=]" && getCell(xPos - 1, yPos + 1) == "[=]"){

        return true;

    }

    if(getCell("North") == modCell || getCell("North") == modCell2 ){

        return true;

    }

    if(getCell("East") == modCell || getCell("East") == modCell2 ){

        return true;

    }
    
    if(getCell("South") == modCell || getCell("South") == modCell2 ){

        return true;

    }
    
    if(getCell("West") == modCell || getCell("West") == modCell2 ){

        return true;

    }

    if(getCell(xPos + 1, yPos + 1) == modCell || getCell(xPos + 1, yPos + 1) == modCell2){ // Top right

        return true;

    }

    if(getCell(xPos - 1, yPos + 1) == modCell || getCell(xPos - 1, yPos + 1) == modCell2 ){ // Top left

        return true;

    }

    if(getCell(xPos + 1, yPos - 1) == modCell || getCell(xPos + 1, yPos - 1) == modCell2 ){ // Bottom right

        return true;

    }

    if(getCell(xPos - 1, yPos - 1) == modCell || getCell(xPos - 1, yPos - 1) == modCell2 ){ // Bottom Left

        return true;

    }

    return false;

}

void Grid::changeMap(Player &currentPlayer, std::vector<std::vector<Cell>> anomolies){

    //0. Cryochambers
    //1. Reactor
    //2. Science
    //3. Weapons
    //4. Quarters
    //5. Thrusters

    int cryo = 0;
    int reactor = 1;
    int science = 2;
    int weapons = 3;
    int quarters = 4;
    int thrusters = 5;

    validFlares.clear();

    Coordinates coordsHolder; 

    coordsHolder = nearbyCoords("[#]"); // Finds the coords of nearest door char

    if(currMap == "Cryochambers.txt"){ // First Room Doors

        if( coordsHolder.yPos == 19){

            loadMap("Reactor.txt", anomolies[reactor]);

            currentPlayer.setXPos(21);
            currentPlayer.setYPos(1);

            drawPlayer(currentPlayer);

        }else if( coordsHolder.xPos == 49){

            loadMap("Weapons.txt", anomolies[weapons]);

            currentPlayer.setXPos(2);
            currentPlayer.setYPos(3);

            drawPlayer(currentPlayer);

        }

    }else if(currMap == "Reactor.txt"){ // Reactor Doors

        if( coordsHolder.xPos == 49){

            loadMap("Science.txt", anomolies[science]);

            currentPlayer.setXPos(1);
            currentPlayer.setYPos(9);

            drawPlayer(currentPlayer);

        }else if( coordsHolder.yPos == 0){

            loadMap("Cryochambers.txt", anomolies[cryo]);

            currentPlayer.setXPos(21);
            currentPlayer.setYPos(18);

            drawPlayer(currentPlayer);

        }else{

            std::cout << coordsHolder.yPos << std::endl;

        }

    }else if(currMap == "Science.txt"){ // Science Lab Doors

        if( coordsHolder.xPos == 0){

            loadMap("Reactor.txt", anomolies[reactor]);

            currentPlayer.setXPos(48);
            currentPlayer.setYPos(9);

            drawPlayer(currentPlayer);

        }else{

            std::cout << coordsHolder.xPos << std::endl;

        }

    }else if(currMap == "Weapons.txt"){ // Weapons Doors

        if( coordsHolder.xPos == 0){

            loadMap("Cryochambers.txt", anomolies[cryo]);

            currentPlayer.setXPos(48);
            currentPlayer.setYPos(3);

            drawPlayer(currentPlayer);

        }else if( coordsHolder.yPos == 2){

            loadMap("Quarters.txt", anomolies[quarters]);

            currentPlayer.setXPos(22);
            currentPlayer.setYPos(18);

            drawPlayer(currentPlayer);

        }else if( coordsHolder.xPos == 49){

            loadMap("Thrusters.txt", anomolies[thrusters]);

            currentPlayer.setXPos(1);
            currentPlayer.setYPos(3);

            drawPlayer(currentPlayer);

        }

    }else if(currMap == "Quarters.txt"){ // Quarters Doors

        if( coordsHolder.yPos == 19){

            loadMap("Weapons.txt", anomolies[weapons]);

            currentPlayer.setXPos(21);
            currentPlayer.setYPos(3);

            drawPlayer(currentPlayer);

        }

    }else if(currMap == "Thrusters.txt"){ // Thrusters Doors

        if(coordsHolder.xPos == 0){

            loadMap("Weapons.txt", anomolies[weapons]);

            currentPlayer.setXPos(48);
            currentPlayer.setYPos(3);

            drawPlayer(currentPlayer);

        }
    }
}

void Grid::changeMap(Player &currentPlayer, std::vector<std::vector<Cell>> anomolies, std::string mapName){

    //0. Cryochambers
    //1. Reactor
    //2. Science
    //3. Weapons
    //4. Quarters
    //5. Thrusters

    int cryo = 0;
    int reactor = 1;
    int science = 2;
    int weapons = 3;
    int quarters = 4;
    int thrusters = 5;

    validFlares.clear();

    if(strLowerGrid(mapName) == "cryochambers" || strLowerGrid(mapName) == "cryo"){ // Cryo 

        loadMap("Cryochambers.txt", anomolies[cryo]);  

        currentPlayer.setXPos(21);
        currentPlayer.setYPos(18);

        drawPlayer(currentPlayer);

    }else if(strLowerGrid(mapName) == "reactor"){ // Reactor 

        loadMap("Reactor.txt", anomolies[reactor]);

        currentPlayer.setXPos(21);
        currentPlayer.setYPos(1);

        drawPlayer(currentPlayer);

    }else if(strLowerGrid(mapName) == "science"){ // Science Lab Doors

        loadMap("Science.txt", anomolies[science]);

        currentPlayer.setXPos(1);
        currentPlayer.setYPos(9);

        drawPlayer(currentPlayer);

    }else if(strLowerGrid(mapName) == "weapons"){ // Weapons Doors

        loadMap("Weapons.txt", anomolies[weapons]);

        currentPlayer.setXPos(2);
        currentPlayer.setYPos(3);

        drawPlayer(currentPlayer);

    }else if(strLowerGrid(mapName) == "quarters"){ // Quarters Doors

        loadMap("Quarters.txt", anomolies[quarters]);

        currentPlayer.setXPos(22);
        currentPlayer.setYPos(18);

        drawPlayer(currentPlayer);

    }else if(strLowerGrid(mapName) == "thrusters"){ // Thrusters Doors

        loadMap("Thrusters.txt", anomolies[thrusters]);

        currentPlayer.setXPos(1);
        currentPlayer.setYPos(3);

        drawPlayer(currentPlayer);

    }else{

        std::cout << "[INVALID MAP NAME]" << std::endl;

    }
}

void modSpacer(){

    for(int i = 0; i < 8; i++){

        std::cout << std::endl;

    }

}

std::string Grid::modGame(Player &currentPlayer){

    char buttonPress = 'a';
    std::string input = "";

    if(nearbyCoords("[^]").xPos != -1 && !reactorFixed){ // Power Module

        std::cout << "[SYSTEM MESSAGE]: Welcome to the Centralized Fusion Reactor!" << std::endl;
        std::cout << "[POWER LEVEL]: 0%" << std::endl;
        std::cout << "[SYSTEM MESSAGE]: Please input a fuel cell into the cell chamber." << std::endl;

        currentPlayer.printInventory();

        getline(std::cin, input);

        while(strLowerGrid(input) != "tritium cell" && strLowerGrid(input) != "exit" ){

            std::cout << std::endl;

            std::cout << "Non-viable fuel source, please place a Tritium based cell into the chamber." << std::endl;

            getline(std::cin, input);

            std::cout << std::endl;

        }

        if(strLowerGrid(input) == "exit"){

            std::cout << "[SYSTEM MESSAGE]: Goodbye!" << std::endl;

            return "";

        }

        if(!currentPlayer.getInventoryItem("Tritium Cell")){

            std::cout << "You do not have the fuel source! The cell is contained within the Thruster bay." << std::endl;
            return "";

        }

        std::cout << std::endl;

        std::cout << "[POWER LEVEL]: 100%" << std::endl;
        std::cout << "[SYSTEM MESSAGE]: Power restored!" << std::endl;
        std::cout << std::endl;
        std::cout << "[ENTER TO CONTINUE]" << std::endl;
        getline(std::cin, input);

        reactorFixed = true;

        return "powerEnabled";

    }else if(!reactorFixed && !(nearbyCoords("[<]").xPos != -1 || nearbyCoords("[>]").xPos != -1)){ // If reactor not fixed and not beginning module

        std::cout << "Hmm, this module needs power. I need to put a cell in the Reactor and the cells got left in the thruster bay." << std::endl;
        return "";

    }

    if(nearbyCoords("[$]").xPos != -1){ // Reactor Locked Door Game

        std::cout << "[DOOR LOCK CONTROLS]" << std::endl;
        std::cout << "|Enter admin code to unlock doors or type exit to leave: ";

        getline(std::cin, input);

        while(input != "5783" && input != "exit"){

            std::cout << "Incorrect code! Please input the correct code!" << std::endl;

            getline(std::cin, input);

        }

        if(input == "exit"){

            std::cout << "Leaving program..." << std::endl;
            return "";

        }

        std::cout << "Access Granted! Doors unlocking..." << std::endl;
        scienceDoorsOpen = true;

        return "doors";

    }else if(nearbyCoords("[<]").xPos != -1 || nearbyCoords("[>]").xPos != -1){ // Beginning Module

        if(beginningAccomplished){

            std::cout << std::endl;
            std::cout << std::endl;

            std::cout << "[EMERGENCY POWER ACTIVATED]" << std::endl;
            std::cout << "[RELOADING PRE-MADE MESSAGE]..." << std::endl;
            std::cout << "[Captain Acker]: Hello crew! I hope you have brought all you need and are all settled in." << std::endl;
            std::cout << "[Captain Acker]: Once we make contact, I will have the scientists collect a sample using our specially designed syringe." << std::endl;
            std::cout << "[Captain Acker]: This will be left in the science lab, which is Southeast of the cryo-chambers. Thanks again for your dedication and volunteerism!" << std::endl;
            
            return "";

        }

        std::cout << "Unscramble the text: [ Y G T V I A R ] or type exit to leave." << std::endl;

        getline(std::cin, input);

        while(strLowerGrid(input) != "gravity" && input != "exit"){

            std::cout << std::endl;
            std::cout << "Incorrect solution! Try Again: [ Y G T V I A R ]" << std::endl;
            std::cout << std::endl;
            getline(std::cin, input);

        }

        if(input == "exit"){

            std::cout << "Leaving program..." << std::endl;
            return "";

        }

        std::cout << std::endl;
        std::cout << std::endl;

        std::cout << "[EMERGENCY POWER ACTIVATED]" << std::endl;
        std::cout << "[LOADING PRE-MADE MESSAGE]..." << std::endl;
        std::cout << "[Captain Acker]: Hello crew! I hope you have brought all you need and are all settled in." << std::endl;
        std::cout << "[Captain Acker]: Once we make contact, I will have the scientists collect a sample using our specially designed syringe." << std::endl;
        std::cout << "[Captain Acker]: This will be left in the science lab, which is Southeast of the cryo-chambers. Thanks again for your dedication and volunteerism!" << std::endl;

        std::cout << std::endl;
        std::cout << "Well, I guess it's just me now. Seems like the power is off, I need to put a cell into the Reactor. I last saw the cell in the Thruster bay." << std::endl;
        std::cout << std::endl;
        beginningAccomplished = true;

        return "beginning";

    }else if(nearbyCoords("[=]").xPos != -1 && nearbyCoords("[*]").xPos != -1){ // Weapons Module

        bool movingBarrel = true;
        bool leftOrRight = true;

        std::string topBarrel = "      ";
        std::string midBarrel = " ";
        std::string bottomBarrel = "       ";

        std::cout << std::endl;
        std::cout << "[SENTRY SYSTEM OFFLINE] MAJOR ALARM TRIGGERED" << std::endl;
        std::cout << "= BARREL MISALIGNED = CHOOSE CORRECT ALIGNMENT =" << std::endl;

        while(movingBarrel){

            std::cout << std::endl;
            std::cout << "[BARREL CONFIGURATION]" << std::endl;
            std::cout << "-------[  ]------- [MAIN CONNECTION PORT]" << std::endl;
            std::cout << topBarrel<< "==|  |== [1]" << std::endl;
            std::cout << midBarrel << "==|  |== [2]" << std::endl;
            std::cout << bottomBarrel << "==|  |== [3]" << std::endl;
            std::cout << std::endl;

            std::cout << "Press 1-3 to move a certain barrel or X to exit" << std::endl;

            buttonPress = _getch();
            input = buttonPress;

            while(input != "1" && input != "2" && input != "3" && strLowerGrid(input) != "x"){

                std::cout << "Please press 1-3 to move a certain barrel or X to exit" << std::endl;

                buttonPress = _getch();
                input = buttonPress;

            }

            if(input == "1"){

                leftOrRight = true;

                while(leftOrRight){

                    std::cout << "[MOVING BARREL 1] L to move left, R to move right, X to leave barrel" << std::endl;

                    buttonPress = _getch();
                    input = buttonPress;

                    while(strLowerGrid(input) != "l" && strLowerGrid(input) != "r" && strLowerGrid(input) != "x"){

                        std::cout << "Please press L or R or X to exit barrel" << std::endl;

                        buttonPress = _getch();
                        input = buttonPress;

                    }

                    if(strLowerGrid(input) == "l" && topBarrel.length() > 0){

                        topBarrel = topBarrel.substr(1, topBarrel.length() - 1);

                    }else if(strLowerGrid(input) == "r" && topBarrel.length() <= 11){

                        topBarrel = " " + topBarrel;

                    }

                    std::cout << std::endl;
                    std::cout << "[BARREL CONFIGURATION]" << std::endl;
                    std::cout << "-------[  ]------- [MAIN CONNECTION PORT]" << std::endl;
                    std::cout << topBarrel<< "==|  |== [1]" << std::endl;
                    std::cout << midBarrel << "==|  |== [2]" << std::endl;
                    std::cout << bottomBarrel << "==|  |== [3]" << std::endl;
                    std::cout << std::endl;

                    if(strLowerGrid(input) == "x"){

                        leftOrRight = false;
                        modSpacer();

                    }

                }

            }else if(input == "2"){

                leftOrRight = true;

                while(leftOrRight){

                    std::cout << "[MOVING BARREL 2] L to move left, R to move right, X to leave barrel" << std::endl;

                    buttonPress = _getch();
                    input = buttonPress;

                    while(strLowerGrid(input) != "l" && strLowerGrid(input) != "r" && strLowerGrid(input) != "x"){

                        std::cout << "Please press L or R or X to exit barrel" << std::endl;

                        buttonPress = _getch();
                        input = buttonPress;

                    }

                    if(strLowerGrid(input) == "l" && midBarrel.length() > 0){

                        midBarrel = midBarrel.substr(1, midBarrel.length() - 1);

                    }else if(strLowerGrid(input) == "r" && midBarrel.length() <= 11){

                        midBarrel = " " + midBarrel;

                    }

                    std::cout << std::endl;
                    std::cout << "[BARREL CONFIGURATION]" << std::endl;
                    std::cout << "-------[  ]------- [MAIN CONNECTION PORT]" << std::endl;
                    std::cout << topBarrel<< "==|  |== [1]" << std::endl;
                    std::cout << midBarrel << "==|  |== [2]" << std::endl;
                    std::cout << bottomBarrel << "==|  |== [3]" << std::endl;
                    std::cout << std::endl;

                    if(strLowerGrid(input) == "x"){

                        leftOrRight = false;
                        modSpacer();

                    }

                }

            }else if(input == "3"){

                leftOrRight = true;

                while(leftOrRight){

                    std::cout << "[MOVING BARREL 3] L to move left, R to move right, X to leave barrel" << std::endl;

                    buttonPress = _getch();
                    input = buttonPress;

                    while(strLowerGrid(input) != "l" && strLowerGrid(input) != "r" && strLowerGrid(input) != "x"){

                        std::cout << "Please type L or R or X to exit barrel" << std::endl;

                        buttonPress = _getch();
                        input = buttonPress;

                    }

                    if(strLowerGrid(input) == "l" && bottomBarrel.length() > 0){

                        bottomBarrel = bottomBarrel.substr(1, bottomBarrel.length() - 1);

                    }else if(strLowerGrid(input) == "r" && bottomBarrel.length() <= 11){

                        bottomBarrel = " " + bottomBarrel;

                    }

                    std::cout << std::endl;
                    std::cout << "[BARREL CONFIGURATION]" << std::endl;
                    std::cout << "-------[  ]-------" << std::endl;
                    std::cout << topBarrel<< "==|  |==" << std::endl;
                    std::cout << midBarrel << "==|  |==" << std::endl;
                    std::cout << bottomBarrel << "==|  |==" << std::endl;
                    std::cout << std::endl;

                    if(strLowerGrid(input) == "x"){

                        leftOrRight = false;
                        modSpacer();

                    }

                }

            }else{

                std::cout << "Exiting Barrel Alignment..." << std::endl;
                return "";

            }

            if(topBarrel.length() == 5 && midBarrel.length() == 5 && bottomBarrel.length() == 5){

                std::cout <<"[SENTRY SYSTEM ONLINE] BARREL ALIGNED" << std::endl;

                weaponsFixed = true;

                return "weapons";

            }

        }

    }else if(nearbyCoords("[~]").xPos != -1){ // Quarters Module

        if(!overrideComplete){

            std::cout << "[SYSTEM MESSAGE]: POWER RESET DETECTED" << std::endl;
            std::cout << "[SYSTEM MESSAGE]: PERSONAL ARMORY LOCKED" << std::endl;
            std::cout << std::endl;
            std::cout << "Maybe I can override this. I last remember the keys in the Corporals room. I'll check there." << std::endl;

            return "";

        }

        if(armoryUnlocked){

            std::cout << "[SYSTEM MESSAGE]: WeCOme CaaIn ACeR!" << std::endl;
            std::cout << "[POWER ERROR]: ARMORY MOTOR BLOWN" << std::endl;
            std::cout << std::endl;
            std::cout << "Damn, guess I'll stick with what I got..." << std::endl;

            return "";

        }

        std::cout << "[SYSTEM MESSAGE]: Welcome Captain Acker!" << std::endl;
        std::cout << "[SYSTEM MESSAGE]: Please pick which locker to open or type EXIT:" << std::endl;
        std::cout << "[LOCKER 1]: Flamethrower, Flares" << std::endl;
        std::cout << "[LOCKER 2]: Shotgun, Medkit" << std::endl;

        getline(std::cin, input);

        while(input != "1" && input != "2" && strLowerGrid(input) != "exit"){

            std::cout << std::endl;

            std::cout << "Please type 1 or 2 or exit" << std::endl;
            
            getline(std::cin, input);

        }

        if(strLowerGrid(input) == "exit"){

            std::cout << "[SYSTEM MESSAGE]: Goodbye!" << std::endl;
            return "";

        }

        armoryUnlocked = true;

        if(input == "1"){

            std::cout << "[SYSTEM MESSAGE]: Opening Locker 1..." << std::endl;

            return "locker1";

        }

        if(input == "2"){

            std::cout << "[SYSTEM MESSAGE]: Opening Locker 2..." << std::endl;

            return "locker2";

        }

    }else if(nearbyCoords("[!]").xPos != -1 ){ // Override Module

        if(!currentPlayer.getInventoryItem("Override Key")){

            std::cout << "This is the override panel. It looks like there's a key slot. There should be a key in the Corporals room." << std::endl;
            return "";

        }

        std::cout << "The key slot on the override panel seems to match the size of the key I found. Let's try it." << std::endl;
        std::cout << " *The key turns and a loud ping sounds from the panel" << std::endl;

        overrideComplete = true;

        return "override";

    }else if(nearbyCoords("[%]").xPos != -1){ // Syringe Module

        if(!overrideComplete){

            std::cout << "[SYSTEM ERROR]: Credentials for Auto Science Desk locked, please compelete override procedures located in the Thruster Bay." << std::endl;
            std::cout << std::endl;

            std::cout << "Hm, I last remember the keys in the Corporals room. I'll check there." << std::endl;
            return "";

        }

        std::cout << "[SYSTEM MESSAGE]: Welcome Captain Acker!" << std::endl;
        std::cout << "[SYSTEM MESSAGE]: Please insert alien sample into the Auto Science Desk or type Exit:" << std::endl;
        std::cout << std::endl;

        currentPlayer.printInventory();

        getline(std::cin, input);

        while(strLowerGrid(input) != "toxic syringe" && strLowerGrid(input) != "exit"){

            std::cout << std::endl;
            std::cout << "[SYSTEM MESSAGE]: Non Bio Compounds Found! Please enter an alien sample exit." << std::endl;

            getline(std::cin, input);

        }

        if(strLowerGrid(input) == "exit"){

            std::cout << "[SYSTEM MESSAGE]: Goodbye!" << std::endl;
            return "";

        }

        if(!currentPlayer.getInventoryItem("Toxic Syringe")){

            std::cout << "[SYSTEM MESSAGE]: You do not have the items required! Go collect a sample with your syringe!" << std::endl;
            return "";

        }

        std::cout << "[SYSTEM MESSAGE]: Processing Organics..." << std::endl;
        std::cout << "[SYSTEM MESSAGE]: Removing keratin..." << std::endl;
        std::cout << "[SYSTEM MESSAGE]: Yielding product..." << std::endl;
        std::cout << std::endl;
        std::cout << "[CURE COMPLETE]" << std::endl;
        std::cout << std::endl;
        std::cout << "[SYSTEM MESSAGE]: Depositing cure..." << std::endl;

        currentPlayer.addToInventory("Canister of Blight Cure");

        std::cout << std::endl;
        std::cout << "[Canister of Blight Cure Acquired!]" << std::endl;

        cureFound = true;

        return "cure";

    }else if(nearbyCoords("[0]").xPos != -1){ // Module Already Complete

        std::cout << "This module is already completed!" << std::endl;
        return "";

    }
    

    std::cout << "Move closer to the module!" << std::endl;
    return "";
}

Coordinates createCoords(int xPos_, int yPos_){

    Coordinates newCoords(xPos_, yPos_);

    return  newCoords;

}

int Grid::mapToIndex(){

    //0. Cryochambers
    //1. Reactor
    //2. Science
    //3. Weapons
    //4. Quarters
    //5. Thrusters

    if(currMap == "Cryochambers.txt"){
        return 0;
    }

    if(currMap == "Reactor.txt"){
        return 1;
    }

    if(currMap == "Science.txt"){
        return 2;
    }

    if(currMap == "Weapons.txt"){
        return 3;
    }

    if(currMap == "Quarters.txt"){
        return 4;
    }

    if(currMap == "Thrusters.txt"){
        return 5;
    }

    return 0;

}

void Grid::updateVisibility(){

    int xPosition = playerPosition.xPos;
    int yPosition = playerPosition.yPos;

    int xEquation = 0;
    int yEquation = 0;

    for(int i = -2; i < 3; i++){ // Row or Y position

        for(int n = -6; n < 7; n++){ // Col or X position

            if(xPosition + n >= 0 && xPosition + n < colSize && yPosition + i >= 0 && yPosition + i < rowSize){ // If the coords in the box are within map boundaries

                xEquation = (( (xPosition + n) - xPosition) * ( (xPosition + n) - xPosition)) / 36;
                yEquation = (( (yPosition + i) - yPosition) * ( (yPosition + i) - yPosition)) / 9;

                if(xEquation + yEquation <= 1){ // If the coords are within the circle, set them to true

                    visiMap[mapToIndex()][yPosition + i][xPosition + n] = true;

                } 
            }
        }
    }
}

bool Grid::withinFlareCircle(Coordinates flareCenter, int xPos, int yPos){

    int xEquation = ( ((xPos - flareCenter.xPos) * (xPos - flareCenter.xPos)) /100  );

    int yEquation = ( ((yPos - flareCenter.yPos) * (yPos - flareCenter.yPos)) /9);

    return ( xEquation + yEquation <= 1);

}

bool Grid::throwFlare(){

    if(distToWall(direction, playerPosition.xPos, playerPosition.yPos) < 1){

        return false;

    }

    Cell flareCell;

    flareCell.character = ';';

    if(direction == "North"){

        if(distToWall(direction, playerPosition.xPos, playerPosition.yPos) > 4){

            flareCell.coords.xPos = playerPosition.xPos;
            flareCell.coords.yPos = playerPosition.yPos - 4;

        }else{

            flareCell.coords.xPos = playerPosition.xPos;
            flareCell.coords.yPos = playerPosition.yPos - distToWall(direction, playerPosition.xPos, playerPosition.yPos);

        }

    }else if(direction == "East"){

        if(distToWall(direction, playerPosition.xPos, playerPosition.yPos) > 4){

            flareCell.coords.xPos = playerPosition.xPos + 4;
            flareCell.coords.yPos = playerPosition.yPos;

        }else{

            flareCell.coords.xPos = playerPosition.xPos + distToWall(direction, playerPosition.xPos, playerPosition.yPos);
            flareCell.coords.yPos = playerPosition.yPos;

        }

    }else if(direction == "South"){

        if(distToWall(direction, playerPosition.xPos, playerPosition.yPos) > 4){

            flareCell.coords.xPos = playerPosition.xPos;
            flareCell.coords.yPos = playerPosition.yPos + 4;

        }else{

            flareCell.coords.xPos = playerPosition.xPos;
            flareCell.coords.yPos = playerPosition.yPos + distToWall(direction, playerPosition.xPos, playerPosition.yPos);

        }

    }else if(direction == "West"){

        if(distToWall(direction, playerPosition.xPos, playerPosition.yPos) > 4){

            flareCell.coords.xPos = playerPosition.xPos - 4;
            flareCell.coords.yPos = playerPosition.yPos;

        }else{

            flareCell.coords.xPos = playerPosition.xPos + distToWall(direction, playerPosition.xPos, playerPosition.yPos);
            flareCell.coords.yPos = playerPosition.yPos;

        }

    }

    validFlares.push_back(flareCell);

    return true;

}

Cell Grid::getRecentFlare(){

    return validFlares.back();

}

bool Grid::checkAllModFixed(){

    return (beginningAccomplished && reactorFixed && scienceDoorsOpen && cureFound && weaponsFixed && overrideComplete);

}