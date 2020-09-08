#pragma once
#include <iostream>
#include <string>
#include <stdarg.h>
#include <stdio.h>
using namespace std;

void LogError(string msg) {
	cout << "[ERROR]:" << msg << endl;
}