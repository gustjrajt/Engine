#include "UObject/NameTypes.h"
#include "UObject/UObjectGlobals.h"
#include "CoreTypes.h"
//#include "Logging/Logger.h"

unordered_map<uint64, FString>& GetNamePool()
{
	static unordered_map<uint64, FString> NamePool;
	return NamePool;
}



FName::FName(FStringView InString)
{
	FString String = InString.data();
	HashCode = Hash(String.data());
	GetNamePool()[HashCode] = String;
}

FName::FName(const WIDECHAR* InString)
	:FName(FStringView(InString))
{
}

FString FName::ToString() const
{
	FString Out;
	ToString(Out);
	return Out;
}

void FName::ToString(FString& Out) const
{
	if (HashCode == 0)
	{
		//E_LOG(Error, TEXT("HashCode == 0"));
		return;
	}

	Out = GetNamePool()[HashCode];
}
