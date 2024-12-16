#pragma once
#include "CoreTypes.h"

class UClass;
class UObject;
class CORE_API UObjectBase
{
public:
	UObjectBase();
	UObjectBase(EObjectFlags InObjectFlags, UClass* InClass, UObject* InOuter);
	virtual ~UObjectBase();

	EObjectFlags GetFlags() { return ObjectFlags; }
	UClass* GetClass() { return ClassPrivate; }
	UObject* GetOuter() { return OuterPrivate; }

private:
	EObjectFlags ObjectFlags;
	UClass* ClassPrivate;
	UObject* OuterPrivate;
};