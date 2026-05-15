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

int isCodeHeader(string header) {
	if (header == "=data=" || header == "=armor=" || header == "=weapons=" || header == "=attacks=" || header == "=food=" || header == "=entities=") {
		return 1;
	}
	return 0;
}

void loadTokens(vector<string> &from, vector<string> &to, long long &c) {
	while(c + 1 < from.size() && !isCodeHeader(from[c+1])) {
		to.push_back(from[c]);
		if (debugMods) {
			cout << "number: " << c << " value: " << from[c] << "\n";
		}
		c++;
	}
	to.push_back(from[c]);
	c++;
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
		} else {
			c++;
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
	// Parse Attacks
	for(int i = 1;i < attackCode.size();i++) {
		string s = attackCode[i];
		if(s[0] == '-') {
			attack temp;
			temp.name = s.substr(1);
			i++;
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
	// parse foods (big back)
	for(int i = 1;i < foodCode.size();i++) {
		string s = foodCode[i];
		if(s == "-") {
			food temp;
			temp.name = s.substr(1);
			i++;
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
