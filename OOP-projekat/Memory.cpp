#include "Memory.h"
#include "Exceptions.h"
#include <fstream>
using namespace std;

Memory* Memory::instance_ = nullptr;

Memory* Memory::getInstance(){
	if (instance_ == nullptr) {
		instance_ = new Memory();
	}
	return instance_;
}

void Memory::set(string varName, double val) {
	int oldVar = 0;
	for (auto& n : theMemory_) {
		if (n.elemName_ == varName) {
			n.elemVal_ = val;
			oldVar = 1;
			break;
		}
	}
	if (!oldVar) {
		theMemory_.push_back(MemElem(varName, val));
	}
}

double Memory::get(string name) {
	for (auto& n : theMemory_) {
		if (n.elemName_ == name) {
			return n.elemVal_;
		}
	}
	throw new VarNotAvailableException("Ne postoji varijabla sa tim imenom.");
}

vector<MemElem> Memory::getAll() {
	return theMemory_;
}

void Memory::writeMem(string fileName) {
	fstream outputFile(fileName + ".mem", ios::out);
	for (auto& n : theMemory_) {
		outputFile << n.elemName_ << " = " << n.elemVal_ << "\n";
	}
	outputFile.close();
}