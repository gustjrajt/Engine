#include "Launch.h"
#include "LaunchEngineLoop.h"

IMPLEMENT_MODULE(FDefaultModuleImpl, Launch);

FEngineLoop GEngineLoop;

/**
 * PreInits the engine loop
 */
int32 LAUNCH_API EnginePreInit(const TCHAR* CmdLine)
{
	int32 ErrorLevel = GEngineLoop.PreInit(CmdLine);

	return ErrorLevel;
}

 /**
  * Inits the engine loop
  */
int32 LAUNCH_API EngineInit()
{
	int32 ErrorLevel = GEngineLoop.Init();

	return ErrorLevel;
}

  /**
   * Ticks the engine loop
   */
void LAUNCH_API EngineTick()
{
	GEngineLoop.Tick();
}

int32 LAUNCH_API GuardedMain(const TCHAR* CmdLine)
{
	int32 ErrorLevel = EnginePreInit(CmdLine);
	if (ErrorLevel != 0) { return ErrorLevel; }

	ErrorLevel = EngineInit();
	if (ErrorLevel != 0) { return ErrorLevel; }

	while (!IsEngineExitRequested())
	{
		EngineTick();
	}

	return ErrorLevel;
}