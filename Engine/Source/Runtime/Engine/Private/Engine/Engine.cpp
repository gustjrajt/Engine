#include "Engine/Engine.h"

IMPLEMENT_MODULE(FDefaultModuleImpl, Engine);

UEngine::UEngine()
{
	E_LOG(Fatal, TEXT("{} Hello"), 10);
}