#pragma once
#include <iostream>
using namespace std;

enum CompilationType {
	SIMPLE, ADVANCED
};



class Compiler {
public:
	void compile(const string& fileIn, CompilationType compilationType);
	static Compiler* getInstance();
private:
	Compiler() {};
	static Compiler* instance_;
	string toPostfix(const string& line);
};