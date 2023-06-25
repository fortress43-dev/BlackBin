// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "D_GameManager.generated.h"

UENUM()
enum EGuideText
{
	Barrier,
};

UCLASS()
class BLACKBIN_API AD_GameManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AD_GameManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
#pragma region Max Hp Settings
	UPROPERTY(EditAnywhere, Category = "DefaultSetting")
	int bossHp    = 100;
	UPROPERTY(EditAnywhere, Category = "DefaultSetting")
	int playerHp  = 100;
	UPROPERTY(EditAnywhere, Category = "DefaultSetting")
	int barrierHp = 100;
#pragma endregion




#pragma region Widgets
	//SourceClasses
	UPROPERTY(EditAnywhere, Category = "Widgets")
	TSubclassOf<class UMainMenuWidget> mainMenuWidgetSource;
	UPROPERTY(EditAnywhere, Category = "Widgets")
	TSubclassOf<class UD_StartStoryWidget> startStoryWidgetSource;
	UPROPERTY(EditAnywhere, Category = "Widgets")
	TSubclassOf<class UD_OnGameWidget> onGameWidgetSource;

	//Created Widget
	UPROPERTY(EditAnywhere)
	class UMainMenuWidget* mainMenuWidget = nullptr;
	UPROPERTY(EditAnywhere)
	class UD_StartStoryWidget* startStoryWidget = nullptr;
	UPROPERTY(EditAnywhere)
	class UD_OnGameWidget* onGameWidget = nullptr;
	
	//MediaPlayer
	UPROPERTY(EditAnywhere, Category = "MediaPlayers")
	class UMediaPlayer* openingMediaPlayer;
#pragma endregion



#pragma region MapNames
	UPROPERTY(VisibleAnywhere, Category = "Maps Names")
	FString MainMenuLevelName = TEXT("MainMenu");
	UPROPERTY(EditAnywhere, Category = "Maps Names")
	FString OnGameLevelName   = TEXT("OnGame");
#pragma endregion


#pragma region Functions
	
	void ShowMainWidget(int zOrder);
	void ShowStoryWidget(int zOrder);
	void ShowOnGameWidget(int zOrder);

	void ShowGuideText(EGuideText);
	void SetBattlePanelVisibility(bool);

	void HideStoryWidget();
#pragma endregion


	

};

