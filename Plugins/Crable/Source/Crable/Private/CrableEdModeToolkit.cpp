// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "CrableEdModeToolkit.h"
#include "CrableEdMode.h"
// IN-GAME SELECTION
#include "Engine/Selection.h"

// ENGINE ACTORS
#include "Engine/StaticMeshActor.h"
#include "Engine/DecalActor.h"
#include "Sound/AmbientSound.h"

// UTILITY LIB
#include "Editor/ContentBrowser/Public/ContentBrowserModule.h"
#include "IContentBrowserSingleton.h"
//#include "Editor/ContentBrowser/Private/SContentBrowser.h"
#include "Runtime/AssetRegistry/Public/AssetRegistryModule.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/MetaData.h"
#include "Runtime/Core/Public/Containers/Map.h"

// WIDGET 
#include "Widgets/Input/SButton.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SCheckBox.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "EditorModeManager.h"


#define LOCTEXT_NAMESPACE "FCrableEdModeToolkit"

FCrableEdModeToolkit::FCrableEdModeToolkit()
{
	
}

// Renaming Variable Space
FString StaticMeshRename;
FString DecalRename;
FString AmbientSoundRename;

//CheckBoxes Variable Space
bool IsPrefix;
bool IsIncremental;


//TextField Capture Function
void FCrableEdModeToolkit::OnRenameFieldUpdate(const FText& InText, ETextCommit::Type InCommitType, int RenameIndex)
{

	// Setting IndexWise
	switch (RenameIndex)
	{
		case 0:
			StaticMeshRename = InText.ToString();
			break;

		case 1:
			DecalRename = InText.ToString();
			break;

		case 2:
			AmbientSoundRename = InText.ToString();
			break;
	}
}

//Set IsIncremental Variable  
void FCrableEdModeToolkit::OnIncrementalBool(ECheckBoxState InState, int CheckIndex)
{
	ECheckBoxState Checker = InState;

	switch (CheckIndex)
	{
		// Case 0 is incremental Option
	case 0:
		if (UINT16(Checker) == 1)
		{
			IsIncremental = true;
		}
		else if (UINT16(Checker) == 0) {
			IsIncremental = false;
		}
		break;
		// Case 1 is for Prefix option
	case 1:
		if (UINT16(Checker) == 1)
		{
			IsPrefix = true;
		}
		else if (UINT16(Checker) == 0) {
			IsPrefix = false;
		}
		break;
		break;
	}

	
}


// Getting Rename Text Variable
//void GetRenameText(const FText& InText, ETextCommit::Type InCommitType)
//{
//	FCrableEdModeToolkit::OnButtonClickGetLevelActor(InText);
//}

void FCrableEdModeToolkit::Init(const TSharedPtr<IToolkitHost>& InitToolkitHost)
{
	struct Locals
	{
		static bool IsWidgetEnabled()
		{
			
			return GEditor->GetSelectedActors()->Num() != 0;
		}

		
		/// <summary>
		/// LOD Generation from Content Browser Selection
		/// </summary>
		/// <param name="InitToolkitHost">LODGeneration</param>
		static FReply OnGenerateLOD()
		{
			
			FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");

			// Getting the Selected Objects
			TArray<FAssetData> SelectedAssets;
			ContentBrowserModule.Get().GetSelectedAssets(SelectedAssets);
			int index = 0;

			for (FAssetData& AssetData : SelectedAssets)
			{
				//Add index value
				index++;

				//UMetaData* MetaDataAsset = AssetData.GetPackage()->GetMetaData()->GetMapForObject();
				UObject* AssetObject = AssetData.GetAsset();

				FString AssetName = AssetObject->GetFName().ToString();
				FString AssetIndex = AssetObject->GetPathName();
				UE_LOG(LogTemp, Warning, TEXT("LOD - > Asset Name, %s"), *AssetName);
				UE_LOG(LogTemp, Warning, TEXT("LOD - > Path, %s"), *AssetIndex);
				UE_LOG(LogTemp, Warning, TEXT("LOD - > Index, %d"), index);
				
			}
			index = 0;
			
			return FReply::Handled();
		}


		/// <summary>
		/// Sync the assets from the content browser
		/// </summary>
		/// <param name="InitToolkitHost">Sync Content Browser</param>
		static FReply OnSyncStaticMeshes()
		{
			TArray<AActor*> LevelActors;

			//////////////////////////////////////
			// Loading the Content Browser Construct
			FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");

			////////////////////////////////////////////////////
			// Geting all the static mesh actors in the current level
			UGameplayStatics::GetAllActorsOfClass(GEditor->GetEditorWorldContext().World(), AStaticMeshActor::StaticClass(), LevelActors);

			// Getting the Selected Objects
			TArray<FAssetData> SelectedAssets;
			ContentBrowserModule.Get().GetSelectedAssets(SelectedAssets);


			// Begining the transaction
			GEditor->BeginTransaction(LOCTEXT("RenameActorsTransactionName", "Rename Actor"));



			// Iterating inside the level actors array
			for (int32 LevelActorIndex = 0; LevelActorIndex < LevelActors.Num(); LevelActorIndex++)
			{
				AActor* Actor = LevelActors[LevelActorIndex];
				FString LabelTitle = Actor->GetActorLabel();
				FString ActorPackagePath = Actor->GetPathName();
				UE_LOG(LogTemp, Warning, TEXT("SYNC - > Asset Name, %s"), *LabelTitle);
				UE_LOG(LogTemp, Warning, TEXT("SYNC - > Asset PackagePath, %s"), *ActorPackagePath);


				//Spliiter Context
				FString Splitter = ".";
				//TArray<FString> leftSplit;
				FString leftSplit, rightSplit;
				//Clipping the Package path name,
				ActorPackagePath.Split(TEXT("."), &leftSplit, &rightSplit, ESearchCase::IgnoreCase, ESearchDir::FromEnd);
				UE_LOG(LogTemp, Warning, TEXT("SYNC - > Trimmed Right, %s"), *rightSplit);
				UE_LOG(LogTemp, Warning, TEXT("SYNC - > Trimmed Left, %s"), *leftSplit);

				//Checking the asset Name and replacing it if it is not matching with the scene clip
				if (LabelTitle != rightSplit)
				{
					Actor->SetActorLabel(rightSplit);
				}
			}

			int index = 0;

			// For Content Browser
			for (FAssetData& AssetData : SelectedAssets)
			{
				//Add index value
				index++;

				//UMetaData* MetaDataAsset = AssetData.GetPackage()->GetMetaData()->GetMapForObject();
				UObject* AssetObject = AssetData.GetAsset();

				FString AssetName = AssetObject->GetFName().ToString();
				FString AssetIndex = AssetObject->GetPathName();
				UE_LOG(LogTemp, Warning, TEXT("SYNC - > Asset Name, %s"), *AssetName);
				UE_LOG(LogTemp, Warning, TEXT("SYNC - > Path, %s"), *AssetIndex);
				UE_LOG(LogTemp, Warning, TEXT("SYNC - > Index, %d"), index);

			}
			index = 0;

			// We're done renaming the actors so close transaction
			GEditor->EndTransaction();

			return FReply::Handled();
		}




		/// <summary>
		/// Renaming Facity for Selection based Rename from incremental or prefix values.
		/// </summary>
		/// <param name="InitToolkitHost">Renamer</param>
		static FReply OnButtonClickGetLevelActor()
		{
			TArray<AActor*> LevelActors;
			TArray<AActor*> LevelDecalActors;
			TArray<AActor*> LevelAmbientSoundActors;
			//UWorld* World = World->GetWorld();

			// Getting the Selected Actors from the Scene
			USelection* SelectedActors = GEditor->GetSelectedActors();
			
			// For each selected actor
			for (FSelectionIterator Iter(*SelectedActors); Iter; ++Iter)
			{
				if (AActor* LevelActor = Cast<AActor>(*Iter))
				{
					// Adding switch to append relavant Actors in the ActorArray,,
					auto ClassSpecifier = LevelActor->GetClass();
					if (ClassSpecifier == AStaticMeshActor::StaticClass())
					{
						LevelActors.Add(LevelActor);
					}else
					if (ClassSpecifier == ADecalActor::StaticClass())
					{
						LevelDecalActors.Add(LevelActor);
					}else
					if (ClassSpecifier == AAmbientSound::StaticClass())
					{
						LevelAmbientSoundActors.Add(LevelActor);
					}
					
				}
			}

			FString StaticMeshRenamedText = StaticMeshRename;
			FString DecalActorRenamedText = DecalRename;

			/////////////////////
			//Loop throughout the level to find all the Static Mesh Actors, Decal, Ambient Sound

			/*UGameplayStatics::GetAllActorsOfClass(GEditor->GetEditorWorldContext().World(), AStaticMeshActor::StaticClass(), LevelActors);

			UGameplayStatics::GetAllActorsOfClass(GEditor->GetEditorWorldContext().World(), ADecalActor::StaticClass(), LevelDecalActors);
			
			UGameplayStatics::GetAllActorsOfClass(GEditor->GetEditorWorldContext().World(), AAmbientSound::StaticClass(), LevelAmbientSoundActors);*/
			
			//Iterating each Static Mesh Actor
			//for (LevelActors)
			//{
				//break;
			//}

			// Begining the transaction
			GEditor->BeginTransaction(LOCTEXT("RenameActorsTransactionName", "Rename Actor"));

			//IF Incremental 
			//STATIC MESH
			if (IsPrefix && IsIncremental == false)
			{
				for (int32 LevelActorIndex = 0; LevelActorIndex < LevelActors.Num(); LevelActorIndex++)
				{
					AActor* Actor = LevelActors[LevelActorIndex];
					FString LabelTitle = Actor->GetActorLabel();
					FString Label = StaticMeshRenamedText + "_" + LabelTitle;
					Actor->SetActorLabel(Label);
				}
			}
			else if (IsIncremental)
			{
				for (int32 LevelActorIndex = 0; LevelActorIndex < LevelActors.Num(); LevelActorIndex++)
				{
					AActor* Actor = LevelActors[LevelActorIndex];
					FString Label = StaticMeshRenamedText + FString::FromInt(LevelActorIndex);
					Actor->SetActorLabel(Label);
				}
			}
			else
			{
				for (AActor* Actor : LevelActors)
				{
					//FString name= Actor->GetName();
					//UE_LOG(LogTemp, Warning, TEXT("Clipped, %s"), *name);
					Actor->SetActorLabel(StaticMeshRenamedText);
				}
			}


			//DECAL ACTOR
			if (IsPrefix && IsIncremental == false)
			{
				for (int32 LevelActorIndex = 0; LevelActorIndex < LevelDecalActors.Num(); LevelActorIndex++)
				{
					AActor* Actor = LevelDecalActors[LevelActorIndex];
					FString LabelTitle = Actor->GetActorLabel();
					FString Label = DecalActorRenamedText + "_" + LabelTitle;
					Actor->SetActorLabel(Label);
				}
			}else if (IsIncremental)
			{
				for (int32 LevelActorIndex = 0; LevelActorIndex < LevelDecalActors.Num(); LevelActorIndex++)
				{
					AActor* Actor = LevelDecalActors[LevelActorIndex];
					FString Label = DecalActorRenamedText + FString::FromInt(LevelActorIndex);
					Actor->SetActorLabel(Label);
				}
			}
			else if (IsIncremental == false && IsPrefix == false)
			{
				for (AActor* Actor : LevelDecalActors)
				{
					//FString name= Actor->GetName();
					//UE_LOG(LogTemp, Warning, TEXT("Clipped, %s"), *name);
					Actor->SetActorLabel(DecalActorRenamedText);
				}
			}


			//AMBIENT SOUND
			if (IsPrefix && IsIncremental == false)
			{
				for (int32 LevelActorIndex = 0; LevelActorIndex < LevelAmbientSoundActors.Num(); LevelActorIndex++)
				{
					AActor* Actor = LevelAmbientSoundActors[LevelActorIndex];
					FString LabelTitle = Actor->GetActorLabel();
					FString Label = AmbientSoundRename + "_" + LabelTitle;
					Actor->SetActorLabel(Label);
					FString LogicalName = Actor->GetFName().ToString();
					//FString LogicalName = Actor->GetFName().ToString();
					UE_LOG(LogTemp, Warning, TEXT("LOGICAL NAME, %s"), *LogicalName);
				}
			}
			else if (IsIncremental)
			{
				for (int32 LevelActorIndex = 0; LevelActorIndex < LevelAmbientSoundActors.Num(); LevelActorIndex++)
				{
					AActor* Actor = LevelAmbientSoundActors[LevelActorIndex];
					FString Label = AmbientSoundRename + FString::FromInt(LevelActorIndex);
					Actor->SetActorLabel(Label);
				}
			}
			else
			{
				for (AActor* Actor : LevelAmbientSoundActors)
				{
					//FString name= Actor->GetName();
					//UE_LOG(LogTemp, Warning, TEXT("Clipped, %s"), *name);
					Actor->SetActorLabel(AmbientSoundRename);
				}
			}
			
			// We're done renaming the actors so close transaction
			GEditor->EndTransaction();


			UE_LOG(LogTemp, Warning, TEXT("Clipped, %d"), LevelActors.Num());
			return FReply::Handled();
		}

		static FReply OnButtonClick(FVector InOffset)
		{
			USelection* SelectedActors = GEditor->GetSelectedActors();

			//int ftextion = GEditor->GetSelectedActorCount();
			//UE_LOG(LogTemp, Warning, TEXT("Some warning message %d"), ftextion);

			// Let editor know that we're about to do something that we want to undo/redo
			GEditor->BeginTransaction(LOCTEXT("MoveActorsTransactionName", "MoveActors"));


			// For each selected actor
			for (FSelectionIterator Iter(*SelectedActors); Iter; ++Iter)
			{
				if (AActor* LevelActor = Cast<AActor>(*Iter))
				{
					// Register actor in opened transaction (undo/redo)
					LevelActor->Modify();
					// Move actor to given location
					LevelActor->TeleportTo(LevelActor->GetActorLocation() + InOffset, FRotator(0, 0, 0));
				}
			}

			// We're done moving actors so close transaction
			GEditor->EndTransaction();
			

			return FReply::Handled();
		}

		static TSharedRef<SWidget> MakeButton(FText InLabel, const FVector InOffset)
		{
			return SNew(SButton)
				.Text(InLabel)
				.OnClicked_Static(&Locals::OnButtonClick, InOffset);
		}

		
	};

	const float Factor = 256.0f;

	SAssignNew(ToolkitWidget, SBorder)
		.HAlign(HAlign_Center)
		.Padding(5)
		.IsEnabled_Static(&Locals::IsWidgetEnabled)
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.AutoHeight()
			.HAlign(HAlign_Center)
			.Padding(10)
			[
				SNew(STextBlock)
				.AutoWrapText(true)
				.Text(LOCTEXT("HelperLabel", "Please type the Naming Convention of the Following Asset type."))
			]
			/*+ SVerticalBox::Slot()
				.HAlign(HAlign_Center)
				.AutoHeight()
				[
					Locals::MakeButton(LOCTEXT("UpButtonLabel", "Up"), FVector(0, 0, Factor))
				]
			+ SVerticalBox::Slot()
				.HAlign(HAlign_Center)
				.AutoHeight()
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.AutoWidth()
					[
						Locals::MakeButton(LOCTEXT("LeftButtonLabel", "Left"), FVector(0, -Factor, 0))
					]
					+ SHorizontalBox::Slot()
						.AutoWidth()
						[
							Locals::MakeButton(LOCTEXT("RightButtonLabel", "Right"), FVector(0, Factor, 0))
						]
				]
			+ SVerticalBox::Slot()
				.HAlign(HAlign_Center)
				.AutoHeight()
				[
					Locals::MakeButton(LOCTEXT("DownButtonLabel", "Down"), FVector(0, 0, -Factor))
				]*/

			// Naming Convention for Three assets type


			///////////////
			// STATIC MESH
			+ SVerticalBox::Slot()
				.Padding(5)
				.AutoHeight()
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.HAlign(HAlign_Left)
					.AutoWidth()
					[
						SNew(STextBlock)
						.AutoWrapText(true)
						.MinDesiredWidth(120.0)
						.Text(LOCTEXT("HelperLabel1", "Static Mesh"))
					]
					+ SHorizontalBox::Slot()
					.HAlign(HAlign_Right)
					.AutoWidth()
					[
						SNew(SEditableTextBox)
						.MinDesiredWidth(70.0)
						.Text(FText(LOCTEXT("TextLabel1", "")))
						.OnTextCommitted(this, &FCrableEdModeToolkit::OnRenameFieldUpdate, 0)
					]
				]



			////////////
			// DECALS
			+ SVerticalBox::Slot()
				.Padding(5)
				.AutoHeight()
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.AutoWidth().HAlign(HAlign_Left)
					[
						SNew(STextBlock)
						.AutoWrapText(true)
						.MinDesiredWidth(120.0)
						.Text(LOCTEXT("DecalLabel", "Decals"))
					]
					+ SHorizontalBox::Slot()
					.AutoWidth().HAlign(HAlign_Right)
					[
						SNew(SEditableTextBox)
						.MinDesiredWidth(70.0)
						.Text(FText(LOCTEXT("DecalTextLabel", "")))
						.OnTextCommitted(this, &FCrableEdModeToolkit::OnRenameFieldUpdate, 1)
					]
				]

			// DECALS
			+ SVerticalBox::Slot()
				.Padding(5)
				.AutoHeight()
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.AutoWidth().HAlign(HAlign_Left)
					[
						SNew(STextBlock)
						.AutoWrapText(true)
						.MinDesiredWidth(120.0)
						.Text(LOCTEXT("AmbientLabel", "Ambient Sound"))
					]
					+ SHorizontalBox::Slot()
					.AutoWidth().HAlign(HAlign_Right)
					[
						SNew(SEditableTextBox)
						.MinDesiredWidth(70.0)
						.Text(FText(LOCTEXT("AmbientSoundLabel", "")))
						.OnTextCommitted(this, &FCrableEdModeToolkit::OnRenameFieldUpdate, 2)
					]
			]


			/////////
			// SUBMITTIONS
			+SVerticalBox::Slot()
				.Padding(5)
				.AutoHeight()
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.AutoWidth().HAlign(HAlign_Left)
				[
					SNew(SButton)
					.HAlign(HAlign_Center)
					.Text(LOCTEXT("SubmitLabel", "Rename !"))
					.OnClicked_Static(&Locals::OnButtonClickGetLevelActor)
				]
			]




			// More Options Tag Line
			+ SVerticalBox::Slot()
				.Padding(10)
				.HAlign(HAlign_Center)
				.AutoHeight()
				[
					SNew(STextBlock)
					.AutoWrapText(true)
					.MinDesiredWidth(100.0)
					.Text(LOCTEXT("MoreOptionsLabels", "Advance Options"))
				]




			// More Options
			+ SVerticalBox::Slot()
				.Padding(5)
				.AutoHeight()
				[
					
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.HAlign(HAlign_Left)
					.AutoWidth()
					[
						SNew(STextBlock)
						.AutoWrapText(true)
						.MinDesiredWidth(128.0)
						.Text(LOCTEXT("IncrementalLabel", "++ Naming"))
					]
					+ SHorizontalBox::Slot()
						.HAlign(HAlign_Right)
						.AutoWidth()
					[
						SNew(SCheckBox)
						.IsChecked(ECheckBoxState::Unchecked)
						.OnCheckStateChanged(this, &FCrableEdModeToolkit::OnIncrementalBool, 0)
					]
					+ SHorizontalBox::Slot()
						.HAlign(HAlign_Left)
						.AutoWidth()
					[
						SNew(STextBlock)
						.AutoWrapText(true)
						.MinDesiredWidth(128.0)
						.Text(LOCTEXT("PrefixLabel", "Prefix"))
					]
					+ SHorizontalBox::Slot()
						.HAlign(HAlign_Right)
						.AutoWidth()
					[
						SNew(SCheckBox)
						.IsChecked(ECheckBoxState::Unchecked)
						.OnCheckStateChanged(this, &FCrableEdModeToolkit::OnIncrementalBool, 1)
					]

				]

			//////////////
			// LOD GENERATION PANEL
			+SVerticalBox::Slot()
				.Padding(25)
				.HAlign(HAlign_Center)
				.AutoHeight()
				[
					SNew(SButton)
					.HAlign(HAlign_Center)
					.Text(LOCTEXT("LODGenerationButtonLabel", "Generate LOD!"))
					.OnClicked_Static(&Locals::OnGenerateLOD)
				]



			// Sync Assets tagline
			+SVerticalBox::Slot()
				.Padding(10)
				.HAlign(HAlign_Center)
				.AutoHeight()
				[
					SNew(STextBlock)
					.AutoWrapText(true)
				.MinDesiredWidth(100.0)
				.Text(LOCTEXT("SMDetectLabels", "Please click the button below to sync the assets from it's package path."))
				]




			//////////////
			// DETECT CHANGES ON STATIC MESHES
			+SVerticalBox::Slot()
				.Padding(10)
				.HAlign(HAlign_Center)
				.AutoHeight()
				[
					SNew(SButton)
					.HAlign(HAlign_Center)
				.Text(LOCTEXT("DetectStaticMeshChangesButtonLabel", "Sync Static Meshes!"))
				.OnClicked_Static(&Locals::OnSyncStaticMeshes)
				]



			//OnButtonClickGetLevelActor
			// Help text BOTTOM TEXT
			+ SVerticalBox::Slot()
				.HAlign(HAlign_Center)
				.Padding(10)
				.AutoHeight()
				[
					SNew(STextBlock)
					.AutoWrapText(true)
					.MinDesiredWidth(160.0)
					.Text(LOCTEXT("HelperLabel2", "This tool is solving all the issues."))
				]

		];
		
	FModeToolkit::Init(InitToolkitHost);
}

FName FCrableEdModeToolkit::GetToolkitFName() const
{
	return FName("CrableEdMode");
}

FText FCrableEdModeToolkit::GetBaseToolkitName() const
{
	return NSLOCTEXT("CrableEdModeToolkit", "DisplayName", "CrableEdMode Tool");
}

class FEdMode* FCrableEdModeToolkit::GetEditorMode() const
{
	return GLevelEditorModeTools().GetActiveMode(FCrableEdMode::EM_CrableEdModeId);
}


#undef LOCTEXT_NAMESPACE
