#pragma once
#include <string>
#include <iostream>
using namespace std;

class Token {
private:
	double val_;
	int creatorFunct_;
	string name_;
	friend class PlusOperation;
	friend class PowOperation;
	friend class EqualsOperation;
	friend class TimesOperation;
	friend class Machine;
public:
	Token(string name, double val, int creator);
	Token(string name, int creator);
	void giveValue(double val);
	double getValue();
	int getCreator();
};