/*2014.5.6 测试环境: mingw*/
#include <iostream>
#include <vector>
#include <stack>
#include <string>
#include <cstdlib>
#include <cctype>
#include <cstring>
using namespace std;

vector<string> Parse(char *str){
	vector<string> tokens;
	int len = strlen(str);
	char *p = (char *)malloc((len + 1)*sizeof(char));
	int i = 0, j = 0;
	while (i<len){
		if (str[i] == ' '){
			i++;
			continue;
		}
		p[j++] = str[i++];
	}
	p[j] = '\0';
	j = 0;
	len = strlen(p);
	while (j<len){
		char temp[2];
		string token;
		switch (p[j]){
		case '+':
		case '*':
		case '/':
		case '(':
		case ')':{
			temp[0] = p[j];
			temp[1] = '\0';
			token = temp;
			tokens.push_back(token);
			break;
		}
		case '-':{
			if (p[j - 1] == ')' || isdigit(p[j - 1])){
				temp[0] = p[j];
				temp[1] = '\0';
				token = temp;
				tokens.push_back(token);
			}
			else{
			temp[0] = '#';
			temp[1] = '\0';
			token = temp;
			tokens.push_back(token);
			}
			break;
		}
		default:{
			i = j;
			while (isdigit(p[i]) && i<len){
				i++;
			}
			char *op = (char *)malloc(i - j + 1);
			strncpy(op, p + j, i - j);
			op[i - j] = '\0';
			token = op;
			tokens.push_back(token);
			j = i - 1;
			break;
		}
		}
		j++;
	}
	return tokens;
}
int Priority(string op){
	int priority;
	if (op == "#")
		priority = 3;
	else if (op == "*" || op == "/")
		priority = 2;
	else if (op == "+" || op == "-")
		priority = 1;
	else if (op == "(")
		priority = 0;
	return priority;
}

void calculate(stack<int> &opdStack, string opt){
	if (opt == "#"){
		int opd = opdStack.top();
		int result = 0 - opd;
		opdStack.pop();
		opdStack.push(result);
	}
	else if (opt == "+"){
		int rOpd = opdStack.top();
		opdStack.pop();
		int lOpd = opdStack.top();
		opdStack.pop();
		int result = lOpd + rOpd;
		opdStack.push(result);
	}
	else if (opt == "-"){
		int rOpd = opdStack.top();
		opdStack.pop();
		int lOpd = opdStack.top();
		opdStack.pop();
		int result = lOpd - rOpd;
		opdStack.push(result);
	}
	else if (opt == "*"){
		int rOpd = opdStack.top();
		opdStack.pop();
		int lOpd = opdStack.top();
		opdStack.pop();
		int result = lOpd * rOpd;
		opdStack.push(result);
	}
	else if (opt == "/"){
		int rOpd = opdStack.top();
		opdStack.pop();
		int lOpd = opdStack.top();
		opdStack.pop();
		int result = lOpd / rOpd;
		opdStack.push(result);
	}
}

int MidExpression(char *str){
	vector<string> tokens = Parse(str);
	int i = 0;
	int size = tokens.size();
	stack<int> S;
	stack<string> oS;
    for(i = 0; i<size; i++){
		string token = tokens[i];
		if (token == "#" || token == "+" || token == "-" || token == "*" || token == "/")
		{
			if (oS.size() == 0)//如果操作符栈为空
			{
				oS.push(token);
			}
			else
			{
				int tokenPriority = Priority(token);
				string topOpt = oS.top();
				int topOptPriority = Priority(topOpt);
				if (tokenPriority>topOptPriority)
				{
					oS.push(token);
				}
				else{
					while (tokenPriority <= topOptPriority){
						oS.pop();calculate(S, topOpt);
						if (oS.size()>0)
						{
							topOpt = oS.top();
							topOptPriority = getPriority(topOpt);
						}
						else
							break;

					}
					oS.push(token);
				}
			}
		}
		else if (token == "(")
		{
			oS.push(token);
		}
		else if (token == ")")
		{
			while (oS.top() != "(")
			{
				string topOpt = oS.top();
				calculate(S, topOpt);
				oS.pop();
			}
			oS.pop();
		}
		else
		{
			S.push(atoi(token.c_str()));
		}
	}
	while (oS.size() != 0)
	{
		string topOpt = oS.top();
		calculate(S, topOpt);
		oS.pop();
	}
	return S.top();
}


int main(int argc, char *argv[])
{
	char *str = "4+6+9/23";
	cout << MidExpression(str) << endl;
	system("pause");
	return 0;
}