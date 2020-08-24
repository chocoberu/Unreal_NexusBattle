// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "NexusBattle/NBPlayerController.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeNBPlayerController() {}
// Cross Module References
	NEXUSBATTLE_API UClass* Z_Construct_UClass_ANBPlayerController_NoRegister();
	NEXUSBATTLE_API UClass* Z_Construct_UClass_ANBPlayerController();
	ENGINE_API UClass* Z_Construct_UClass_APlayerController();
	UPackage* Z_Construct_UPackage__Script_NexusBattle();
// End Cross Module References
	void ANBPlayerController::StaticRegisterNativesANBPlayerController()
	{
	}
	UClass* Z_Construct_UClass_ANBPlayerController_NoRegister()
	{
		return ANBPlayerController::StaticClass();
	}
	struct Z_Construct_UClass_ANBPlayerController_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UE4CodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_ANBPlayerController_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_APlayerController,
		(UObject* (*)())Z_Construct_UPackage__Script_NexusBattle,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ANBPlayerController_Statics::Class_MetaDataParams[] = {
		{ "Comment", "/**\n * \n */" },
		{ "HideCategories", "Collision Rendering Utilities|Transformation" },
		{ "IncludePath", "NBPlayerController.h" },
		{ "ModuleRelativePath", "NBPlayerController.h" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_ANBPlayerController_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<ANBPlayerController>::IsAbstract,
	};
	const UE4CodeGen_Private::FClassParams Z_Construct_UClass_ANBPlayerController_Statics::ClassParams = {
		&ANBPlayerController::StaticClass,
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
		0x009002A4u,
		METADATA_PARAMS(Z_Construct_UClass_ANBPlayerController_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_ANBPlayerController_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_ANBPlayerController()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UE4CodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_ANBPlayerController_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(ANBPlayerController, 1504254518);
	template<> NEXUSBATTLE_API UClass* StaticClass<ANBPlayerController>()
	{
		return ANBPlayerController::StaticClass();
	}
	static FCompiledInDefer Z_CompiledInDefer_UClass_ANBPlayerController(Z_Construct_UClass_ANBPlayerController, &ANBPlayerController::StaticClass, TEXT("/Script/NexusBattle"), TEXT("ANBPlayerController"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(ANBPlayerController);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
