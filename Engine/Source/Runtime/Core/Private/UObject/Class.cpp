#include "UObject/Class.h"
#include "UObject/UObjectArray.h"

CORE_API map<FString, UClass*>& GetClassMap()
{
	static map<FString, UClass*> ClassMap;
	return ClassMap;
}

UClass::UClass(FString InClassName, const type_info& InClassTypeInfo, const uint64 InClassSize,
	ClassConstructorType InClassConstructorType, StaticClassFunctionType InSuperClassFunction)
	: SuperClass(nullptr)
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
UObject* UClass::CreateDefaultObject()
{
	_ASSERT(!ClassDefaultObject);

	FStaticConstructObjectParameters StaticConstructObjectParameters(this);
	StaticConstructObjectParameters.SetFlags = EObjectFlags::RF_ClassDefaultObject;
	StaticConstructObjectParameters.Name = FName(ClassName);
	ClassConstructor(FObjectInitializer(ClassDefaultObject, StaticConstructObjectParameters));

	return ClassDefaultObject.get();
}

void UClass::InternalCreateDefaultObjectWrapper() const
{
	const_cast<UClass*>(this)->CreateDefaultObject();
}

UClass* GetPrivateStaticClassBody(FString InClassName,
	UClass::ClassConstructorType InClassConstructor,
	UClass::StaticClassFunctionType InSuperClassFn,
	const type_info& InClassTypeInfo, const uint64 InClassSize)
{
	// UClass 메모리 Pool 생성, 이미 존재한다면 건너뜀 (undorered_map 특성)
	GUObjectArray.Create(typeid(UClass), sizeof(UClass));


	/*
	마찬가지로 InClassTypeInfo 메모리 Pool생성(Shared_ptr메모리 크기를 합한값)
	*/


	// __Ref_count_obj_alloc3<_Ty, _Alloc> shared_ptr과 _Ty의 메모리 크기를 합한값
	const uint64 SharedPtrSize = sizeof(_Ref_count_obj_alloc3<UObject, FAllocator<UObject>>) - sizeof(UObject);
	GUObjectArray.Create(InClassTypeInfo, SharedPtrSize + InClassSize);

	// 생선된 UClass Pool에 실질적인 메모리 할당 및 UClass 인스턴스 생성
	UClass* ReturnClass = (UClass*)GUObjectArray.Malloc(typeid(UClass));
	ReturnClass = ::new (ReturnClass)
		UClass
		(
			InClassName,
			InClassTypeInfo,
			InClassSize,
			InClassConstructor,
			InSuperClassFn
		);
	//생성된 UClass정보를 Map에 저장
	GetClassMap().emplace(InClassName, ReturnClass);

	return ReturnClass;
}
