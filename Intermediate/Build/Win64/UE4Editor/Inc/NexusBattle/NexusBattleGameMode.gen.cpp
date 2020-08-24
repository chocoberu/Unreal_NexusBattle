// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "NexusBattle/NexusBattleGameMode.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeNexusBattleGameMode() {}
// Cross Module References
	NEXUSBATTLE_API UClass* Z_Construct_UClass_ANexusBattleGameMode_NoRegister();
	NEXUSBATTLE_API UClass* Z_Construct_UClass_ANexusBattleGameMode();
	ENGINE_API UClass* Z_Construct_UClass_AGameModeBase();
	UPackage* Z_Construct_UPackage__Script_NexusBattle();
// End Cross Module References
	void ANexusBattleGameMode::StaticRegisterNativesANexusBattleGameMode()
	{
	}
	UClass* Z_Construct_UClass_ANexusBattleGameMode_NoRegister()
	{
		return ANexusBattleGameMode::StaticClass();
	}
	struct Z_Construct_UClass_ANexusBattleGameMode_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UE4CodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_ANexusBattleGameMode_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_AGameModeBase,
		(UObject* (*)())Z_Construct_UPackage__Script_NexusBattle,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ANexusBattleGameMode_Statics::Class_MetaDataParams[] = {
		{ "HideCategories", "Info Rendering MovementReplication Replication Actor Input Movement Collision Rendering Utilities|Transformation" },
		{ "IncludePath", "NexusBattleGameMode.h" },
		{ "ModuleRelativePath", "NexusBattleGameMode.h" },
		{ "ShowCategories", "Input|MouseInput Input|TouchInput" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_ANexusBattleGameMode_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<ANexusBattleGameMode>::IsAbstract,
	};
	const UE4CodeGen_Private::FClassParams Z_Construct_UClass_ANexusBattleGameMode_Statics::ClassParams = {
		&ANexusBattleGameMode::StaticClass,
		"Game",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		nullptr,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		0,
		0,
		0x008802ACu,
		METADATA_PARAMS(Z_Construct_UClass_ANexusBattleGameMode_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_ANexusBattleGameMode_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_ANexusBattleGameMode()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UE4CodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_ANexusBattleGameMode_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(ANexusBattleGameMode, 122219617);
	template<> NEXUSBATTLE_API UClass* StaticClass<ANexusBattleGameMode>()
	{
		return ANexusBattleGameMode::StaticClass();
	}
	static FCompiledInDefer Z_CompiledInDefer_UClass_ANexusBattleGameMode(Z_Construct_UClass_ANexusBattleGameMode, &ANexusBattleGameMode::StaticClass, TEXT("/Script/NexusBattle"), TEXT("ANexusBattleGameMode"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(ANexusBattleGameMode);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
