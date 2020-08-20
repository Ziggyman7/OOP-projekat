#include <iostream>
#include <string>
#include <stack>
#include <fstream>
#include "Compiler.h"

using namespace std;

Compiler* Compiler::instance_ = nullptr;

int getPriority(char a) {
	if (a == '=') return 0;
	if (a == '+') return 1;
	if (a == '*') return 2;
	if (a == '^') return 3;
	return 0;
}

int isOperator(char a) {
	if (a == '=') return 1;
	if (a == '+') return 1;
	if (a == '*') return 1;
	if (a == '^') return 1;
	return 0;
}

Compiler* Compiler::getInstance() {
	if (instance_ == nullptr) {
		instance_ = new Compiler();
	}
	return instance_;
}

void Compiler::compile(const string& fileIn, CompilationType compilationType) {
	fstream inputFile(fileIn + ".txt", ios::in),
			outputFile(fileIn + ".imf", ios::out);
	string line;
	string postLine;
	int lineCounter_ = 1;
	int tCounter_ = 1;
	int k = 0;
	string helper1, helper2;
	stack<string> charStack;
	while (getline(inputFile, line)) {
		postLine = toPostfix(line);
		cout << postLine<<"\n";
		
		for (int i = 0; i < postLine.length(); i++) {
			if (postLine[i] == ' ') continue;
			if (!isOperator(postLine[i])) {
				k = 0;
				while (postLine[k+i] != ' ') k++;
				charStack.push(string(postLine,i,k));
				i = i + k-1;
			} else {
				helper1 = charStack.top();
				charStack.pop();
				helper2 = charStack.top();
				charStack.pop();
				if (postLine[i] == '=') {
					outputFile << "[" << lineCounter_++ << "] " << "= " << helper2 << " " << helper1 << endl;
				} else {
					outputFile << "[" << lineCounter_++ << "] " << postLine[i] << " t" << tCounter_++ << " " << helper2 << " " << helper1 << endl;
					helper1 = 't' + to_string(tCounter_ -1);
					charStack.push(helper1);
				}
			}
		}
	}
	inputFile.close();
	outputFile.close();
}

string Compiler::toPostfix(const string& in){
	stack<char> opStack;
	string outLine;
	for (int i = 0; i < in.length(); i++) {
		if (isOperator(in[i])) {
			if (opStack.empty()) {
				opStack.push(in[i]);
			}
			else if (getPriority(opStack.top()) < getPriority(in[i])) {
				opStack.push(in[i]);
			}
			else {
				while (getPriority(opStack.top()) >= getPriority(in[i])) {
					outLine += opStack.top();
					outLine += ' ';
					opStack.pop();
				}
				opStack.push(in[i]);
			}

		}
		else if (in[i] == ' ') continue;
		else {
			outLine += in[i];
			if((in[i]!='-')&&(in[i]!='.') && (in[i+1] != '.'))outLine += ' ';
		}

	}
	while (!opStack.empty()) {
		outLine += opStack.top();
		outLine += ' ';
		opStack.pop();
	}
	return outLine;
}