// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "CrableEdMode.h"
#include "CrableEdModeToolkit.h"
#include "Toolkits/ToolkitManager.h"
#include "EditorModeManager.h"

const FEditorModeID FCrableEdMode::EM_CrableEdModeId = TEXT("EM_CrableEdMode");

FCrableEdMode::FCrableEdMode()
{

}

FCrableEdMode::~FCrableEdMode()
{

}

void FCrableEdMode::Enter()
{
	FEdMode::Enter();

	if (!Toolkit.IsValid() && UsesToolkits())
	{
		Toolkit = MakeShareable(new FCrableEdModeToolkit);
		Toolkit->Init(Owner->GetToolkitHost());
	}
}

void FCrableEdMode::Exit()
{
	if (Toolkit.IsValid())
	{
		FToolkitManager::Get().CloseToolkit(Toolkit.ToSharedRef());
		Toolkit.Reset();
	}

	// Call base Exit method to ensure proper cleanup
	FEdMode::Exit();
}

bool FCrableEdMode::UsesToolkits() const
{
	return true;
}




