#include<iostream>
#include<string>
#include<sstream>
#include<fstream> 
#include<vector>

using namespace std;
vector<string> str;
 
int findTerminal = 0;

//每个状态的函数
void P(int treeWidth);
void L(int treeWidth);
void P1(int treeWidth);
void S(int treeWidth);
void S1(int treeWidth);
void C(int treeWidth);
void C1(int treeWidth);
void E(int treeWidth);
void E1(int treeWidth);
void T(int treeWidth);
void T1(int treeWidth);
void F(int treeWidth);

//判断是否为字符,OCT,DEX,HEX
bool isId(string str) {
	for (int i = 0; i < str.size(); i++) {
		if ((str[i] >= '0' && str[i] <= '9') || (str[i] >= 'A' && str[i] <= 'Z') || (str[i] >= 'a' && str[i] <= 'z'));
		else return false;
	}
	return true;
}
//打印每一行的树的输出
void printTree(string s, int treeWidth) {
	for (int i = 0; i < treeWidth; i++) {
		cout << " ";
	}
	cout << s << endl;
}

//根据FIRST集判断候选式
void P(int treeWidth) {
	if (findTerminal < str.size()) {
		if (str[findTerminal] == "if" || str[findTerminal] == "while" || isId(str[findTerminal])) {
			printTree("P", treeWidth);
			L(treeWidth + 1);
			P1(treeWidth + 1);
		}
		else {
			cout << "ERROR";
			exit(0);
		}
	}
	else {
		cout << "ERROR";
		exit(0);
	}
}
void L(int treeWidth) {
	if (findTerminal < str.size()) {
		if (str[findTerminal] == "while" || str[findTerminal] == "if" || isId(str[findTerminal])) {
			printTree("L", treeWidth);
			S(treeWidth + 1);
			if (str[findTerminal] == ";") {
				findTerminal++;
				printTree(";", treeWidth + 1);
			}
			else {
				cout << "ERROR";
				exit(0);
			}
		}
		else {
			cout << "ERROR";
			exit(0);
		}
	}
	else {
		cout << "ERROR";
		exit(0);
	}
}
void P1(int treeWidth) {
	if (findTerminal < str.size()) {
		if (str[findTerminal] == "if" || str[findTerminal] == "while" || isId(str[findTerminal])) {
			printTree("P1", treeWidth);
			P(treeWidth + 1);
		}
		else {
			cout << "error";
			exit(0);
		}
	}
	else {//空集情况
		printTree("P1", treeWidth);
		printTree("null", treeWidth + 1);
	}
}
void S(int treeWidth) {
	if (findTerminal < str.size()) {
		if (str[findTerminal] == "if") {
			printTree("S", treeWidth);
			printTree("if", treeWidth + 1);
			findTerminal++;
			C(treeWidth + 1);
			if (str[findTerminal] == "then") {
				printTree("then", treeWidth + 1);
				findTerminal++;
				S(treeWidth + 1);
				S1(treeWidth + 1);
			}
			else {
				cout << "error";
				exit(0);
			}
		}
		else if (str[findTerminal] == "while") {
			printTree("S", treeWidth);
			printTree("while", treeWidth + 1);
			findTerminal++;
			C(treeWidth + 1);
			if (str[findTerminal] == "do") {
				printTree("do", treeWidth + 1);
				findTerminal++;
				S(treeWidth + 1);
			}
			else {
				cout << "error";
				exit(0);
			}
		}
		else if (isId(str[findTerminal])) {
			printTree("S", treeWidth);
			printTree(str[findTerminal], treeWidth + 1);
			findTerminal++;
			if (str[findTerminal] == "=") {
				findTerminal++;
				printTree("=", treeWidth + 1);
			}
			else {
				cout << "error";
				exit(0);
			}
			E(treeWidth + 1);
		}
		else {
			cout << "error";
			exit(0);
		}
	}
	else {
		cout << "error";
		exit(0);
	}

}
void S1(int treeWidth) {
	if (findTerminal < str.size()) {
		if (str[findTerminal] == "else") {
			printTree("S1", treeWidth);
			printTree("else", treeWidth + 1);
			findTerminal++;
			S(treeWidth + 1);
		}
		else if (str[findTerminal] == ";") {
			printTree("S1", treeWidth);
			printTree("null", treeWidth + 1);
		}
		else {
			cout << "error";
			exit(0);
		}
	}
	else {
		printTree("S1", treeWidth);
		printTree("null", treeWidth + 1);
	}
}
void C(int treeWidth) {
	if (findTerminal < str.size()) {
		if (str[findTerminal] == "(" || isId(str[findTerminal])) {
			printTree("C", treeWidth);
			E(treeWidth + 1);
			C1(treeWidth + 1);
		}
		else {
			cout << "error";
			exit(0);
		}
	}
	else {
		cout << "error";
		exit(0);
	}

}
void C1(int treeWidth) {
	if (findTerminal < str.size()) {
		if (str[findTerminal] == ">") {
			printTree("C1", treeWidth);
			printTree(">", treeWidth + 1);
			findTerminal++;
			E(treeWidth + 1);
		}
		else if (str[findTerminal] == "<") {
			printTree("C1", treeWidth);
			printTree("<", treeWidth + 1);
			findTerminal++;
			E(treeWidth + 1);
		}
		else if (str[findTerminal] == "=") {
			printTree("C1", treeWidth);
			printTree("=", treeWidth + 1);
			findTerminal++;
			E(treeWidth + 1);
		}
		else if (str[findTerminal] == ">=") {
			printTree("C1", treeWidth);
			printTree(">=", treeWidth + 1);
			findTerminal++;
			E(treeWidth + 1);
		}
		else if (str[findTerminal] == "<=") {
			printTree("C1", treeWidth);
			printTree("<=", treeWidth + 1);
			findTerminal++;
			E(treeWidth + 1);
		}
		else if (str[findTerminal] == "<>") {
			printTree("C1", treeWidth);
			printTree("<>", treeWidth + 1);
			findTerminal++;
			E(treeWidth + 1);
		}
		else {
			cout << "error";
			exit(0);
		}
	}
	else {
		cout << "error";
		exit(0);
	}
}
void E(int treeWidth) {
	if (findTerminal < str.size()) {
		if (str[findTerminal] == "(" || isId(str[findTerminal])) {
			printTree("E", treeWidth);
			T(treeWidth + 1);
			E1(treeWidth + 1);
		}
		else {
			cout << "error";
			exit(0);
		}
	}
	else {
		cout << "error";
		exit(0);
	}
}
void E1(int treeWidth) {
	if (findTerminal < str.size()) {
		if (str[findTerminal] == "+") {
			printTree("E1", treeWidth);
			printTree("+", treeWidth + 1);
			findTerminal++;
			T(treeWidth + 1);
			E1(treeWidth + 1);
		}
		else if (str[findTerminal] == "-") {
			printTree("E1", treeWidth);
			printTree("-", treeWidth + 1);
			findTerminal++;
			T(treeWidth + 1);
			E1(treeWidth + 1);
		}
		else if (str[findTerminal] == ">" || str[findTerminal] == "<" || str[findTerminal] == "=" || str[findTerminal] == ">=" || str[findTerminal] == "<=" || str[findTerminal] == "<>" || str[findTerminal] == ")" || str[findTerminal] == "do" || str[findTerminal] == "then" || str[findTerminal] == ";" || str[findTerminal] == "*" || str[findTerminal] == "/" || str[findTerminal] == "while" || str[findTerminal] == "if") {
			printTree("E1", treeWidth);
			printTree("null", treeWidth + 1);
		}
		else {
			cout << "error";
			exit(0);
		}
	}
	else {
		printTree("E1", treeWidth);
		printTree("null", treeWidth + 1);
	}
}
void T(int treeWidth) {
	if (findTerminal < str.size()) {
		if (str[findTerminal] == "(" || isId(str[findTerminal])) {
			printTree("T", treeWidth);
			F(treeWidth + 1);
			T1(treeWidth + 1);
		}
		else {
			cout << "Error";
			exit(0);
		}
	}
	else {
		cout << "Error";
		exit(0);
	}

}
void T1(int treeWidth) {
	if (findTerminal < str.size()) {
		if (str[findTerminal] == "*") {
			printTree("T1", treeWidth);
			printTree("*", treeWidth + 1);
			findTerminal++;
			F(treeWidth + 1);
			T1(treeWidth + 1);
		}
		else if (str[findTerminal] == "/") {
			printTree("T1", treeWidth);
			printTree("/", treeWidth + 1);
			findTerminal++;
			F(treeWidth + 1);
			T1(treeWidth + 1);
		}
		else if (str[findTerminal] == "+" || str[findTerminal] == "-" || str[findTerminal] == ">" || str[findTerminal] == "<" || str[findTerminal] == "=" || str[findTerminal] == ">=" || str[findTerminal] == "<=" || str[findTerminal] == "<>" || str[findTerminal] == ")" || str[findTerminal] == "do" || str[findTerminal] == "then" || str[findTerminal] == ";" || str[findTerminal] == "while" || str[findTerminal] == "if") {
			printTree("T1", treeWidth);
			printTree("null", treeWidth + 1);
		}
		else {
			cout << "Error";
			exit(0);
		}
	}
	else {
		printTree("T1", treeWidth);
		printTree("null", treeWidth + 1);
	}
}
void F(int treeWidth) {
	if (findTerminal < str.size()) {
		if (str[findTerminal] == "(") {
			printTree("F", treeWidth);
			printTree("(", treeWidth + 1);
			findTerminal++;
			E(treeWidth + 1);
			if (str[findTerminal] == ")") {
				findTerminal++;
				printTree(")", treeWidth + 1);
			}
			else {
				cout << "Error";
				exit(0);
			}
		}
		else if (isId(str[findTerminal])) {
			printTree("F", treeWidth);
			printTree(str[findTerminal], treeWidth + 1);
			printTree(str[findTerminal], treeWidth + 1);
			findTerminal++;
		}
		else {
			cout << "Error";
			exit(0);
		}
	}
	else {
		cout << "Error";
		exit(0);
	}
}

int main(void) {
	ifstream ReadFile;
	string tmp;
	ReadFile.open("try.txt");
	if (ReadFile.fail())//文件打开失败:返回0
	{
		return 0;
	}
	else {
		while (getline(ReadFile, tmp))
		{
			stringstream ss(tmp);
			string substr;
			while (ss >> substr) {
				str.push_back(substr);
			}
			cout << tmp << endl;
		}
		P(0);
		ReadFile.close();
	}
	cout << "correct";
	return 0;
}
