#include "Memory.h"
#include "Exceptions.h"
using namespace std;

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

