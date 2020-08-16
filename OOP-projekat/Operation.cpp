#include "Operation.h"
#include "Memory.h"
using namespace std;

void ArithmeticOperation::execute(Token result){
	switch (type_) {

		case '+': {
			result.val_ = (ports_[0]).val_ + (ports_[1]).val_;
		}
		case '*': {
			result.val_ = (ports_[0]).val_ * (ports_[1]).val_;
		}
		case '^': {
			result.val_ = pow((ports_[0]).val_, (ports_[1]).val_);
		}
		case '=': {
			result.val_ = (ports_[0]).val_;
			Memory::getInstance()->set((ports_[0]).name_, (ports_[0]).val_);
		}
	}
}

void ArithmeticOperation::addTimeOfStart(int time) { timeOfStart_ = time; }