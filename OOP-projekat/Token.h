#pragma once
#include <string>
#include <iostream>
using namespace std;

class Token {
private:
	double val_;
	int creatorFunct_;
	string name_;
	friend class ArithmeticOperation;
public:
	Token(string name, double val, int creator);
	Token(string name, int creator);
	void giveValue(double val);
	double getValue();
	int getCreator();
};