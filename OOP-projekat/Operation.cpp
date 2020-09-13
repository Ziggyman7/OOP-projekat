#include "Operation.h"
#include "Memory.h"
using namespace std;

void PlusOperation::execute() {
	cout << "executing " << (*ports_[0]).name_ << " = " << (*ports_[1]).val_ << " + " << (*ports_[2]).val_ << "\n";

	ports_[0]->val_ = ports_[1]->val_ + ports_[2]->val_;
}

void TimesOperation::execute() {
	cout << "executing " << (*ports_[0]).name_ << " = " << (*ports_[1]).val_ << " * " << (*ports_[2]).val_ << "\n";

	(*ports_[0]).val_ = (*ports_[1]).val_ * (*ports_[2]).val_;
}

void PowOperation::execute() {
	cout << "executing " << (*ports_[0]).name_ << " = " << (*ports_[1]).val_ << " ^ " << (*ports_[2]).val_ << "\n";

	(*ports_[0]).val_ = pow((*ports_[1]).val_, (*ports_[2]).val_);
}

void EqualsOperation::execute() {
	cout << "executing " << (*ports_[0]).name_ << " = " << (*ports_[1]).val_ << "\n";

	(*ports_[0]).val_ = (*ports_[1]).val_;
	Memory::getInstance()->set((*ports_[0]).name_, (*ports_[0]).val_);
}


void ArithmeticOperation::addTimeOfStart(int time) { 
	timeOfStart_ = time; 
}