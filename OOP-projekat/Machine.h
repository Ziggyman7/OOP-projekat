#pragma once
#include "Memory.h"
#include "Operation.h"
#include "Compiler.h"

class Machine {
public:
	static Machine* getInstance();
	void exec(string fileName);
	void readConfig();
	CompilationType getType() { return cType_; };
	
private:
	Machine() {};
	static Machine* instance_;
	vector<ArithmeticOperation*> waiting_;
	vector<ArithmeticOperation*> executing_;
	vector<ArithmeticOperation*> completed_;
	vector<Token*> allTokens_;
	int timeCounter_;
	string fileName_;
	int Ta_, Tm_, Te_, Tw_, Nw_;
	int numberOfAssingments_;
	CompilationType cType_;
	void moveToExecuting(int opNumber);								//
	void moveToCompleted(int opNumber);								//
	void readIMF(string fileName);									//
												//
	void writeToLog(string &fileName);								//
	void writeToMem(string fileName);								//
	bool checkIfReady(ArithmeticOperation* operationToBeChecked);	//
	void checkAllIfReady();											//
	bool checkIfFinished(ArithmeticOperation* operationToBeChecked);//
	void checkAllIfFinished();										//
};			