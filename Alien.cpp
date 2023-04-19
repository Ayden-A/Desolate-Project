#include <iostream>
#include "Alien.h"

using namespace std;


Alien::Alien(){

    health = 50;
    damage = 15;
    critChance = 0.1;

    coords.xPos = -1;
    coords.yPos = -1;

}

int Alien::getHealth(){

    return health;

}

int Alien::getDamage(){

    return damage;

}

double Alien::getCritChance(){

    return critChance;

}

Coordinates Alien::getCoords(){

    return coords;

}



void Alien::setHealth(int health_){

    health = health_;

}

void Alien::setDamage(int damage_){

    damage = damage_;

}

void Alien::setCritChance(double critChance_){

    critChance = critChance_;

}

void Alien::setCoords(Coordinates coords_){

    coords.xPos = coords_.xPos;
    coords.yPos = coords_.yPos;

}

void Alien::setCoords(int xPos_, int yPos_){

    coords.xPos = xPos_;
    coords.yPos = yPos_;

}

void Alien::printHealthBar(){

    if(health <= 0){

        return;

    }

    int barSegments = health / 5;

    cout << "ALIEN HEALTH: " << health << " [";

    for(int i = 0; i < barSegments; i++){

        cout << "+";

    }

    if(barSegments < 10){

        for(int i = 0; i < 10 - barSegments; i++){

            cout << " ";

        }

    }

    cout << "]" << endl;

}