#include "Machine.h"
#include <fstream>
using namespace std;

Machine* Machine::instance_ = nullptr;

Machine* Machine::getInstance() {
	if (instance_ == nullptr)
		instance_ = new Machine();
	return instance_;
}

void Machine::exec(string fileName) {
	cout << "a";
	readConfig();
	cout << "b";
	readIMF(fileName);
	cout << "c";
	fileName_ = fileName;
	timeCounter = 0;
	int i = 1;
	fstream output(fileName + ".log", ios::out);
	output.flush();
	output.close();
	cout << "d";
	while (!(waiting_.empty() && executing_.empty())) {
		checkAllIfReady();
		checkAllIfFinished();
		timeCounter++;
		cout <<i++;
	}
	cout << "e";
	writeToMem(fileName);
	cout << "f";
}

void Machine::readConfig() {
	fstream inputfile("config.txt", ios::in);
	string line;
	string variable;
	int value;
	while (getline(inputfile, line)) {
		variable = string(1,line[0]) + line[1];
		switch (line.length()) {
		case 6: {
			value = line[5] - '0';
			break; }
		case 7: {
			value = (line[5]-48)*10 + line[6]-48;
			break; }
		case 8: {
			value = ((line[5]-48)*10 + line[6]-48)*10 + line[7];
			break; }
		case 20: {
			cType_ = SIMPLE;
			break; }
		default: cType_ = ADVANCED;
		}
		if (variable == "Ta") { Ta_ = value; }
		if (variable == "Tm") { Tm_ = value; }
		if (variable == "Tw") { Tw_ = value; }
		if (variable == "Te") { Te_ = value; }
		if (variable == "Nw") { Nw_ = value; }
	}
	inputfile.close();
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
		tokenName = "";
		tokensForOp.clear();
		opNum = (line[1])-48;
		int i = 2;
		while (line[i] != ']') {
			opNum = opNum * 10 + (line[i])-48;
			i++;
		}
		i = i + 2;

		opType = line[i];
		i = i + 2;

		switch (opType)
		{
		case '=': {
			opLat = Tw_;
			break; }
		case '+': {
			opLat = Ta_;
			break; }
		case '*': {
			opLat = Tm_;
			break; }
		case '^': {
			opLat = Te_;
			break; }
		}

		while (line[i] != ' ') {
			tokenName.push_back(line[i]);
			i++;
		}
		allTokens_.push_back(Token(tokenName, opNum));
		tokensForOp.push_back(&(allTokens_.back()));
		i++;
		while (i < line.length()) {
			tokenName = "";
			while ((line[i] != ' ') && (i < line.length())) {
				tokenName.push_back(line[i]);
				i++;
			}
			i++;
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
		
	}
	inputFile.close();
	for (auto&n : tokensForOp) {
		cout << (*n).name_ << ":" << n->val_ << ":" << n->creatorFunct_ << "\n";
	}
	cout << "je iz imf\n";
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
	for (auto&n : operationBeingChecked.ports_) {
		cout << (*n).name_<< ":" << n->val_ << ":" << n->creatorFunct_<< "\n";
	}
	for (int i = 0; i < operationBeingChecked.ports_.size();i++) {
		creator = operationBeingChecked.ports_[i]->creatorFunct_;
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

void Machine::writeToLog(string fileName, int ID, int lag) {
	fstream outputfile(fileName + ".log", ios::app);
	outputfile << "[" << ID << "]\t(" << timeCounter << "-" << timeCounter + lag << ")ns\n";
	outputfile.close();
}

void Machine::moveToExecuting(int opNum) {
	ArithmeticOperation helper = waiting_[0];
	for (int i = 0; i < waiting_.size();i++) {
		if (waiting_[i].operationID_ = opNum) {
			ArithmeticOperation helper = waiting_[i];
			waiting_.erase(waiting_.begin()+i);
			break;
		}
	}
	executing_.push_back(helper);
}

void Machine::moveToCompleted(int opNum) {
	ArithmeticOperation helper = executing_[0];
	for (int i = 0; i < executing_.size(); i++) {
		if (executing_[i].operationID_ = opNum) {
			ArithmeticOperation helper = executing_[i];
			executing_.erase(executing_.begin() + i);
			break;
		}
	}
	completed_.push_back(helper);
}

void Machine::writeToMem(string fileName) {
	Memory::getInstance()->writeMem(fileName);
}