#include "gl_engine_shell.h"
#include "MyWorld.h"
#include "Rotator.h"
#include "DefferRenderer.h"
using namespace std;
using namespace QEngine;

EngineShellPointer gEngine;
BaseWorldPointer myWorld;
BaseRendererPointer myRenderer;

void SetupScripts(){
	BaseScriptPointer rotator = make_shared<Rotator>();
	myWorld->BindScriptTo(rotator, "cubes");
}

int main() {
	gEngine = EngineShell::Instance();
	gEngine->LaunchQEngine();
	myWorld = make_shared<MyWorld>();
	myRenderer = make_shared<DefferRenderer>();
	Context::SetWorldAndInit(myWorld, "world_config.json");
	Context::SetRenderer(myRenderer);
	//SetupScripts();
	gEngine->Run();
	return 0;
}