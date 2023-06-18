// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"


UCLASS()
class BLACKBIN_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

public:
#pragma region Buttons
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* btnNewGame;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* btnStoryMode;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* btnQuit;

#pragma endregion


#pragma region Panels
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UCanvasPanel* PanelMenu;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UCanvasPanel* PanelDifficulty;

#pragma endregion


#pragma region Functions
	UFUNCTION()
	void OnClickNewGame();

	UFUNCTION()
	void OnClickStoryMode();

	UFUNCTION()
	void OnClickQuit();

#pragma endregion

	UPROPERTY()
		float timer = 0;


	


};