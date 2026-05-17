#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <filesystem>
#include <iterator>
#include "templates.h"
#include "globals.h"
#include "functions.h"

using namespace std;
namespace fs = filesystem;

void parseAttacks(vector<string> &attackCode);
void parseFoods (vector<string> &foodCode);
void parseArmor(vector<string> &armorCode);
void parseWeapons(vector<string> &weaponCode);
void parseEntities(vector<string> &entityCode);

int isCodeHeader(string header) {
	if (header == "=data=" || header == "=armor=" || header == "=weapons=" || header == "=attacks=" || header == "=food=" || header == "=entities=") {
		return 1;
	}
	return 0;
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
	}
	
	// weird ahh code :skull:
	vector<string> dataCode;
	vector<string> attackCode;
	vector<string> foodCode;
	vector<string> armorCode;
	vector<string> weaponCode;
	vector<string> entityCode;
	string tempHeader = "sup";
	for(size_t c = 0; c  < tokenList.size(); c++) {
		string s = tokenList[c];
		if (isCodeHeader(s)) {
			tempHeader = s;
		}
		if (tempHeader == "=data=") {
			dataCode.push_back(s);
		} else if (tempHeader == "=attacks=") {
			attackCode.push_back(s);
		} else if (tempHeader == "=entities=") {
			entityCode.push_back(s);
		} else if (tempHeader == "=food=") {
			foodCode.push_back(s);
		} else if (tempHeader == "=armor=") {
			armorCode.push_back(s);
		} else if (tempHeader == "=weapons=") {
			weaponCode.push_back(s);
		}
		if (debugMods) {
			cout << "header: " << tempHeader << " num: " << c << " token: " << s << "\n";
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
			if (debugMods) {
				cout << "error (metadata) no: " << modLoadingErrors << " token: " << i << " name: " << dataCode[i] << "\n";
			}
		}
	}
	parseAttacks(attackCode);
	parseFoods(foodCode);
	parseArmor(armorCode);
	parseWeapons(weaponCode);
	parseEntities(entityCode);
}
void parseAttacks(vector<string> &attackCode)
{
	// Parse Attacks
	for(int i = 1;i < attackCode.size();i++) {
		string s = attackCode[i];
		if(s[0] == '-') {
			attack temp;
			temp.name = s.substr(1);
			int j = i + 1;
			while(j < attackCode.size() && attackCode[j][0] != '-' && attackCode[j][0] != '=') {
				s = attackCode[j];
				if(s == "type") {
					temp.type = attackCode[++j];
				} else if (s == "base dmg") {
					temp.baseDmg = stoi(attackCode[++j]);
				} else if (s == "height") {
					temp.height = stoi(attackCode[++j]);
				} else {
					modLoadingErrors++;
					if (debugMods) {
						cout << "error (attacks) no: " << modLoadingErrors << " token: " << j << " name: " << attackCode[j] << "\n";
					}
				}
				j++;
			}
			worldAttacks.push_back(temp);
			i = j - 1;
		}
	}
}
void parseFoods (vector<string> &foodCode)
{
	// parse foods (big back)
	for(int i = 1;i < foodCode.size();i++) {
		string s = foodCode[i];
		if(s[0] == '-') {
			food temp;
			temp.name = s.substr(1);
			int j = i + 1;
			while(j < foodCode.size() && foodCode[j][0] != '-' && foodCode[j][0] != '=') {
				s = foodCode[j];
				if (s == "heal") {
					temp.heal = stoi(foodCode[++j]);
				} else if (s == "price") {
					temp.price = stoi(foodCode[++j]);
				} else {
					modLoadingErrors++;
					if (debugMods) {
						cout << "error (food) no: " << modLoadingErrors << " token: " << j << " name: " << foodCode[j] << "\n";
					}
				}
				j++;
			}
			worldFoods.push_back(temp);
			i = j - 1;
		}
	}
}
void parseArmor(vector<string> &armorCode)
{
	// parse armor
	for(int i = 1;i < armorCode.size();i++) {
		string s = armorCode[i];
		if (s[0] == '-') {
			armorPiece temp;
			temp.name = s.substr(1);
			int j = i + 1;
			while(j < armorCode.size() && armorCode[j][0] != '-' && armorCode[j][0] != '=') {
				s = armorCode[j];
				if(s == "protection") {
					temp.dmgReduction = stoi(armorCode[++j]);
				} else if(s == "height") {
					temp.height = stoi(armorCode[++j]);
				} else if(s == "price") {
					temp.price = stoi(armorCode[++j]);
				} else if(s == "tier") {
					temp.tier = stoi(armorCode[++j]);
				} else {
					modLoadingErrors++;
					if (debugMods) {
						cout << "error (armor) no: " << modLoadingErrors << " token: " << j << " name: " << armorCode[j] << "\n";
					}
				}
				j++;
			}
		worldArmor.push_back(temp);
		i = j - 1;
		}
	}
}
void parseWeapons(vector<string> &weaponCode)
{
	// parse weapons
	for(int i = 1;i < weaponCode.size();i++) {
		string s = weaponCode[i];
		if (s[0] == '-') {
			weapon temp;
			temp.name = s.substr(1);
			int j = i + 1;
			while(j < weaponCode.size() && weaponCode[j][0] != '-' && weaponCode[j][0] != '=') {
				s = weaponCode[j];
				if (s == "price") {
					temp.price = stoi(weaponCode[++j]);
				} else if (s == "dmg mult") {
					temp.dmgMult = stof(weaponCode[++j]);
				} else if (s == "tier") {
					temp.tier = stoi(weaponCode[++j]);
				} else {
					modLoadingErrors++;
					if(debugMods) {
						cout << "error (weapon) no: " << modLoadingErrors << " token: " << j << " name: " << weaponCode[j] << "\n";
					}
				}
				j++;
			}
			worldWeapons.push_back(temp);
			i = j - 1;
		}
	}
}

void parseEntities(vector<string> &entityCode)
{
	// FINALLY, PARSE THE DAMN ENTITIES
	for(int i = 1;i < entityCode.size();i++) {
		string s = entityCode[i];
		if (s[0] == '-') {
			entity temp;
			temp.name = s.substr(1);
			int j = i + 1;
			while(j < entityCode.size() && entityCode[j][0] != '-' && entityCode[j][0] != '=') {
				s = entityCode[j];
				if (s == "hp") {
					temp.hp = stoi(entityCode[++j]);
				} else if (s == "skill") {
					temp.skill = stoi(entityCode[++j]);
				} else if (s == "tier") {
					temp.tier = stoi(entityCode[++j]);
				} else if (s == "weapon") {
					j++;
					int success = 0;
					for(int k = 0; k < worldWeapons.size(); k++) {
						weapon tempWeapon = worldWeapons[k];
						if (tempWeapon.name == entityCode[j]) {
							temp.weapon = tempWeapon;
							success = 1;
							break;
						}
					}
					if (!success) {
						cout << "ERROR : COULD NOT FIND WEAPON " << entityCode[j] << " PLEASE PRESS CTRL+C TO CLOSE THE PROGRAM SAFELY" << "\n";
						cin.get();
					}
				} else if (s == "armor") {
					int finished = 0;
					while (!finished) {
						int success = 0;
						j++;
						for (int k = 0; k < worldArmor.size() && !finished; k++) {
							armorPiece tempArmor = worldArmor[k];
							if (tempArmor.name == entityCode[j]) {		
								if (tempArmor.height == 2) {
									temp.armor.tunic = tempArmor;
								} else {
									temp.armor.leggings = tempArmor;
								}
								success = 1;
								if (j + 1 < entityCode.size() && entityCode[j+1] == "and") {
									j++;
								} else {
									finished = 1;
								}
							}
						}
						if (!success) {
							cout << "ERROR : COULD NOT FIND ARMOR " << entityCode[j] << " PLEASE PRESS CTRL+C TO CLOSE THE PROGRAM SAFELY" << "\n";							
							cin.get();
						}
					}
				} else if (s == "attacks") {
					int finished = 0;
					while(!finished) {
						int success = 0;
						j++;
						for(int k = 0;k < worldAttacks.size() && !finished;k++) {
							attack tempAttack = worldAttacks[k];
							if (tempAttack.name == entityCode[j]) {
								success = 1;
								temp.attacks.push_back(tempAttack);
								if (j+1<entityCode.size()&&entityCode[j+1]=="and") {
									j++;
								} else {
									finished = 1;
								}
							}
						}
						if(!success) {
							cout << "ERROR : COULD NOT FIND ATTACK " << entityCode[j] << " PLEASE PRESS CTRL+C TO CLOSE THE PROGRAM SAFELY" << "\n";							
							cin.get();
						}
					}
				}
				j++;
			}
			worldEntities.push_back(temp);
			i = j - 1;
		}
	}
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
