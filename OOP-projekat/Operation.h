#pragma once
 
#include "Token.h"
#include <vector>

using namespace std;

class Operation {
public:
	virtual void execute(Token result) = 0;
	Operation(int id, int lat, vector<Token> tokens) {
		ports_ = tokens;
		operationID_ = id;
		latency_ = lat;
	}
	friend class Machine;

protected:
	vector<Token> ports_;
	int operationID_;
	int latency_;
	int timeOfStart_;
};


class ArithmeticOperation : public Operation {
public:
	void execute(Token result);
	ArithmeticOperation(int id, int lat, vector<Token> tokens, char type) : Operation(id, lat, tokens) { type_ = type; }
	void addTimeOfStart(int time);
private:
	char type_;
};
