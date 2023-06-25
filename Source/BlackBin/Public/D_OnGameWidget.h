// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"


#include "D_OnGameWidget.generated.h"

/**
 * 
 */
UCLASS()
class BLACKBIN_API UD_OnGameWidget : public UUserWidget
{
	GENERATED_BODY()
		

public:
	virtual void NativeConstruct() override;

#pragma region UI Elements
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UCanvasPanel* PanelBossInfo;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UProgressBar* BossHpProgressBar;


	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UProgressBar* PlayerHpProgressBar;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UProgressBar* BarrierHpProgressBar;


	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* SkillGuideText;
#pragma endregion




#pragma region MaxHpSettings
// This values will be set by GameManager
	// when it create this widget class
	UPROPERTY(VisibleAnywhere, Category = MaxHp)
		float bossMaxHp		= 100.;
	UPROPERTY(VisibleAnywhere, Category = MaxHp)
		float playerMaxHp  = 100.;
	UPROPERTY(VisibleAnywhere, Category = MaxHp)
		float barrierMaxHp = 100.;
#pragma endregion

	

	// Data Transfer Functions
	void SetBossHpBar   (int32 curHp);
	void SetPlayerHpBar (int32 curHP);
	void SetBarrierHpBar(int32 curHp);
	void SetGuideText   (FText text);

	
};
