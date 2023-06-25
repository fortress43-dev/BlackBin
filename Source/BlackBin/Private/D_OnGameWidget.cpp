// Fill out your copyright notice in the Description page of Project Settings.


#include "D_OnGameWidget.h"
#include <UMG/Public/Components/ProgressBar.h>
#include <UMG/Public/Components/TextBlock.h>

void UD_OnGameWidget::NativeConstruct() {

	Super::NativeConstruct();

}

void UD_OnGameWidget::SetBossHpBar(int32 curHp)
{
	if(curHp < 0) curHp = 0;
	if(curHp > bossMaxHp) curHp = bossMaxHp;
	BossHpProgressBar->SetPercent(curHp / bossMaxHp);
}

void UD_OnGameWidget::SetPlayerHpBar(int32 curHp)
{
	if (curHp < 0) curHp = 0;
	if (curHp > playerMaxHp) curHp = playerMaxHp;
	PlayerHpProgressBar->SetPercent(curHp / playerMaxHp);
}

void UD_OnGameWidget::SetBarrierHpBar(int32 curHp)
{
	if (curHp < 0) curHp = 0;
	if (curHp > barrierMaxHp) curHp = barrierMaxHp;
	BarrierHpProgressBar->SetPercent(curHp / barrierMaxHp);
}

void UD_OnGameWidget::SetGuideText(FText text)
{
	SkillGuideText->SetText(text);
}

