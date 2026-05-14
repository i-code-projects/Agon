#pragma once
#include <string>
#include <vector>

int isCodeHeader(std::string header);
void loadTokens(std::vector<std::string> &from, std::vector<std::string> &to, long long &c);
void modParser(std::string path);
void modLoader();
