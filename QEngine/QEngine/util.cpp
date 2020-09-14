#include "util.h"
void LogError(string msg) {
	cout << "[ERROR]:" << msg << endl;
}

string ItoStr(int i) {
	if (i == 0)return "0";
	string res = "";
	while (i) {
		res += i % 10;
		i /= 10;
	}
	return res;
}