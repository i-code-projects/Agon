#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include <random>
#include <fstream>
#include <filesystem>
#include <iterator>
#include <cstring>
#include "templates.h"
#include "globals.h"
#include "functions.h"


using namespace std;
namespace fs = filesystem;

string logo = R"(                                                  
        ##                                        
     /####                                        
    /  ###                                        
       /##                                        
      /  ##                                       
      /  ##         /###      /###   ###  /###    
     /    ##       /  ###  / / ###  / ###/ #### / 
     /    ##      /    ###/ /   ###/   ##   ###/  
    /      ##    ##     ## ##    ##    ##    ##   
    /########    ##     ## ##    ##    ##    ##   
   /        ##   ##     ## ##    ##    ##    ##   
   #        ##   ##     ## ##    ##    ##    ##   
  /####      ##  ##     ## ##    ##    ##    ##   
 /   ####    ## / ########  ######     ###   ###  
/     ##      #/    ### ###  ####       ###   ### 
#                        ###                      
 ##                ####   ###                     
                 /######  /#                      
                /     ###/       
)";
// sMCP = startMenuChoicePrompt but in a bad way.
string sMCP = R"(
		-play
		-mods
		-help
		-exit
)";

// PLACEHOLDER NON-DEAFULT ENTITIES
entity horse = {0,0,100,0,40,1,"De Horse",{{"armor",1,3}},{"horseshoe",1.2},{{"kick","range",5,1},{"trample","normal",10,2}},};
entity howler = {0,0,70,50,20,1,"HOWLER",{{"none",0,0}},{"claws",1.2},{{"scratch","normal",7,1},{"combo","range",5,2}},};
// extras
char clear[] = "\033[2J\033[H";
unsigned seed = chrono::system_clock::now().time_since_epoch().count();
mt19937 mt_rand(seed);
// GLOBAL VALUES
int useDefault = 1;
int debugMods = 0;
int modLoadingErrors = 0;

vector<entity> worldEntities = {horse,howler};
vector<attack> worldAttacks = {};
vector<food> worldFoods = {};
vector<weapon> worldWeapons = {};
vector<armorPiece> worldArmor = {};

void gameLoop();
void arenaLoop(entity& player);

int main(int argc, char* argv[]) {
	int gameRunning = 1;
	cout << "\033[3J\033[H\033[2J";
	if (argc > 1) {
		if (fs::exists(argv[argc-1])) {
			modParser(argv[argc-1]);
		} else if (strcmp(argv[argc-1],"-d") == 0) {
			debugMods = 1;
		}
	}
	while(gameRunning) {
		cout << clear << logo << sMCP << "-> ";
		string input;
		cin >> input;
		if (input == "exit") {
			gameRunning = 0;
		} else if (input == "play") {
			gameLoop();
		} else if (input == "mods") {
			modLoader();
		}
	}
	return 0;
}

void gameLoop() {
	string input;
	entity player;
	cout << R"(
		Enter your name:
-> )";
	getline(cin >> ws,player.name);
	cout << clear << logo << "	" <<  player.name << "'s JOURNEY HAS BEGUN\n-> ";
	this_thread::sleep_for(chrono::seconds(2));
	int exited = 0;
	while(!exited) {
		cout << clear << logo;		
		cout << R"(
		-shop
		-arena
		-fishing
		-info
-> )";
		cin >> input;
		if (input == "arena") {
			arenaLoop(player);
		} else if (input == "shop") {
			cout << "NOu!";
		} else if (input == "info") {
			cout << "money: " << player.money << "\n";
			cout << "fishingCatches: " << player.fishingCatches << "\n";
			cout << "skill: " << player.skill << "\n";
			cout << "leggings: " << player.armor.leggings.name << "\n";
			cout << "tunic: " << player.armor.tunic.name << "\n";
			this_thread::sleep_for(chrono::seconds(5));
		}
	}
}

void arenaLoop(entity& player) {
	// prints
	cout << clear;
	cout << R"(
	THE PLAYER HAS ENTERED THE ARENA
)";
	this_thread::sleep_for(chrono::seconds(2));
	// random engine startup
	uniform_int_distribution<int> dist(0,worldEntities.size() - 1);
	// all the bullshit. fuck these.
	int round = 1;
	entity enemy;
	do {
	int enemyIndex = dist(mt_rand);
	enemy = worldEntities[enemyIndex];
	} while (enemy.tier > round);
	// ACTUAL GAMEPLAY
	string input;
	while(player.hp > 0) {
		cout << clear << logo << enemy.name;
		cout << " (" << enemy.hp << ")" << "\n";
		cout << "Player stats | hp: " << player.hp << " mana: " << player.mana << " money: " << player.money << "\n-> ";
		cin >> input;
	}
}
