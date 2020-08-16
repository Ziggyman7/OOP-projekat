#pragma once

#include <exception>
#include <iostream>

using namespace std;

class VarNotAvailableException : public exception {
public:
	VarNotAvailableException(const char* msg) : exception(msg) {}
};