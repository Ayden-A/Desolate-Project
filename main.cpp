#include <iostream>
#include <fstream>

#include <conio.h>
#include <cstdlib>

#include "Player.h"
#include "Grid.h"
#include "Alien.h"
#include "GameState.h"

using namespace std;

struct Item{

    std::string itemName;
    std::string mapFile;

    Coordinates coords;

    bool found;

};

string strLower(string inputString){

    int stringLength = inputString.length();

    string result = "";

    for(int i = 0; i < stringLength; i++){

        if(inputString[i] >= 'A' && inputString[i] <= 'Z'){

            result += inputString[i] + 32;

        }else{

            result += inputString[i];

        }
            

    }

    return result;

}

vector<string> splitString(string str){

    string currWord;

    vector<string> cmdLine;

    for(int i = 0; i < str.length(); i++){
        if(str[i] == ' '){
            cmdLine.push_back(currWord);
            currWord = "";
        }else{
            currWord += str[i];
        }
    }

    cmdLine.push_back(currWord);

    for(int i = 0; i < cmdLine.size(); i++){

        cout << "[" << cmdLine[i] << "]" << endl;

    }

    return cmdLine;

}

string fixString(string inputString){

    int stringLength = inputString.length();

    string result = "";

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

bool checkValidInput(string menu, string input){

    if(menu == "profileChoice"){

        if(input == "1" || input == "2" || input == "3"){

            return true;

        }

        return false;

    }

    if(menu == "loadScreen"){

        if(strLower(input) == "load" || strLower(input) == "new"){

            return true;

        }

        return false;

    }

    cout << "[ERROR] Menu type not valid" << endl;
    return false;

}

int callLoadingScreen(GameState game){ // Deals with save file

    string input = ""; 

    cout << endl;
    cout << "Checking for save games..." << endl;
    cout << endl;

    if(!game.saveIsValid()){

        cout << "No save file, starting new game..." << endl;

        return -1;

    }

    cout << "Save File detected, load game or start new? [ Load || New ]" << endl;
    cout << endl;

    getline(cin, input);

    cout << endl;

    while(!checkValidInput("loadScreen", input)){

        cout << "Incorrect input! Please use Load or New." << endl;

        getline(cin, input);

    }

    if(strLower(input) == "load"){

        return 1;

    }

    return 0;

}

void showHelp(){

    cout << "[Controls]" << endl;
    cout << " - W A S D to move" << endl;
    cout << " - Pressing T will allow you to type" << endl;
    cout << " - Typing Sneak, Walk, or Sprint will change walk speeds to 1 space, 2 spaces, and 4 spaces" << endl;
    cout << " - [Q] Objectives" << endl;
    cout << " - [E] Inventory" << endl;
    cout << " - [F] Interact with object" << endl;
    cout << " - [C] Steps and Days remaining" << endl;
    cout << " - [M] Map" << endl;
    cout << " - All other controls will appear on screen" << endl;

    cout << endl;

    cout << "[Legend]" << endl;
    cout << " + is the Player" << endl;
    cout << " @ are Aliens" << endl;
    cout << " # are doors to different ship segments, $ are locked doors." << endl;
    cout << " ? are items. Interact with them to save them in your inventory" << endl;
    cout << " [] are modules. Each module has a different purpose." << endl;

    cout << endl;

}

void fillMapObjects(vector<Item> &mapObjects){

    int TOTAL_ITEM_COUNT = 14; // Add more to this when adding new items

    Item newItem;

    for(int i = 0; i < TOTAL_ITEM_COUNT; i++){

        switch (i)
        {
        case 0:

            newItem.itemName = "Crow Bar";
            newItem.mapFile = "Cryochambers.txt";

            newItem.coords.xPos = 4;
            newItem.coords.yPos = 10;

            newItem.found = false;

            mapObjects.push_back(newItem);
        break;

        case 1:

            newItem.itemName = "Medkit";
            newItem.mapFile = "Cryochambers.txt";

            newItem.coords.xPos = 24;
            newItem.coords.yPos = 10;

            newItem.found = false;

            mapObjects.push_back(newItem);
        break;

        case 2:

            newItem.itemName = "Medkit";
            newItem.mapFile = "Science.txt";

            newItem.coords.xPos = 11;
            newItem.coords.yPos = 14;

            newItem.found = false;

            mapObjects.push_back(newItem);
        break;

        case 3:

            newItem.itemName = "Sample Syringe";
            newItem.mapFile = "Science.txt";

            newItem.coords.xPos = 23;
            newItem.coords.yPos = 9;

            newItem.found = false;

            mapObjects.push_back(newItem);
        break;

        case 4:

            newItem.itemName = "Old Sticky Note";
            newItem.mapFile = "Reactor.txt";

            newItem.coords.xPos = 7;
            newItem.coords.yPos = 10;

            newItem.found = false;

            mapObjects.push_back(newItem);

        break;

        case 5:

            newItem.itemName = "Firecrackers";
            newItem.mapFile = "Weapons.txt";

            newItem.coords.xPos = 10;
            newItem.coords.yPos = 11;

            newItem.found = false;

            mapObjects.push_back(newItem);

        break;

        case 6:

            newItem.itemName = "Medkit";
            newItem.mapFile = "Weapons.txt";

            newItem.coords.xPos = 21;
            newItem.coords.yPos = 7;

            newItem.found = false;

            mapObjects.push_back(newItem);

        break;

        case 7:

            newItem.itemName = "Shotgun";
            newItem.mapFile = "Quarters.txt";

            newItem.coords.xPos = 6;
            newItem.coords.yPos = 14;

            newItem.found = false;

            mapObjects.push_back(newItem);

        break;

        case 8:

            newItem.itemName = "Override Key";
            newItem.mapFile = "Quarters.txt";

            newItem.coords.xPos = 5;
            newItem.coords.yPos = 6;

            newItem.found = false;

            mapObjects.push_back(newItem);

        break;

        case 9:

            newItem.itemName = "Medkit";
            newItem.mapFile = "Thrusters.txt";

            newItem.coords.xPos = 18;
            newItem.coords.yPos = 3;

            newItem.found = false;

            mapObjects.push_back(newItem);

        break;

        case 10:

            newItem.itemName = "Flame Thrower";
            newItem.mapFile = "Quarters.txt";

            newItem.coords.xPos = 39;
            newItem.coords.yPos = 4;

            newItem.found = false;

            mapObjects.push_back(newItem);

        break;

        case 11:

            newItem.itemName = "Tritium Cell";
            newItem.mapFile = "Thrusters.txt";

            newItem.coords.xPos = 39;
            newItem.coords.yPos = 4;

            newItem.found = false;

            mapObjects.push_back(newItem);

        break;

        case 12:

            newItem.itemName = "Flare";
            newItem.mapFile = "Quarters.txt";

            newItem.coords.xPos = 39;
            newItem.coords.yPos = 3;

            newItem.found = false;

            mapObjects.push_back(newItem);

        break;

        case 13:

            newItem.itemName = "Medkit";
            newItem.mapFile = "Quarters.txt";

            newItem.coords.xPos = 6;
            newItem.coords.yPos = 13;

            newItem.found = false;

            mapObjects.push_back(newItem);

        break;
        
        default:
            break;
        }

    }

}

void callIntro(){ // Used for displaying the first play through info

    string strInput = "";

    cout << endl;
    cout << endl;
    cout << "*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*" << endl;
    cout << "#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#" << endl;
    cout << "*#  _____  ______  _____  ____  _            _______ ______  #*" << endl;
    cout << "#* |  __ \\|  ____|/ ____|/ __ \\| |        /\\|__   __|  ____| *#" << endl;
    cout << "*# | |  | | |__  | (___ | |  | | |       /  \\  | |  | |__    #*" << endl;
    cout << "#* | |  | |  __|  \\___ \\| |  | | |      / /\\ \\ | |  |  __|   *#" << endl;
    cout << "*# | |__| | |____ ____) | |__| | |____ / ____ \\| |  | |____  #*" << endl;
    cout << "#* |_____/|______|_____/ \\____/|______/_/    \\_\\_|  |______| *#" << endl;
    cout << "*#							     #*" << endl;
    cout << "#*                 					     *#" << endl;
    cout << "*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*" << endl;
    cout << "#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#" << endl;
    cout << endl;
    cout << endl;

    cout << "[STORY]:" << endl;
    cout << "You were sent on a mission to find alien life far away." << endl;
    cout << "After establishing navigation and calculating basic needs, you and your crew enter cryosleep" << endl;
    cout << "Fortunately, aliens DO exist! Unfortunately, they have infected your ship but since your chamber failed to open on time," << endl;
    cout << "you awaken to find the ship ruined and your crew murdered... You check the clock and you find you have 10 days before" << endl;
    cout << "you reach Earth! From the few minutes awake, you know they are attracted to sound but cannot see. You must fix all modules," << endl; 
    cout << "gather an alien sample, and eliminate the ship-wide infection before reaching your home, Earth" << endl;
    cout << endl;
    cout << "Do you have what it takes to save humanity from possible doom? " << endl;

    cout << endl;

    cout << "[PRESS ENTER TO CONTINUE]" << endl;

    getline(cin, strInput);

    cout << endl;

    cout << "[Game Info]" << endl;
    cout << " - The aliens will hear if you walk or run near them! This will trigger a fight with the alien!" << endl;
    cout << " - Steps are important! The more steps you take, the more tired you are. Once you run out of steps, " << endl;
    cout << "    you sleep in the nearest room and a day will have passed." << endl;

    cout << endl;

    showHelp();

    cout << endl;

    cout << "[PRESS ENTER TO CONTINUE]" << endl;

    getline(cin, strInput);

}

string getItem(vector<Item> &itemVector, string mapName, Coordinates itemCoords){

    for(int i = 0; i < itemVector.size(); i++){

        if(itemVector[i].coords.xPos == itemCoords.xPos && itemVector[i].coords.yPos == itemCoords.yPos){

            if(itemVector[i].mapFile == mapName && itemVector[i].found == false){

                itemVector[i].found = true;
                return itemVector[i].itemName;

            }

        }

    }

    return "NULL";

}

void menuSpacer(){

    for(int i = 0; i < 30; i++){

        cout << endl;

    }

}

void printMonsterImage(){

    ifstream readFile;

    readFile.open("monster.txt");

    if(readFile.fail()){

        return;

    }

    string line;

    while(getline(readFile, line)){

        cout << line << endl;

    }

    readFile.close();

}

bool alienFight(Player &currentPlayer, Alien angryAlien){

    bool fighting = true;

    srand(time(0));

    int randomNum = 0;

    char buttonPress = 'a';
    string equippedItem = currentPlayer.getEquipped();
    string input = "";

    float flameMultiplier = 1;
    int flameDMG = 20;

    cout << endl;
    cout << endl;
    cout << endl;

    printMonsterImage();

    cout << endl;

    cout << "[A MONSTER HEARD YOU AND LUNGES FORWARDS]" << endl;
    
    cout << endl;

    while(fighting){

        angryAlien.printHealthBar();
        currentPlayer.printHealthBar();

        cout << endl;

        cout << "[EQUIPPED]: " << fixString(equippedItem) << endl;
        cout << "Choose an option:" << endl;
        cout << " - A for Attack" << endl;
        cout << " - S to attempt to flee!" << endl;
        cout << " - E for Inventory" << endl;

        cout << endl;

        buttonPress = _getch();

        input = buttonPress;

        cout << endl;
        cout << endl;

        if(strLower(input) == "a"){

            if(equippedItem == "Fists"){

                cout << "You swung at the alien with your Fists!" << endl;
                cout << "[10 DMG]" << endl;
                angryAlien.setHealth( angryAlien.getHealth() - 10);

            }else if(strLower(equippedItem) == "crow bar"){

                cout << "You swung at the alien with your Crow Bar!" << endl;
                cout << "[17 DMG]" << endl;
                angryAlien.setHealth( angryAlien.getHealth() - 17);

            }else if(strLower(equippedItem) == "shotgun"){

                randomNum = rand() % 100;

                if(randomNum >= 50){

                    cout << "You pepper the alien with lead!" << endl;
                    cout << "[30 DMG]" << endl;
                    angryAlien.setHealth( angryAlien.getHealth() - 30);

                }else{

                    cout << "You blast a hole in the alien!" << endl;
                    cout << "[50 DMG]" << endl;
                    angryAlien.setHealth( angryAlien.getHealth() - 50);

                }

            }else if(strLower(equippedItem) == "flame thrower"){

                cout << "You ignited the alien! It's causing burn damage!" << endl;
                cout << "[" << flameDMG * flameMultiplier << " DMG]" << endl;
                angryAlien.setHealth( angryAlien.getHealth() - (flameDMG * flameMultiplier));

                flameMultiplier += 0.5;

            }else{

                cout << "[5 DMG]" << endl;
                angryAlien.setHealth( angryAlien.getHealth() - 5);

            }

            if(angryAlien.getHealth() > 0){

                cout << "The alien swipes at you with claws!" << endl;
                cout << "[15 DMG]" << endl;
                currentPlayer.setHealth(currentPlayer.getHealth() - angryAlien.getDamage());

            }

            cout << endl;
            cout << endl;
            cout << endl;

        }else if(strLower(input) == "e"){

            currentPlayer.printInventory();

            cout << endl;

            cout << "Choose an item to equip or type E to escape" << endl;

            getline(cin, input);

            cout << endl;
            
            while(!currentPlayer.getInventoryItem(input) && strLower(input) != "e"){

                cout << "Please type what you see in your inventory!" << endl;

                getline(cin, input);

            }

            if(strLower(input) == "e"){

                cout << "Leaving inventory..." << endl;

            }else if(strLower(input) == "medkit"){

                cout << "Health restored to 100!" << endl;
                currentPlayer.setHealth(100);
                currentPlayer.removeFromInventory("Medkit");

            }else if(strLower(input) == "sample syringe"){

                cout << "You collected a sample but the alien slashed your back!" << endl;
                cout << "[25 DMG]" << endl;
                currentPlayer.setHealth(currentPlayer.getHealth() - 25);

                currentPlayer.addToInventory("Toxic Syringe");
                currentPlayer.removeFromInventory("Sample Syringe");

            }else{

                cout << fixString(input) << " now equipped!" << endl;
                currentPlayer.setEquipped(fixString(input));
                equippedItem = input;
            }

        }else if(strLower(input) == "s"){

            randomNum = rand() % 100;

            if(randomNum <= 66){

                cout << "You escaped!" << endl;
                return false;

            }else{

                cout << "You tripped and got slashed!" << endl;
                cout << "[50 DMG]" << endl;
                currentPlayer.setHealth(currentPlayer.getHealth() - 50);

            }

        }

        if(currentPlayer.getHealth() <= 0 || angryAlien.getHealth() <= 0){

            fighting = false;

        }
        
    }

    if(currentPlayer.getHealth() <= 0){

        return false;

    }


    cout << "You won with " << currentPlayer.getHealth() << " health remaining!" << endl;
    cout << endl;
    return true;

}

void printSleepMsg(int daysRemaining){

    cout << "You have fallen asleep due to fatigue..." << endl;

    cout << endl;

    cout << " You now have:";

    cout << endl;            

    switch (daysRemaining){

        case 10:

            cout << "  __    ___  " << endl;
            cout << " /_ |  / _ \\ " << endl;
            cout << "  | | | | | |" << endl;
            cout << "  | | | | | |" << endl;
            cout << "  | | | |_| |" << endl;
            cout << "  |_|  \\___/ " << endl;
            
        break;

        case 9:

            cout << "   ___  " << endl;
            cout << "  / _ \\ " << endl;
            cout << " | (_) |" << endl;
            cout << "  \\__, |" << endl;
            cout << "    / / " << endl;
            cout << "   /_/  " << endl;

        break;

        case 8:

            cout << "   ___  " << endl;
            cout << "  / _ \\ " << endl;
            cout << " | (_) |" << endl;
            cout << "  > _ < " << endl;
            cout << " | (_) |" << endl;
            cout << "  \\___/ " << endl;

        break;

        case 7:

            cout << "  ______ " << endl;
            cout << " |____  |" << endl;
            cout << "     / / " << endl;
            cout << "    / /  " << endl;
            cout << "   / /   " << endl;
            cout << "  /_/    " << endl;

        break;

        case 6:

            cout << "    __  " << endl;
            cout << "   / /  " << endl;
            cout << "  / /_  " << endl;
            cout << " | '_ \\ " << endl;
            cout << " | (_) |" << endl;
            cout << "  \\___/ " << endl;

        break;

        case 5:

            cout << "  _____ " << endl;
            cout << " | ____|" << endl;
            cout << " | |__  " << endl;
            cout << " |___ \\ " << endl;
            cout << "  ___) |" << endl;
            cout << " |____/ " << endl;

        break;

        case 4:

            cout << "  _  _   " << endl;
            cout << " | || |  " << endl;
            cout << " | || |_ " << endl;
            cout << " |__   _|" << endl;
            cout << "    | |  " << endl;
            cout << "    |_|  " << endl;

        break;

        case 3:

            cout << "  ____  " << endl;
            cout << " |___ \\ " << endl;
            cout << "   __) |" << endl;
            cout << "  |__ < " << endl;
            cout << "  ___) |" << endl;
            cout << " |____/ " << endl;

        break;

        case 2:

            cout << "  ___  " << endl;
            cout << " |__ \\ " << endl;
            cout << "    ) |" << endl;
            cout << "   / / " << endl;
            cout << "  / /_ " << endl;
            cout << " |____|" << endl;

        break;

        case 1:

            cout << "  __ " << endl;
            cout << " /_ |" << endl;
            cout << "  | |" << endl;
            cout << "  | |" << endl;
            cout << "  | |" << endl;
            cout << "  |_|" << endl;

        break;
        
        default:

            cout << "[LN 482: ERROR] DAYS REMAINING NOT VALID" << endl;

        break;
        
    }

    cout << endl;
    cout << "              days remaining." << endl;

    cout << endl;
    cout << "[PRESS ENTER TO CONTINUE]" << endl;

    string garbo = "";

    getline(cin, garbo);

}

void printObjectives(Grid mapObject, Player currentPlayer){

    // Modules Fixed
    // Cure Made

    bool syringeFound = currentPlayer.getInventoryItem("Sample Syringe");
    bool cureMade = currentPlayer.getInventoryItem("Canister of Blight Cure");
    bool cellObtained = currentPlayer.getInventoryItem("Tritium Cell");
    bool keyObtained = currentPlayer.getInventoryItem("Override Key");
    
    bool cryo = mapObject.modFixCheck("cryo");
    bool reactor = mapObject.modFixCheck("reactor");
    bool doors = mapObject.modFixCheck("doors");
    bool weapons = mapObject.modFixCheck("weapons");
    bool armory = mapObject.modFixCheck("armory");
    bool overrideMod = mapObject.modFixCheck("override");

    if(!cryo){

        cout << "[ ] Fix the module in the Cryochambers" << endl;
        cout << endl;
        return;

    }else{

        cout << "[X] Fix the module in the Cryochambers" << endl;
        cout << endl;

    }
    

    if(!cellObtained && !reactor){

        cout << "[ ] Get the Tritium Cell from the Thruster Bay" << endl;
        cout << endl;
        return;

    }else{

        cout << "[X] Get the Tritium Cell from the Thruster Bay" << endl;
        cout << endl;
        
    }

    if(!reactor){

        cout << "[ ] Put the cell into the Reactor module" << endl;
        cout << endl;
        return;

    }else{

        cout << "[X] Put the cell into the Reactor module" << endl;
        cout << endl;

    }
    
    if(!weapons){

        cout << "[ ] Fix the module in Weapons" << endl;
        cout << endl;

    }else{

        cout << "[X] Fix the module in Weapons" << endl;
        cout << endl;

    }

    if(!doors){

        cout << "[ ] Unlock the Reactor Doors" << endl;
        cout << endl;
        return;

    }else{

        cout << "[X] Unlock the Reactor Doors" << endl;
        cout << endl;

    }

    if(!keyObtained && !overrideMod){

        cout << "[ ] Find the Override Key in the Quarters" << endl;
        cout << endl;
        return;

    }else{

        cout << "[X] Find the Override Key in the Quarters" << endl;
        cout << endl;

    }

    if(!overrideMod){

        cout << "[ ] Put the key in the override module in Thrusters" << endl;
        cout << endl;
        return;

    }else{

        cout << "[X] Put the key in the override module in Thrusters" << endl;
        cout << endl;
        
    }

    if(!armory){

        cout << "[ ] Grab an armory kit from Quarters (optional)" << endl;
        cout << endl;

    }else{

        cout << "[X] Grab an armory kit from Quarters (optional)" << endl;
        cout << endl;

    }

    if(!syringeFound && !cureMade){

        cout << "[ ] Find the Sample Syringe" << endl;
        cout << endl;
        return;

    }else{

        cout << "[X] Find the Sample Syringe" << endl;
        cout << endl;

    }

    if(!cureMade){

        cout << "[ ] Collect a sample and make a cure in the Science Lab" << endl;
        cout << endl;
        return;

    }else{

        cout << "[X] Collect a sample and make a cure in the Science Lab" << endl;
        cout << endl;

    }

}

int main(){

    //Map
    Grid gameMap;

    //Player
    Player gamePlayer;
    

    //Profile Management
    GameState currGame;

    //Map Cell
    Cell basicCell;

    // Interactable Items
    vector<Item> gameItems;
    fillMapObjects(gameItems);


    //Debug Vars
    bool devMode = false;
    bool visionConeEnabled = true;
    bool fightsEnabled = true;
    bool debugPanel = false;
    bool infiniteSteps = false;
    bool godMode = false;
    vector<string> cmdLine;

    // User Input
    char input = 'a';
    string strInput = "";
    int stoiInput = 0;

    // Item Variables
    bool itemClose = false;
    string itemName = "NULL";

    // Alien Variables
    bool alienClose = false;
    bool playerWon = false;
    Alien currAlien;

    // Door Variables
    bool doorClose = false;

    // Module Variables
    bool modClose = false;
    string modWin = "";

    int loadState = callLoadingScreen(currGame);

    if(loadState == 1){

        currGame.readSave();
        currGame.printProfiles();

        cout << "Choose a profile: ";

        getline(cin, strInput);

        while(!checkValidInput("profileChoice", strInput)){

            cout << "Invalid choice, use a number 1-3" << endl;

            getline(cin, strInput);

        }

        stoiInput = stoi(strInput);

        currGame.setProfileIndex(stoiInput - 1);

        if(currGame.getUsername() == "[EMPTY]"){

            cout << "Enter your username: ";

            getline(cin, strInput);

            currGame.setUsername(strInput);

        }else{

            currGame.loadGame(gamePlayer, gameMap);

        }

    }else{

        callIntro();

        gamePlayer.setXPos(7);
        gamePlayer.setYPos(2);

        gamePlayer.setDirection("South");

        gameMap.loadMap("Cryochambers.txt");
        currGame.addVisited("Cryochambers.txt");

        gameMap.drawPlayer(gamePlayer);

        gamePlayer.addToInventory("Flare");

    }

    bool menu = true;
    bool quit = false;

    while(menu){

        if(infiniteSteps){

            gamePlayer.setSteps(16);

        }

        if(gamePlayer.getSteps() <= 0){

            currGame.setDaysRemaining(currGame.getDaysRemaining() - 1);

            printSleepMsg(currGame.getDaysRemaining());

        }
        
        gameMap.printMap(visionConeEnabled);
        gamePlayer.printHealthBar();

        // Variable Checking

        itemClose = gameMap.checkForNearbyItems(); // Variables for checking if objects are near
        alienClose = gameMap.checkForNearbyAliens();
        doorClose = gameMap.checkForNearbyDoors();
        modClose = gameMap.checkForNearbyModules();

        //

        if(alienClose && gamePlayer.getSpeed() != "Sneak" && fightsEnabled){

            playerWon = alienFight(gamePlayer, currAlien);

            if(!playerWon && gamePlayer.getHealth() <= 0){

                cout << endl;
                cout << "The claws on those aliens are sharper than you thought!" << endl;
                cout << endl;
                cout << "[Restart? Y/N]: ";

                input = _getch();
                strInput = input;

                while(strLower(strInput) != "y" && strLower(strInput) != "n"){

                    cout << "[Invalid Input, use Y or N]: ";
                    input = _getch();
                    strInput = input;

                }

                if(strLower(strInput) == "y"){

                    cout << endl;
                    cout << endl;
                    main();

                }else{

                    return 0;

                }

            }else if(!playerWon){

                gameMap.printMap(visionConeEnabled);
                gamePlayer.printHealthBar();

            }

            if(playerWon){

                basicCell.coords.xPos = gameMap.nearbyCoords("[@]").xPos;
                basicCell.coords.yPos = gameMap.nearbyCoords("[@]").yPos;
                basicCell.character = ' ';

                currGame.addAnomoly(basicCell, gameMap.getCurrMap());

                gameMap.setCell(gameMap.nearbyCoords("[@]").xPos, gameMap.nearbyCoords("[@]").yPos, ' ');

                gameMap.printMap(visionConeEnabled);
                gamePlayer.printHealthBar();

            }

        }

        cout << endl;

        cout << "                   -" << gamePlayer.getSpeed() << "ing-" << endl;
        cout << "    - Press T To Type | Type HELP for controls - " << endl;
        cout << "               - [E] Inventory-" << endl;
        cout << "              - [Q] Objectives -" << endl;

        if(itemClose){

            cout << "        ! F to interact with object !" << endl;

        }

        if(doorClose){

            cout << "               ! O to open door !" << endl;
            
        }

        if(modClose){

            cout << "               ! G to fix module !" << endl;

        }

        cout << "                    - QUIT -" << endl;



        if(debugPanel){

            cout << "[CMDS]" << endl;
            cout << " - dev_fullBright     - dev_tp" << endl;
            cout << " - dev_aggro          - dev_godmode" << endl;
            cout << " - dev_infiniteSteps  - dev_giveitem" << endl;
            cout << " - devMode" << endl;

            cout << endl;
            cout << "[Debug Panel]" << endl;
            cout << "[Steps]: " << gamePlayer.getSteps() << "     [Facing]: " << gamePlayer.getDirection() << endl;
            gameMap.printPlrPos();
            cout << "[x, y] = (" << gamePlayer.getXPos() << ", " << gamePlayer.getYPos() << ")" << endl;
            cout << "[Item Close] = " << itemClose << "  [Item Deets] = (" << gameMap.nearbyCoords("[?]").xPos << ", " << gameMap.nearbyCoords("[?]").yPos << ")" << endl;
            cout << endl;

        }

        input = _getch();
        // getline(cin, strInput);

        strInput = input;

        if(strInput == "t" || strInput == "T"){

            cout << "[ENTER TEXT]: ";
            getline(cin, strInput);

            cmdLine = splitString(strLower(strInput));

        }

        menuSpacer();

        if(strLower(strInput) == "w"){ // Move Up

            gameMap.movePlayer("North", gamePlayer);

        }else if(strLower(strInput) == "a"){ // Move Left

            gameMap.movePlayer("West", gamePlayer);

        }else if(strLower(strInput) == "s"){ // Move Down

            gameMap.movePlayer("South", gamePlayer);

        }else if(strLower(strInput) == "d"){ // Move Right

            gameMap.movePlayer("East", gamePlayer);

        }else if(strLower(strInput) == "sneak"){ // Sneak mode

            cout << "Sneaking now!" << endl;
            gamePlayer.setSpeed("Sneak");

        }else if(strLower(strInput) == "walk"){ // Walk mode

            cout << "Walking now!" << endl;
            gamePlayer.setSpeed("Walk");

        }else if(strLower(strInput) == "sprint" || strLower(strInput) == "run"){ // Sprint mode

            cout << "Sprinting now!" << endl;
            gamePlayer.setSpeed("Sprint");

        }else if(strLower(strInput) == "q"){ // Open Objectives

            printObjectives(gameMap, gamePlayer);

        }else if(strLower(strInput) == "f" && itemClose){ // Item Pickup

            itemName = getItem(gameItems, gameMap.getCurrMap(), gameMap.nearbyCoords("[?]"));

            cout << "You've picked up [" << itemName << "]!" << endl;
            cout << endl;

            gamePlayer.addToInventory(itemName);

            basicCell.coords.xPos = gameMap.nearbyCoords("[?]").xPos;
            basicCell.coords.yPos = gameMap.nearbyCoords("[?]").yPos;
            basicCell.character = ' ';

            currGame.addAnomoly(basicCell, gameMap.getCurrMap());

            gameMap.setCell(gameMap.nearbyCoords("[?]").xPos, gameMap.nearbyCoords("[?]").yPos, ' ');


        }else if(strLower(strInput) == "o" && doorClose){ // Door Open

            gameMap.changeMap(gamePlayer, currGame.getAnomolies());

            //If map has not been visited, visit it
            if(!currGame.mapExists(gameMap.getCurrMap())){

                currGame.addVisited(gameMap.getCurrMap());

            }

        }else if(strLower(strInput) == "g" && modClose){ // Modules

            modWin = gameMap.modGame(gamePlayer);

            if(modWin == "doors"){ // REACTOR DOORS MODULE

                // Changing doors to oppenable
                gameMap.setCell(49, 8, '#');
                basicCell.coords.xPos = 49;
                basicCell.coords.yPos = 8;
                basicCell.character = '#';
                currGame.addAnomoly(basicCell, "Reactor.txt");


                gameMap.setCell(49, 9, '#');
                basicCell.coords.yPos = 9;
                currGame.addAnomoly(basicCell, "Reactor.txt");

                gameMap.setCell(49, 10, '#');
                basicCell.coords.yPos = 10;
                currGame.addAnomoly(basicCell, "Reactor.txt");

                // Removing module from location
                gameMap.setCell(21, 15, '0');
                basicCell.coords.xPos = 21;
                basicCell.coords.yPos = 15;
                basicCell.character = '0';
                currGame.addAnomoly(basicCell, "Reactor.txt");

                gameMap.setCell(22, 15, '0');
                basicCell.coords.xPos = 22;
                currGame.addAnomoly(basicCell, "Reactor.txt");

            }else if(modWin == "weapons"){ // WEAPONS MODULE

                gameMap.setCell(20, 15, '0');
                basicCell.coords.xPos = 20;
                basicCell.coords.yPos = 15;
                basicCell.character = '0';
                currGame.addAnomoly(basicCell, "Weapons.txt");

                gameMap.setCell(21, 15, '0');
                basicCell.coords.xPos = 21;
                currGame.addAnomoly(basicCell, "Weapons.txt");

                gameMap.setCell(22, 15, '0');
                basicCell.coords.xPos = 22;
                currGame.addAnomoly(basicCell, "Weapons.txt");

            }else if(modWin == "powerEnabled"){ // POWER MODULE

                gamePlayer.removeFromInventory("Tritium Cell");

            }else if(modWin == "override"){ // OVERRIDE MODULE

                gamePlayer.removeFromInventory("Override Key");

                gameMap.setCell(17, 17, '0');
                basicCell.coords.xPos = 17;
                basicCell.coords.yPos = 17;
                basicCell.character = '0';
                currGame.addAnomoly(basicCell, "Thrusters.txt");

                gameMap.setCell(18, 17, '0');
                basicCell.coords.xPos = 18;
                currGame.addAnomoly(basicCell, "Thrusters.txt");

            }else if(modWin == "locker1"){ // QUARTERS MODULE: LOCKER 1 = Flamethrower + Flares

                gameMap.setCell(39, 4, '?');
                gameMap.setCell(39, 3, '?');

            }else if(modWin == "locker2"){ // QUARTERS MODULE: LOCKER 2 = Shotgun + Medkit

                gameMap.setCell(6, 14, '?');
                gameMap.setCell(6, 13, '?');

            }

        }else if(strLower(strInput) == "e"){ // Inventory
            
            gamePlayer.printInventory();
            cout << endl;

            cout << "Type the item name to use it or type exit" << endl;

            getline(cin, strInput);

            while(!gamePlayer.getInventoryItem(strInput) && strLower(strInput) != "exit"){

                cout << "Please type what you see in your inventory or type exit!" << endl;

                getline(cin, strInput);

            }

            if(strLower(strInput) == "medkit"){

                cout << "Replenished health to full!" << endl;
                gamePlayer.setHealth(100);
                gamePlayer.removeFromInventory("Medkit");

            }else if(strLower(strInput) == "old sticky note"){

                cout << endl;

                cout << "It reads 5783, it may be a code for something..." << endl;

                cout << endl;

            }else if(strLower(strInput) == "flare"){

                if(gameMap.throwFlare()){

                    cout << "Flare thrown!" << endl;

                    currGame.addAnomoly(gameMap.getRecentFlare(), gameMap.getCurrMap());

                }else{

                    cout << "You're too close to a wall!" << endl;

                }

            }else if(strLower(strInput) != "exit"){

                cout << "You can't use that right now!" << endl;

            }


        }else if(strLower(strInput) == "m"){ // Map

            currGame.printVisited(gameMap.getCurrMap());

        }else if(strLower(strInput) == "c"){ // Check Steps and Days

            cout << "[Steps Remaining] " << gamePlayer.getSteps() << endl;
            cout << "[Days Remaining] " << currGame.getDaysRemaining() << endl;

        }else if(strLower(strInput) == "help"){ // Help

            showHelp();

        }else if(strLower(strInput) == "devmode"){ // Dev Mode

            cout << "[Dev Mode Enabled]" << endl;
            visionConeEnabled = false;
            fightsEnabled = false;
            infiniteSteps = true;
            debugPanel = true;
            gamePlayer.setHealth(1000);
            godMode = true;

        }else if(cmdLine[0] == "dev_fullbright"){ // Fullbright Mode

            if(cmdLine.size() < 2){

                cout << "[ERROR] dev_fullbright <true/false>" << endl;

            }else if(cmdLine[1] == "true"){

                cout << "[Fullbright Enabled]" << endl;
                visionConeEnabled = false;

            }else if(cmdLine[1] == "false"){

                cout << "[Fullbright Disabled]" << endl;
                visionConeEnabled = true;

            }else{

                cout << "[ERROR] dev_fullbright <true/false>" << endl;

            }

        }else if(cmdLine[0] == "dev_aggro"){ // Aggression Mode

            if(cmdLine.size() < 2){

                cout << "[ERROR] dev_aggro <true/false>" << endl;

            }else if(cmdLine[1] == "false"){

                cout << "[Fighting Disabled]" << endl;
                fightsEnabled = false;

            }else if(cmdLine[1] == "true"){

                cout << "[Fighting Enabled]" << endl;
                fightsEnabled = true;

            }else{

                cout << "[ERROR] dev_aggro <true/false>" << endl;

            }

        }else if(cmdLine[0] == "dev_infinitesteps"){ // Infinite Steps

            if(cmdLine.size() < 2){

                cout << "[ERROR] dev_infinitesteps <true/false>" << endl;

            }else if(cmdLine[1] == "false"){

                cout << "[Infinite Steps Disabled]" << endl;
                infiniteSteps = false;

            }else if(cmdLine[1] == "true"){

                cout << "[Infinite Steps Enabled]" << endl;
                infiniteSteps = true;

            }else{

                cout << "[ERROR] dev_infinitesteps <true/false>" << endl;

            }

        }else if(cmdLine[0] == "dev_debugger"){ // Debug Panel

            if(cmdLine.size() < 2){

                cout << "[ERROR] dev_debugger <true/false>" << endl;

            }else if(cmdLine[1] == "false"){

                cout << "[Debugger Disabled]" << endl;
                debugPanel = false;

            }else if(cmdLine[1] == "true"){

                cout << "[Debugger Enabled]" << endl;
                debugPanel = true;

            }else{

                cout << "[ERROR] dev_debugger <true/false>" << endl;

            }

        }else if(cmdLine[0] == "dev_godmode"){ // God Mode

            if(cmdLine.size() < 2){

                cout << "[ERROR] dev_godmode <true/false>" << endl;

            }else if(cmdLine[1] == "false"){

                cout << "[God Mode Disabled]" << endl;
                gamePlayer.setHealth(100);
                godMode = false;

            }else if(cmdLine[1] == "true"){

                cout << "[God Mode Enabled]" << endl;
                gamePlayer.setHealth(1000);
                godMode = true;

            }else{

                cout << "[ERROR] dev_godmode <true/false>" << endl;

            }

        }else if(cmdLine[0] == "dev_tp"){ // Teleport

            if(cmdLine.size() < 2){

                cout << "[ERROR] dev_tp <Map Name>" << endl;

            }else{

                gameMap.changeMap(gamePlayer, currGame.getAnomolies(), cmdLine[1]);

            }

            

        }else if(cmdLine[0] == "dev_giveitem"){ // Give Item

            if(cmdLine.size() < 2){

                cout << "[ERROR] dev_giveitem <Item Name>" << endl;

            }else{

                gamePlayer.addToInventory(fixString(cmdLine[1]));

            }

            

        }else if(cmdLine[0] == "dev_skipday"){ // Skip Day

            gamePlayer.setSteps(0);

        }else if(strLower(strInput) == "quit"){ // Quit

            cout << "C'ya!" << endl;

            return 0;

        }else{ // Input Unrecognized

            cout << "          [Invalid Input, try again!]" << endl;

        }

        if(gamePlayer.getSteps() <= 0){
            currGame.setDaysRemaining(currGame.getDaysRemaining() - 1);
        }

        

        if(currGame.getDaysRemaining() <= 0 || gameMap.checkAllModFixed()){

            menu = false;

        }else if(gamePlayer.getSteps() <= 0){

            printSleepMsg(currGame.getDaysRemaining());

            gamePlayer.setSteps(350);

        }
    }

    if(gameMap.checkAllModFixed() && !quit){

        cout << endl;
        cout << "[WARNING: LANDING SEQUENCE INITIATED]" << endl;
        cout << endl;
        cout << "You sit at the captain's console and start up the weapons sentries. You use their phase beams to blast any aliens left on the ship" << endl;
        cout << endl;
        cout << "The ship begins using it's reverse thrusters and finally it touches back down on your home planet" << endl;
        cout << "A door opens and you step out. Government officials and safety workers crowd you making sure you're okay." << endl;
        cout << "As billions of people await for your first communication in 5 years, you reach in your pocket and pull out the cure." << endl;
        cout << "You steadily raise it in the air as everyone around you cheers!" << endl;
        cout << endl;
        cout << endl;
        cout << "Look at you. You have what it takes." << endl;
        cout << endl;
        cout << endl;

        cout << "[Restart? Y/N]: ";

        input = _getch();
        strInput = input;

        while(strLower(strInput) != "y" && strLower(strInput) != "n"){

            cout << "[Invalid Input, use Y or N]" << endl;
            input = _getch();
            strInput = input;

        }

        if(strLower(strInput) == "y"){

            cout << endl;
            cout << endl;
            main();

        }else{

            return 0;

        }

    }else if(!quit){

        if(gameMap.modFixCheck("weapons") && !gameMap.modFixCheck("cure")){

            cout << endl;
            cout << "[WARNING: LANDING SEQUENCE INITIATED]" << endl;
            cout << endl;
            cout << "You sit at the captain's console and start up the weapons sentries. You use their phase beams to blast any aliens left on the ship" << endl;
            cout << endl;
            cout << "The ship begins using it's reverse thrusters and finally it touches back down on your home planet" << endl;
            cout << "Within minutes you're greeted by many government officials and a safety team. They look at you with excitement and hope." << endl;
            cout << "You stare back, numb, only to look down in sorrow. You did not find a solution to the world's collapsing food supply." << endl;
            cout << endl;
            cout << "Maybe you should be a farmer..." << endl;
            cout << endl;
            cout << "[NO CURE ENDING]" << endl;
            cout << endl;

        }else{

            cout << endl;
            cout << "[ALARMS TRIGGERED]" << endl;
            cout << "You hear frantic pings and whines and whirs as your home planet rapidly approaches." << endl;
            cout << endl;
            cout << "Before you know it, the ship is engulfed in flames and the ship is too unstable to make the proper calculations." << endl;
            cout << endl;
            cout << "The ship crashes into a dense forest where all that's left are half mangled aliens crawling from the wreckage." << endl;
            cout << endl;
            cout << "[CRASH ENDING]" << endl;
            cout << endl;

        }
        
        cout << "[Restart? Y/N]: ";

        input = _getch();
        strInput = input;

        while(strLower(strInput) != "y" && strLower(strInput) != "n"){

            cout << "[Invalid Input, use Y or N]" << endl;
            input = _getch();
            strInput = input;

        }

        if(strLower(strInput) == "n"){

            return 0;

        }else{

            cout << endl;
            cout << endl;
            main();

        }

    }

    return 0;

}