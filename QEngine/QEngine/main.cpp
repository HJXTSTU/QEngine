#include "gl_engine_shell.h"
#include "MyWorld.h"
#include "Rotator.h"
#include "ForwardBaseRenderer.h"
using namespace std;
using namespace QEngine;

EngineShellPointer gEngine;
BaseWorldPointer myWorld;
BaseRendererPointer myRenderer;

void SetupScripts(){
	BaseScriptPointer rotator = make_shared<Rotator>();
	myWorld->BindScriptTo(rotator, "bunny");
}

int main() {
	gEngine = EngineShell::Instance();
	gEngine->LaunchQEngine();
	myWorld = make_shared<MyWorld>();
	myRenderer = make_shared<ForwardBaseRenderer>();
	Context::SetWorldAndInit(myWorld, "world_config.json");
	Context::SetRenderer(myRenderer);
	SetupScripts();
	gEngine->Run();
	return 0;
}