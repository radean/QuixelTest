// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Toolkits/BaseToolkit.h"

class FCrableEdModeToolkit : public FModeToolkit
{
public:

	FCrableEdModeToolkit();

	
	
	/** FModeToolkit interface */
	virtual void Init(const TSharedPtr<IToolkitHost>& InitToolkitHost) override;

	/** IToolkit interface */
	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual class FEdMode* GetEditorMode() const override;
	void OnRenameFieldUpdate(const FText& InText, ETextCommit::Type InCommitType, int RenameIndex);
	void OnIncrementalBool(ECheckBoxState InState, int CheckIndex);
	virtual TSharedPtr<class SWidget> GetInlineContent() const override { return ToolkitWidget; }
	


	static FString RenameText;

private:

	TSharedPtr<SWidget> ToolkitWidget;
};
