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
	fileName_ = fileName;
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
	string tokenName = "";
	char opType;
	vector<Token*> tokensForOp;
	int opLat;
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

		while (line[i] != ' ') {
			tokenName.push_back(line[i]);
			i++;
		}
		allTokens_.push_back(Token(tokenName, opNum));
		tokensForOp.push_back(&(allTokens_.back()));

		while (i < line.length()) {
			while ((line[i] != ' ') && (i < line.length())) {
				tokenName.push_back(line[i]);
				i++;
			}

			if ((tokenName[0] <= '9') && (tokenName[0] >= '0')) {
				allTokens_.push_back(Token(tokenName,stoi(tokenName), opNum));
				tokensForOp.push_back(&(allTokens_.back()));
			}
			else {
				for (auto& n : allTokens_) {
					if (n.name_ == tokenName) {
						tokensForOp.push_back(&n);
						break;
					}
				}
			}
			
		}
		
		waiting_.push_back(ArithmeticOperation(opNum, opLat, tokensForOp, opType));
		inputFile.close();
	}

}

void Machine::checkAllIfReady() {
	for (auto &n : waiting_) {
		checkIfReady(n);
	}
}

void Machine::checkAllIfFinished() {
	for (auto &n : waiting_) {
		checkIfFinished(n);
	}
}

void Machine::checkIfReady(ArithmeticOperation operationBeingChecked) {
	bool tokenReady;
	int creator;
	for (auto &n : operationBeingChecked.ports_) {
		creator = n->getCreator();
		tokenReady = 0;
		for (auto &r : completed_) {
			if (creator == r.operationID_) {
				tokenReady = 1;
				break;
			}
		}
		if (creator == operationBeingChecked.operationID_) tokenReady = 1;
		if (!tokenReady) return;
	}
	operationBeingChecked.execute();
	operationBeingChecked.addTimeOfStart(timeCounter);
	moveToExecuting(operationBeingChecked.operationID_);
	writeToLog(fileName_, operationBeingChecked.operationID_, operationBeingChecked.latency_);
}

void Machine::checkIfFinished(ArithmeticOperation operationBeingChecked) {
	if (timeCounter >= operationBeingChecked.timeOfStart_ + operationBeingChecked.latency_) moveToCompleted(operationBeingChecked.operationID_);
}