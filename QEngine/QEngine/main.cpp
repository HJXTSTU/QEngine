#include "gl_engine_shell.h"
#include "MyWorld.h"
#include "Rotator.h"
using namespace std;
using namespace QEngine;

EngineShellPointer gEngine;
BaseWorldPointer myWorld;

void SetupScripts(){
	BaseScriptPointer rotator = make_shared<Rotator>();
	myWorld->BindScriptTo(rotator, "bunny");
}

int main() {
	gEngine = EngineShell::Instance();
	gEngine->LaunchQEngine();
	myWorld = make_shared<MyWorld>();
	Context::SetWorldAndInit(myWorld, "world_config.json");
	SetupScripts();
	gEngine->Run();
	return 0;
}