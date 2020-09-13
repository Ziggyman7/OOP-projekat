#include "Machine.h"
#include <fstream>
#include <algorithm>

using namespace std;

Machine* Machine::instance_ = nullptr;

Machine* Machine::getInstance() {
	if (instance_ == nullptr)
		instance_ = new Machine();
	return instance_;
}

void Machine::exec(string fileName) {
	fileName_ = fileName;
	readIMF(fileName);
	for (auto &n : waiting_) {
		cout << "operation number " << n->operationID_ << " : " << "operation type " << n->type_ << " : " << "operation latency " << n->latency_ << " : ";
		for (int i=0; i<n->ports_.size();i++) cout << "port["<<i<< "] creator " << n->ports_[i]->creatorFunct_;
		cout << "\n";

	}
	//for (auto&n : allTokens_) cout << n->name_ << ":" << n->val_ << ":" << n->creatorFunct_<<"\n";
	fstream output(fileName + ".log", ios::out);
	output.flush();
	output.close();
	timeCounter_ = 0;
	numberOfAssingments_ = 0;
	while (!(waiting_.empty() && executing_.empty())) {
		checkAllIfReady();
		checkAllIfFinished();
		/*cout<<"Time:"<<*/timeCounter_++;// << "\n";
		/*for (auto &n : waiting_) {
			cout << "operation in waiting number " << n.operationID_ << " : " << "operation type " << n.type_ << " : " << "operation latency " << n.latency_ << " : " << "result = " << n.ports_[0]->val_;
			cout << "\n";
		}
		cout << "\n";

		for (auto &n : executing_) {
			cout << "operation in executing number " << n.operationID_ << " : " << "operation type " << n.type_ << " : " << "operation latency " << n.latency_ << " : " << "result = " << n.ports_[0]->val_;
			cout << "\n";
		}
		cout << "\n";

		for (auto &n : completed_) {
			cout << "operation in completed number " << n.operationID_ << " : " << "operation type " << n.type_ << " : " << "operation latency " << n.latency_ << " : " << "result = " << n.ports_[0]->val_;
			cout << "\n";
		}
		cout << "\n";*/

	}
	writeToMem(fileName);
	writeToLog(fileName);

	for (int i = 0; i < allTokens_.size(); i++) {
		delete allTokens_[i];
	}
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
		case 22: {
			cType_ = ADVANCED;
			}
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
		allTokens_.push_back(new Token(tokenName, opNum));
		tokensForOp.push_back(allTokens_.back());
		i++;
		while (i < line.length()) {
			tokenName = "";
			while ((line[i] != ' ') && (i < line.length())) {
				tokenName.push_back(line[i]);
				i++;
			}
			i++;
			if (((tokenName[0] <= '9') && (tokenName[0] >= '0'))||tokenName[0]=='-') {
				allTokens_.push_back(new Token(tokenName,stod(tokenName), opNum));
				tokensForOp.push_back(allTokens_.back());
			}
			else {
				for (auto& n : allTokens_) {
					if (n->name_ == tokenName) {
						tokensForOp.push_back(n);
						break;
					}
				}
			}
			
		}
		switch (opType)
		{
		case '=': {
			waiting_.push_back(new EqualsOperation(opNum, opLat, tokensForOp, opType));
			break; }
		case '+': {
			waiting_.push_back(new PlusOperation(opNum, opLat, tokensForOp, opType));
			break; }
		case '*': {
			waiting_.push_back(new TimesOperation(opNum, opLat, tokensForOp, opType));
			break; }
		case '^': {
			waiting_.push_back(new PowOperation(opNum, opLat, tokensForOp, opType));
			break; }
		}
	}
	inputFile.close();
	
}

void Machine::checkAllIfReady() {
	for (int i = 0; i < waiting_.size();i++) {
		i-=checkIfReady(waiting_[i]);
	}
}			

void Machine::checkAllIfFinished() {
	for (int i = 0; i < executing_.size(); i++) {
		i -= checkIfFinished(executing_[i]);
	}
}

bool Machine::checkIfReady(ArithmeticOperation* operationBeingChecked) {
	bool tokenReady;
	int creator;
	for (int i = 0; i < operationBeingChecked->ports_.size();i++) {
		creator = operationBeingChecked->ports_[i]->creatorFunct_;
		tokenReady = 0;
		for (auto &r : completed_) {
			if (creator == r->operationID_) {
				tokenReady = 1;
				break;
			}
		}
		if (creator == operationBeingChecked->operationID_) tokenReady = 1;
		if (!tokenReady) return tokenReady;
	}
	if ((operationBeingChecked->type_ == '=') && numberOfAssingments_ >= Nw_) return 0;
	moveToExecuting(operationBeingChecked->operationID_);
	if (operationBeingChecked->type_ == '=') numberOfAssingments_++;
	return tokenReady;
}

bool Machine::checkIfFinished(ArithmeticOperation* operationBeingChecked) {
	if (timeCounter_ >= (operationBeingChecked->timeOfStart_ + operationBeingChecked->latency_ - 1)) {
		if (operationBeingChecked->type_ == '=') numberOfAssingments_--;
		//cout << "operation that just finished number " << operationBeingChecked.operationID_ << " : " << "operation type " << operationBeingChecked.type_ << " : " << "operation latency " << operationBeingChecked.latency_ << " : " << "result = " << operationBeingChecked.ports_[0]->val_<<"\n";
		moveToCompleted(operationBeingChecked->operationID_);
		return 1;
	}
	return 0;
}

void Machine::writeToLog(string& fileName) {
	fstream output(fileName+".log", ios::app);
	sort(completed_.begin(), completed_.end(), [](const ArithmeticOperation* op1, const ArithmeticOperation* op2) {
		if (op1->getStartTime() != op2->getStartTime()) {
			return op1->getStartTime() < op2->getStartTime();
		} else if ((op1->getStartTime() + op1->getLatency()) != (op2->getStartTime() + op2->getLatency())) {
			return (op1->getStartTime() + op1->getLatency()) < (op2->getStartTime() + op2->getLatency());
		} else {
			return op1->getOperationId() < op2->getOperationId();
		}
	});
	for (const auto& op : completed_) {
		output << "[" << op->getOperationId() << "]\t(" << op->getStartTime()  << "-" << op->getStartTime() + op->getLatency() << ")ns" << endl;
	}
	output.close();
}

void Machine::moveToExecuting(int opNum) {
	ArithmeticOperation* helper = waiting_[0];
	for (int i = 0; i < waiting_.size(); i++) {
		if (waiting_[i]->operationID_ == opNum) {
			helper = waiting_[i];
			waiting_.erase(waiting_.begin()+i);
			break;
		}
	}
	helper->execute();
	helper->addTimeOfStart(timeCounter_);
	executing_.push_back(helper);
}

void Machine::moveToCompleted(int opNum) {
	ArithmeticOperation* helper = executing_[0];
	for (int i = 0; i < executing_.size(); i++) {
		if (executing_[i]->operationID_ == opNum) {
			helper = executing_[i];
			executing_.erase(executing_.begin() + i);
			break;
		}
	}
	completed_.push_back(helper);
}

void Machine::writeToMem(string fileName) {
	Memory::getInstance()->writeMem(fileName);
}