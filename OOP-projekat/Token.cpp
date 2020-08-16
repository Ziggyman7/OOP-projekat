#include "Token.h"


Token::Token(string name, double val, int creator) {
	name_ = name;
	val_ = val;
	creatorFunct_ = creator;
}

Token::Token(string name, int creator) {
	creatorFunct_ = creator;
	name_ = name;
}

void Token::giveValue(double val) {
	val_ = val;
}

double::Token::getValue() { return val_; }

int Token::getCreator() { return creatorFunct_; }