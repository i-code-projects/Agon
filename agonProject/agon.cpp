#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include <random>
#include <fstream>
#include <filesystem>
#include <iterator>


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

struct armorPiece {
	string name = "none";
	int height = 0;
	int dmgReduction = 0;
	int tier = 0;
	int price = 0;
};

struct entityArmorComponent {
	armorPiece tunic;
	armorPiece leggings;
};

struct weapon {
	string name = "harry cok's stick";
	float dmgMult = 1.0;
	int tier = 1;
	int price = 0;
};

struct attack {
	string name = "punch";
	string type = "normal";
	int baseDmg = 10;
	int height = 1;
};

struct food {
	string name = "her cake";
	int heal = 10;
	int price = 0;
	int tier = 1;
};

struct entity {
	int money = 0;
	int fishingCatches = 0;
	int hp = 100;
	int mana = 0;
	int skill = 0;
	int tier = 1;
	string name = "your sweet nana";
	entityArmorComponent armor;
	weapon weapon = {"none",1.0};
	vector<attack> attacks = {{"punch","normal",10}};
	vector<food> inventory;
};

// PLACEHOLDER NON-DEAFULT ENTITIES
entity horse = {0,0,100,0,40,1,"De Horse",{{"armor",1,3}},{"horseshoe",1.2},{{"kick","range",5,1},{"trample","normal",10,2}},};
entity howler = {0,0,70,50,20,1,"HOWLER",{{"none",0,0}},{"claws",1.2},{{"scratch","normal",7,1},{"combo","range",5,2}},};
// GLOBAL VALUES
char clear[] = "\033[2J\033[H";
unsigned seed = chrono::system_clock::now().time_since_epoch().count();
mt19937 mt_rand(seed);
int useDefault = 1;
int modLoadingErrors = 0;

vector<entity> worldEntities = {horse,howler};
vector<attack> worldAttacks = {};
vector<food> worldFoods = {};
vector<weapon> worldWeapons = {};
vector<armorPiece> worldArmor = {};

void gameLoop();
void arenaLoop(entity& player);

int isCodeHeader(string header) {
	if (header == "=data=" || header == "=armor=" || header == "=weapons=" || header == "=attacks=" || header == "=food=" || header == "=entities=") {
		return 1;
	}
	return 0;
}

void loadTokens(vector<string> &from, vector<string> &to, long long &c) {
	while(c + 1 < from.size() && !isCodeHeader(from[c+1])) {
		to.push_back(from[c]);
		c++;
	}
}

void modParser(string path) {
	ifstream file(path);
	string input((istreambuf_iterator<char>(file)),istreambuf_iterator<char>());
	// oh no im cooked
	// INITIALIZE LEXER AND DO SOME LEXING
	vector<string> tokenList;
	string token;
	for(size_t c = 0;c < input.length(); c++) {
		if (input[c] == ',') {
			tokenList.push_back(token);
			token.clear();
			tokenList.push_back("and");
		} else if (input[c] == ':' || input[c] == '\n') {
			tokenList.push_back(token);
			token.clear();
		} else if (input[c] != '\t') {
			token += input[c];
		}
		cout << clear << c << "\n";
	}
	// weird ahh code :skull:
	vector<string> dataCode;
	vector<string> attackCode;
	vector<string> foodCode;
	vector<string> armorCode;
	vector<string> weaponCode;
	vector<string> entityCode;
	long long c = 0;
	while(c < tokenList.size()) {
		if (tokenList[c] == "=data=") {
			loadTokens(tokenList,dataCode,c);
		} else if (tokenList[c] == "=attacks=") {
			loadTokens(tokenList,attackCode,c);
		} else if (tokenList[c] == "=entities=") {
			loadTokens(tokenList,entityCode,c);
		} else if (tokenList[c] == "=food=") {
			loadTokens(tokenList,foodCode,c);
		} else if (tokenList[c] == "=armor=") {
			loadTokens(tokenList,armorCode,c);
		} else if (tokenList[c] == "=weapons=") {
			loadTokens(tokenList,weaponCode,c);
		} 
	}
	// END LEXER SO NO MORE LEXING
	// finally, the actual parser
	// this is gonna be worse than ball pain to write
	// Parse Metadata
	for(int i = 1;i < dataCode.size();i++) {
		string s = dataCode[i];
		if(s == "name") {
			i++;
		} else if (s == "overwrite default") {
			useDefault = -1;
		} else {
			modLoadingErrors++;
		}
	}
	// Parse Attacks
	for(int i = 1;i < attackCode.size();i++) {
		string s = attackCode[i];
		if(s[0] == '-') {
			attack temp;
			temp.name = s.substr(1);
			i++;
			for(int j = 1 + i;attackCode[j][0] != '-' && attackCode[j][0] != '=';j++) {
				s = attackCode[j];
				if(s == "type") {
					temp.type = attackCode[j++];
				} else if (s == "base dmg") {
					temp.baseDmg = stoi(attackCode[j++]);
				} else if (s == "height") {
					temp.height = stoi(attackCode[j++]);
				} else {
					modLoadingErrors++;
				}
			}
			worldAttacks.push_back(temp);
		}
	}
	// parse foods (big back)
	for(int i = 1;i < foodCode.size();i++) {
		string s = foodCode[i];
		if(s == "-") {
			food temp;
			temp.name = s.substr(1);
			i++;
			for(int j = i + 1;foodCode[j][0] != '-' && foodCode[j][0] != '=';j++) {
				s = attackCode[j];
				
			}
		}
	}
	// function exit mark
}

void modLoader() {
	cout << "=available mods=\n\n";
	for (const auto& ent : fs::directory_iterator("mods/")) {
		cout << ent.path().filename() << "\n";
	}
	cout << "\nChoose which one to load\n-> ";
	string input;
	while (input != "exit") {
		cin >> input;
		if (fs::exists("mods/" + input + ".as")) {
			modParser("mods/" + input + ".as");
			cout << "successfully loaded : " << input << "\n";
			cout << "with : " << modLoadingErrors << " errors\n-> ";
		} else {
			cout << "file \"" << input << "\" does not exist\n-> ";
		}
	}
}

int main(int argc, char* argv[]) {
	int gameRunning = 1;
	cout << "\033[3J\033[H\033[2J";
	if (argc > 1) {
		if (fs::exists(argv[argc-1])) {
			modParser(argv[argc-1]);
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
