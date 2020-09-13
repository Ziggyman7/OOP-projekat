#pragma once
 
#include "Token.h"
#include <vector>

using namespace std;

class Operation {
public:
	virtual void execute() = 0;
	Operation(int id, int lat, vector<Token*> tokens) {
		ports_ = tokens;
		operationID_ = id;
		latency_ = lat;
	}
	friend class Machine;

protected:
	vector<Token*> ports_;
	int operationID_;
	int latency_;
	int timeOfStart_;
};


class ArithmeticOperation : public Operation {
public:
	virtual void execute() = 0;
	ArithmeticOperation(int id, int lat, vector<Token*> tokens, char type) : Operation(id, lat, tokens) { type_ = type; }
	void addTimeOfStart(int time);

	int getStartTime() const {
		return timeOfStart_;
	}

	int getLatency() const {
		return latency_;
	}

	int getOperationId() const {
		return operationID_;
	}

	friend class Machine;
private:
	char type_;
};

class PlusOperation : public ArithmeticOperation {
public:
	void execute();
	PlusOperation(int id, int lat, vector<Token*> tokens, char type) : ArithmeticOperation(id, lat, tokens, type) {};
};

class TimesOperation : public ArithmeticOperation {
public:
	void execute();
	TimesOperation(int id, int lat, vector<Token*> tokens, char type) : ArithmeticOperation(id, lat, tokens, type) {};
};

class PowOperation : public ArithmeticOperation {
public:
	void execute();
	PowOperation(int id, int lat, vector<Token*> tokens, char type) : ArithmeticOperation(id, lat, tokens, type) {};
};

class EqualsOperation : public ArithmeticOperation {
public:
	void execute();
	EqualsOperation(int id, int lat, vector<Token*> tokens, char type) : ArithmeticOperation(id, lat, tokens, type) {};
};