#include "Machine.h"
#include <fstream>
using namespace std;

Machine* Machine::getInstance() {
	if (instance_ == nullptr)
		instance_ = new Machine();
	return instance_;
}

void Machine::exec(string fileName) {
	readIMF(fileName);
	timeCounter = 0;
	readConfig();
	while (!(waiting_.empty() && executing_.empty())) {
		checkAllIfReady();
		checkAllIfFinished();
		timeCounter++;
	}
	writeToMem(fileName);
}

void Machine::readConfig() {
	fstream inputfile("config.txt", ios::in);
	string line;
	string variable;
	int value;
	while (getline(inputfile, line)) {
		variable = line[0] + line[1];
		switch (line.length) {
		case 6: {
			value = line[6];
			break; }
		case 7: {
			value = line[6] + line[7];
			break; }
		case 8: {
			value = line[6] + line[7] + line[8];
			break; }
		case 15: cType_= SIMPLE;
		default: cType_ = ADVANCED;
		}
		if (variable == "Ta") { Ta_ = value; }
		if (variable == "Tm") { Tm_ = value; }
		if (variable == "Tw") { Tw_ = value; }
		if (variable == "Te") { Te_ = value; }
		if (variable == "Nw") { Nw_ = value; }
		inputfile.close();
	}
}

void Machine::readIMF(string fileName) {
	fstream inputFile(fileName + ".imf", ios::in);
	string line;
	int opNum;
	string tokenName;
	char opType;
	while (getline(inputFile, line)) {
		opNum = line[1];
		int i = 2;
		while (line[i] != ']') {
			opNum = opNum * 10 + line[i];
			i++;
		}
		i = i + 2;
		opType = line[i];
		i = i + 2;
		while (i < line.length()) {
			while (line[i] != ' ') {

			}
		}
	}

}