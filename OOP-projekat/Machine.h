#pragma once
#include "Memory.h"
#include "Operation.h"
#include "Compiler.h"

class Machine {
public:
	static Machine* getInstance();
	void exec(string fileName);
	
private:
	Machine() {};
	static Machine* instance_;
	vector<ArithmeticOperation> waiting_;
	vector<ArithmeticOperation> executing_;
	vector<ArithmeticOperation> completed_;
	vector<Token> allTokens_;
	int timeCounter;
	string fileName_;
	int Ta_, Tm_, Te_, Tw_, Nw_;
	CompilationType cType_;
	void moveToExecuting(int opNumber);
	void moveToCompleted(int opNumber);
	void readIMF(string fileName);									//
	void readConfig();												//
	void writeToMem(string fileName);
	void writeToLog(string fileName, int operationNumber, int lag);
	void checkIfReady(ArithmeticOperation operationToBeChecked);	//
	void checkAllIfReady();											//
	void checkIfFinished(ArithmeticOperation operationToBeChecked);	//
	void checkAllIfFinished();										//
};			