// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "Crable.h"
#include "CrableEdMode.h"

#define LOCTEXT_NAMESPACE "FCrableModule"

void FCrableModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	FEditorModeRegistry::Get().RegisterMode<FCrableEdMode>(FCrableEdMode::EM_CrableEdModeId, LOCTEXT("CrableEdModeName", "Crable"), FSlateIcon(), true);
	//UE_LOG(FCrableModule, Warning, TEXT("ADDED TEXTION"))
}

void FCrableModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FEditorModeRegistry::Get().UnregisterMode(FCrableEdMode::EM_CrableEdModeId);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FCrableModule, Crable)