#pragma once
#include "Object.h"
#include "UObject/UObjectGlobals.h"

/**
 * An object class.
 */
class CORE_API UClass : public UObject
{
private:
	UClass* SuperClass;

public:
	FString ClassName;
	const type_info& ClassTypeInfo;
	const uint64 ClassSize;

public:
	using ClassConstructorType = function<void(const FObjectInitializer&)>;
	ClassConstructorType ClassConstructor;

	// 이 Class의 부모 Class를 얻어오는 함수
	using StaticClassFunctionType = function<UClass* ()>;

	UClass() = delete;
	UClass(FString InClassName, const type_info& InClassTypeInfo,
		const uint64 InClassSize, ClassConstructorType InClassConstructorType,
		StaticClassFunctionType InSuperClassFunction);
	
	virtual ~UClass() {}
};