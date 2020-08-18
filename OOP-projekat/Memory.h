#pragma once
#include <vector>
#include <iostream>
#include <string>

using namespace std;

struct MemElem {
	string elemName_;
	double elemVal_;

	MemElem(string name, double val) {
		elemName_ = name;
		elemVal_ = val;
	}
};

class Memory {
public:
	void set(string varName, double val);
	double get(string varName);
	vector<MemElem> getAll();
	static Memory* getInstance();
	friend class ArithmeticOperation;
	void writeMem(string fileName);
private:
	vector<MemElem> theMemory_;
	Memory() {};
	static Memory* instance_;
};