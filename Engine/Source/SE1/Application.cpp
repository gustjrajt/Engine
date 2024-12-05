#include "CoreTypes.h"
#include "Modules/ModuleManager.h"
#include <iostream>

IMPLEMENT_MODULE(FDefaultModuleImpl, SE1);

int main()
{
	std::cout << "Hello World!" << std::endl;
	return 0;
}