#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include <string>
#include <vector>

class Player{

    public:


        Player();
        std::string PLR_strLower(std::string input);
        int getHealth();
        int getSteps();
        std::string getSpeed();
        int getXPos();
        int getYPos();
        std::string getDirection();
        std::string getEquipped();
        bool getInventoryItem(std::string itemName);

        bool addToInventory(std::string itemName);
        bool removeFromInventory(std::string itemName);

        void setHealth(int health_);
        void setSteps(int steps_);
        void setDirection(std::string direction_);
        void setSpeed(std::string speed_);
        void setXPos(int xPos_);
        void setYPos(int yPos_);
        bool setEquipped(std::string itemName);

        void subtractSteps(int removeCount); // Returns false if not tired, returns true if tired.

        void printInventory();

        void printHealthBar();

    private:

        int health;

        int steps;

        std::string speed;
        std::string direction;

        int xPos;
        int yPos;

        std::vector<std::string> inventory;

        std::string equippedItem;

};


#endif