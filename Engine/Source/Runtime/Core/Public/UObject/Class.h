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

CORE_API UClass* GetPrivateStaticClassBody(
	FString InClassName,
	UClass::ClassConstructorType InClassConstructor,
	UClass::StaticClassFunctionType InSuperClassFn,
	//function<void()> InClassReflection,
	const type_info& InClassTypeInfo,
	const uint64 InClassSize
);

template<class T>
UClass* TGetPrivateStaticClassBody(
	FString InClassName,
	UClass::ClassConstructorType InClassConstructor,
	UClass::StaticClassFunctionType InSuperClassFn)
{
	return GetPrivateStaticClassBody(InClassName, InClassConstructor, InSuperClassFn,
		typeid(T), sizeof(T));
}

/**
 * 클래스의 기본 생성자를 호출하기 위한 헬퍼 템플릿입니다.
 */
template<class T>
void InternalConstructor(const FObjectInitializer& X)
{
	FObjectInitializer* ObjectInitializer = const_cast<FObjectInitializer*>(&X);
	ObjectInitializer->SharedObj = allocate_shared<T>(FAllocator<T>(ObjectInitializer));
}