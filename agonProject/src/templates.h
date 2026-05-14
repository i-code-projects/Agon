#pragma once
#include <vector>
#include <string>

struct armorPiece {
	std::string name = "none";
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
	std::string name = "harry cok's stick";
	float dmgMult = 1.0;
	int tier = 1;
	int price = 0;
};

struct attack {
	std::string name = "punch";
	std::string type = "normal";
	int baseDmg = 10;
	int height = 1;
};

struct food {
	std::string name = "her cake";
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
	std::string name = "your sweet nana";
	entityArmorComponent armor;
	weapon weapon = {"none",1.0};
	std::vector<attack> attacks = {{"punch","normal",10}};
	std::vector<food> inventory;
};
