#include <iostream>
#include <string>
#include "Compiler.h"
#include "Machine.h"

using namespace std;

int main() {
	string nameOfFile;
	cout << "Unesite ime fajla programa.\n";
	cin >> nameOfFile;
	Machine::getInstance()->readConfig();
	Compiler::getInstance()->compile(nameOfFile, Machine::getInstance()->getType());
	Machine::getInstance()->exec(nameOfFile);
	char a;
	cout << "Unesite bilo sta za izlazak iz programa.";
	cin >> a;
}