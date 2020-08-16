#include <iostream>
#include <string>
#include "Compiler.h"
#include "Machine.h"

using namespace std;

int main() {
	string nameOfFile;
	cin >> nameOfFile;
	Compiler::getInstance()->compile(nameOfFile, CompilationType::SIMPLE);
	Machine::getInstance()->exec(nameOfFile);
	char a;
	cout << "Pritisnite bilo koje dugme za kraj.";
	cin >> a;
}