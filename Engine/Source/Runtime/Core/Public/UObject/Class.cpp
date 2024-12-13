#include "Class.h"

map<FString, UClass*> ClassMap;

UClass::UClass(FString InClassName, const type_info& InClassTypeInfo, const uint64 InClassSize, ClassConstructorType InClassConstructorType, StaticClassFunctionType InSuperClassFunction)
	:SuperClass(nullptr)
	, ClassName(InClassName)
	, ClassTypeInfo(InClassTypeInfo)
	, ClassSize(InClassSize)
	, ClassConstructor(InClassConstructorType)
{
	if (InSuperClassFunction)
	{
		SuperClass = InSuperClassFunction();
	}
}
UClass* GetPrivateStaticClassBody(
	FString InClassName, UClass::ClassConstructorType InClassConstructor, UClass::StaticClassFunctionType InSuperClassFn, const type_info& InClassTypeInfo, const uint64 InClassSize)
{
	UClass* ReturnClass = ::new UClass
	(
		InClassName, InClassTypeInfo, InClassSize, InClassConstructor, InSuperClassFn
	);

	ClassMap.emplace(InClassName, ReturnClass);

	return ReturnClass;
}