#include <iostream>

#include "Compiler.h"

using namespace std;

int main() {
	Compiler::getInstance()->compile("test", CompilationType::SIMPLE);
	int a;
	cin >> a;
}