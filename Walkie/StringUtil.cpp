#include "stdafx.h"

#include <string>

#include "StringUtil.h"

std::vector<std::string> splitLine(std::string line) {
	std::string word{};
	std::vector<std::string> splitLine{};
	for (int i = 0; i != line.size(); i++) {
		if (isspace(line[i]) || line[i] == '\0') {
			if (word != "") {
				splitLine.push_back(word);
				word.clear();
			}
		}
		else {
			word += line[i];
		}
		if (i == line.size() - 1) {
			if (word != "") {
				splitLine.push_back(word);
				word.clear();
			}
		}
	}
	return splitLine;
}
