#include "gl_engine_shell.h"
using namespace std;
using namespace QEngine;

EngineShellPointer gEngine;

int main() {
	gEngine = EngineShell::Instance();
	gEngine->LaunchQEngine();
	gEngine->Run();
	return 0;
}