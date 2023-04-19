#include <iostream>
#include "Player.h"


Player::Player(){

    health = 100;

    steps = 350;

    speed = "Walk";

    direction = "North";

    xPos = -1;
    yPos = -1;

    equippedItem = "Fists";

}

std::string Player::PLR_strLower(std::string inputString){

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

std::string PLR_fixString(std::string inputString){

    int stringLength = inputString.length();

    std::string result = "";

    bool cap = true;

    for(int i = 0; i < stringLength; i++){

        // If space then next is capital. If cap is false, set to lower

        if(inputString[i] == ' '){ // If space, next letter is cap

            cap = true;
            result += inputString[i];

        }else if(cap && inputString[0] >= 'a' && inputString[0] <= 'z'){ // If it needs cap and isn't cap

            result += inputString[i] - 32;
            cap = false;

        }else if(!cap && inputString[i] >= 'A' && inputString[i] <= 'Z'){ // If it doesn't need cap and is cap

            result += inputString[i] + 32;

        }else{ // If else doesn't apply, just add

            result += inputString[i];
            cap = false;

        }
        
    }

    return result;

}

int Player::getHealth(){

    return health;

}

int Player::getSteps(){

    return steps;

}

std::string Player::getSpeed(){

    return speed;

}

int Player::getXPos(){

    return xPos;

}

int Player::getYPos(){

    return yPos;

}

std::string Player::getDirection(){

    return direction;

}

std::string Player::getEquipped(){

    return equippedItem;

}

bool Player::getInventoryItem(std::string itemName){

    if(inventory.size() <= 0){

        std::cout << "Inventory is empty!" << std::endl;
        return false;

    }

    std::string loweredItem = PLR_strLower(itemName);

    for(int i = 0; i < inventory.size(); i++){

        if(PLR_strLower(inventory[i]) == (loweredItem)){

            return true;

        }

    }

    return false;

}

bool Player::addToInventory(std::string itemName){

    if(inventory.size() == 0){ // Backpack is empty, add it.

        inventory.push_back(itemName);
        return true;
        
    }

    for(int i = 0; i < inventory.size(); i++){ // Checking if item exists.

        if(inventory[i] == itemName){

            return false;

        }

    }

    inventory.push_back(itemName);

    return true;

}

bool Player::removeFromInventory(std::string itemName){

    if(inventory.size() == 0){

        return false;

    }

    for(int i = 0; i < inventory.size(); i++){

        if(inventory[i] == itemName){

            inventory.erase(inventory.begin() + i);
            return true;

        }

    }

    return false;

}

void Player::setHealth(int health_){

    health = health_;

}

void Player::setSteps(int steps_){

    steps = steps_;

}

void Player::setDirection(std::string direction_){

    direction = direction_;

}

void Player::setSpeed(std::string speed_){

    speed = speed_;

}

void Player::setXPos(int xPos_){

    xPos = xPos_;
    
}

void Player::setYPos(int yPos_){

    yPos = yPos_;

}

bool Player::setEquipped(std::string itemName){

    std::string loweredItem = PLR_strLower(itemName);

    if(getInventoryItem(loweredItem)){

        if(loweredItem == "crow bar" || loweredItem == "shotgun" || loweredItem == "flame thrower"){

            equippedItem = PLR_fixString(itemName);
            return true;

        }
    }

    return false;

}

void Player::subtractSteps(int removeCount){

    steps -= removeCount;

}

void Player::printInventory(){

    if(inventory.size() <= 0){

        std::cout << "Your inventory is empty!" << std::endl;
        std::cout << std::endl;
        return;

    }

    std::cout << "[Inventory]:" << std::endl;
    for(int i = 0; i < inventory.size(); i++){

        std::cout << " =[ " << inventory[i] << " ]= " << std::endl;

    }

}

void Player::printHealthBar(){

    if(health <= 0){

        return;

    }

    int barSegments = health / 5;

    std::cout << "YOUR HEALTH: " << health << " [";

    for(int i = 0; i < barSegments; i++){

        std::cout << "+";

    }

    if(barSegments < 20){

        for(int i = 0; i < 20 - barSegments; i++){

            std::cout << " ";

        }

    }

    std::cout << "]" << std::endl;

}