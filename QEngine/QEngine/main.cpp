#include "gl_engine_shell.h"
#include "MyWorld.h"
using namespace std;
using namespace QEngine;

EngineShellPointer gEngine;
BaseWorldPointer myWorld;

int main() {
	myWorld = make_shared<MyWorld>();
	gEngine = EngineShell::Instance();
	gEngine->LaunchQEngine();
	Context::SetWorldAndInit(myWorld, "world_config.json");
	gEngine->Run();
	return 0;
}