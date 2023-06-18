// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "D_StartStoryWidget.generated.h"

/**
 * 
 */
UCLASS()
class BLACKBIN_API UD_StartStoryWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

#pragma region Borders
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UBorder* borderStoryText;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UBorder* borderCinematic;

	UPROPERTY(EditAnywhere)
	TArray<UBorder*> playList;
		/*=
		{ borderStoryText ,
		  borderCinematic ,
		};*/

	// playerIndex
	UPROPERTY()
	int idx = 0;
#pragma endregion

#pragma region Functions
	UFUNCTION()
	void OnClickScreen();
	void StartGame();

#pragma endregion


};
