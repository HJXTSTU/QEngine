#pragma once
#include "gl_script_manager.h"
#include <unordered_map>
#include <functional>
using namespace std;

typedef function<BaseScriptPointer()> GenerateScriptFunc;
class ScriptFactory {
private:
	static unordered_map<string, GenerateScriptFunc> scriptFunc;
public:
	static BaseScriptPointer CreateScript(const string &type) {
		if (scriptFunc.find(type) != scriptFunc.end()) {
			return scriptFunc[type]();
		}
		return NULL;
	}

	static void Bind(const string &type, GenerateScriptFunc func) {
		if (scriptFunc.find(type) == scriptFunc.end()) {
			scriptFunc[type] = func;
		}
		else {
			LogError("Script type has not been registered.");
		}
	}
};