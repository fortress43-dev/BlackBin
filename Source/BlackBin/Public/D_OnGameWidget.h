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

public:
#pragma region Panels
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UCanvasPanel* PanelBattle;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UCanvasPanel* PanelBattle;

#pragma endregion

#pragma region Character Propertys
	// Boss
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UText* BossName;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class USlider* SliderBossHP;

	// Kena
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class USlider* SliderKenaHP;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class URadialSlider* RadialSliderRotCourage;

#pragma endregion
};
