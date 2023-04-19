#ifndef ALIEN_H
#define ALIEN_H

#include <iostream>
#include "Grid.h"

class Alien{

    public:

        Alien();

        int getHealth();
        int getDamage();
        double getCritChance();
        Coordinates getCoords();

        void setHealth(int health_);
        void setDamage(int damage_);
        void setCritChance(double critChance_);
        void setCoords(Coordinates coords_);
        void setCoords(int xPos, int yPos);

        void printHealthBar();

    private:

        int health;
        int damage;
        double critChance;
        Coordinates coords;
};

#endif