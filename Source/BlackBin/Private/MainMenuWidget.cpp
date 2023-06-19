// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuWidget.h"
#include <UMG/Public/Components/Button.h>
#include <UMG/Public/Components/CanvasPanel.h>
#include <Kismet/GameplayStatics.h>

void UMainMenuWidget::NativeConstruct() {

	Super::NativeConstruct();

	// Button Function Binding
	btnNewGame   ->OnClicked.AddDynamic(this, &UMainMenuWidget::OnClickNewGame);
	btnQuit	     ->OnClicked.AddDynamic(this, &UMainMenuWidget::OnClickQuit);
	btnStoryMode ->OnClicked.AddDynamic(this, &UMainMenuWidget::OnClickStoryMode);
	
	// Panel visibility Init
	PanelDifficulty->SetVisibility(ESlateVisibility::Hidden);
}

void UMainMenuWidget::OnClickNewGame() {
	// Toggle Visibility
	if (PanelDifficulty->GetVisibility() == ESlateVisibility::Visible) {
		PanelDifficulty->SetVisibility(ESlateVisibility::Hidden);
	}
	else {
		PanelDifficulty->SetVisibility(ESlateVisibility::Visible);
	}
}

void UMainMenuWidget::OnClickStoryMode() {
	// Open Level of Specific Name
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("OnGame"));
}

void UMainMenuWidget::OnClickQuit() {
	// Quit Game
	UKismetSystemLibrary::QuitGame(GetWorld(),GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, false);
}