// Fill out your copyright notice in the Description page of Project Settings.


#include "D_GameManager.h"
#include "D_OnGameWidget.h"
#include "D_StartStoryWidget.h"
#include "MainMenuWidget.h"
#include "DebugMessages.h"
#include <UMG/Public/Components/CanvasPanel.h>


// <UI ǥ������>
// 1. ���� ���� �̸��� Ȯ���Ѵ�.
// 2. �ʷ������� ���¸� Ȯ���Ѵ�.
// �� �� ���� ���ǿ� ���ؼ� ǥ���� UI�� �޶�����

// Sets default values
AD_GameManager::AD_GameManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// #ConstructWidgets
	// find Widget on Browser Folder and bind it to variables.
#pragma region ConstructorHelpers

	// #MainMenuWidget
	// ������ ���� �޴�. ���ӳ��̵� / ����ϱ� / ������ �� ���ð���
	ConstructorHelpers::FClassFinder<UMainMenuWidget> tmpMainWidget(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/DKW/UI/BP_MainMenuWidget.BP_MainMenuWidget_C'"));
	if (tmpMainWidget.Succeeded()) {
		mainMenuWidgetSource = tmpMainWidget.Class;
	}

	// #StartStoryWidget 
	// ������ ó�� ������ �� �ó׸����� ���ʷ� �����ϰ� �ִ� ����
	// �Ŀ� �ó׸�ƽ���� �÷����ϴ� �뵵�� ����� ��ȹ
	ConstructorHelpers::FClassFinder<UD_StartStoryWidget> tmpStoryWidget(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/DKW/UI/BP_StoryStartWidget.BP_StoryStartWidget_C'"));
	if (tmpStoryWidget.Succeeded()) {
		startStoryWidgetSource = tmpStoryWidget.Class;
	}

	// #OnGameWidget
	// ���� ���� ��ȣ�ۿ뿡 ���� �ȳ�
	// ������ UI�� �Բ� �����Ѵ�/ �гη� �и��Ͽ� ����
	ConstructorHelpers::FClassFinder<UD_OnGameWidget> tmpOnGameWidget(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/DKW/UI/BP_OnGameWidget.BP_OnGameWidget_C'"));
	if (tmpOnGameWidget.Succeeded()) {
		onGameWidgetSource = tmpOnGameWidget.Class;
	}

#pragma endregion
	
}

// Called when the game starts or when spawned
void AD_GameManager::BeginPlay()
{
	Super::BeginPlay();
	// #RoadDefaultWidget
	// ������ ���� �� ������ �⺻ ������ ��ȯ�Ѵ�
	// Level : MainMenu      Widget :  mainMenuWidget
	// Level : OnGame        Widget :  startStoryWidget

	// map �̸��� �˻��Ѵ�
	// map �̸��� ����  switch  �б⸦ ������
	// �׿� �´� ������ ����

	// current Map Name
	FString curMapName = GetWorld()->GetMapName();
	printf("%s", *curMapName);
	if (curMapName.Contains(MainMenuLevelName, ESearchCase::IgnoreCase)){
		if (mainMenuWidgetSource) {
			ShowMainWidget(0);
		}
	}
	else if(curMapName.Contains(OnGameLevelName, ESearchCase::IgnoreCase)){
		if (startStoryWidgetSource && onGameWidgetSource) {
			ShowStoryWidget(1);
			ShowOnGameWidget(0);
		}
	}
}

// Called every frame
void AD_GameManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

#pragma region Show Widgets

//Show Main widgets
void AD_GameManager::ShowMainWidget(int zOrder = 0) {
	// #FunctionDescribtion
	// ���콺�� ���̰� �Ѵ� 
	// Main Widget �� viewport �� ����
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
	mainMenuWidget = CreateWidget<UMainMenuWidget>(GetWorld(), mainMenuWidgetSource);
	mainMenuWidget->AddToViewport(zOrder);
}

//ShowStrotyWidget
void AD_GameManager::ShowStoryWidget(int zOrder = 0) {
	// #FunctionDescribtion
	// ���콺�� ���̰� �Ѵ� 
	// Main Widget �� viewport �� ����
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
	startStoryWidget = CreateWidget<UD_StartStoryWidget>(GetWorld(), startStoryWidgetSource);
	startStoryWidget->AddToViewport(zOrder);
}

//ShowOnGameWidget
void AD_GameManager::ShowOnGameWidget(int zOrder = 0) {
	// #FunctionDescribtion
		// ���콺�� ���̰� �Ѵ� 
		// Main Widget �� viewport �� ����
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
	onGameWidget = CreateWidget<UD_OnGameWidget>(GetWorld(), onGameWidgetSource);
	onGameWidget->AddToViewport(zOrder);
	
}

#pragma endregion

#pragma region Controll WidgetElements
// Show skill GuideText
// ����   : ShowGuideText(EGuideText::Barrier);
// ������� : onGameWidget
void AD_GameManager::ShowGuideText(EGuideText guide) {

	// �䱸�� �Ϳ� �´� ���̵带 ǥ���Ѵ�
	// ���������� �����Ǹ� ���̵带 �����Ѵ�
	// ���������� ���� : �ð�, Ű�Է�
	// 
	// ���� ���������� ���� �� �ִ� bool �Լ��� �ʿ��ϴ�


	switch (guide)
	{
	case Barrier:
		print("Press E to use Barrier");
		// E Ű�� ������ visible �� ������
		// �䱸���� : Key �Է��� �ν��ؾ� �Ѵ�
		// ��� �ؾ� ���� �� - ã��

		break;
	default:
		break;
	}
}

// Set UI Panel for battle like Boss HP, Player HP
// ������� : onGameWidget
void AD_GameManager::SetBattlePanelVisibility(bool isVisible) {
	// onGameWidget�� battle Panel �� Ȱ��ȭ��Ų��
<<<<<<< HEAD
	//if (onGameWidget) {
	//	if (isVisible) {
	//		onGameWidget->PanelBattle->Visibility = ESlateVisibility::Visible;
	//	}
	//	else{
	//		onGameWidget->PanelBattle->Visibility = ESlateVisibility::Hidden;
	//	}
	//}
=======
	/*if (onGameWidget) {
		if (isVisible) {
			onGameWidget->PanelBattle->Visibility = ESlateVisibility::Visible;
		}
		else{
			onGameWidget->PanelBattle->Visibility = ESlateVisibility::Hidden;
		}
	}*/
>>>>>>> DKW_alpha_fromMain
}

#pragma endregion

