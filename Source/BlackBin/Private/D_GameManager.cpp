// Fill out your copyright notice in the Description page of Project Settings.


#include "D_GameManager.h"
#include "D_OnGameWidget.h"
#include "D_StartStoryWidget.h"
#include "MainMenuWidget.h"
#include "DebugMessages.h"
#include <UMG/Public/Components/CanvasPanel.h>


// <UI 표시조건>
// 1. 현재 맵의 이름을 확인한다.
// 2. 맵레벨에서 상태를 확인한다.
// 위 두 가지 조건에 의해서 표시할 UI가 달라진다

// Sets default values
AD_GameManager::AD_GameManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// #ConstructWidgets
	// find Widget on Browser Folder and bind it to variables.
#pragma region ConstructorHelpers

	// #MainMenuWidget
	// 시작할 때의 메뉴. 게임난이도 / 계속하기 / 끝내기 등 선택가능
	ConstructorHelpers::FClassFinder<UMainMenuWidget> tmpMainWidget(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/DKW/UI/BP_MainMenuWidget.BP_MainMenuWidget_C'"));
	if (tmpMainWidget.Succeeded()) {
		mainMenuWidgetSource = tmpMainWidget.Class;
	}

	// #StartStoryWidget 
	// 게임을 처음 시작할 때 시네마들을 차례로 저장하고 있는 위젯
	// 후에 시네마틱들을 플레이하는 용도로 사용할 계획
	ConstructorHelpers::FClassFinder<UD_StartStoryWidget> tmpStoryWidget(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/DKW/UI/BP_StoryStartWidget.BP_StoryStartWidget_C'"));
	if (tmpStoryWidget.Succeeded()) {
		startStoryWidgetSource = tmpStoryWidget.Class;
	}

	// #OnGameWidget
	// 게임 내의 상호작용에 대한 안내
	// 보스전 UI를 함께 포함한다/ 패널로 분리하여 관리
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
	// 시작할 때는 각 레벨의 기본 위젯을 소환한다
	// Level : MainMenu      Widget :  mainMenuWidget
	// Level : OnGame        Widget :  startStoryWidget

	// map 이름을 검색한다
	// map 이름에 따른  switch  분기를 나눈다
	// 그에 맞는 위젯을 띄운다

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
	// 마우스를 보이게 한다 
	// Main Widget 을 viewport 에 띄운다
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
	mainMenuWidget = CreateWidget<UMainMenuWidget>(GetWorld(), mainMenuWidgetSource);
	mainMenuWidget->AddToViewport(zOrder);
}

//ShowStrotyWidget
void AD_GameManager::ShowStoryWidget(int zOrder = 0) {
	// #FunctionDescribtion
	// 마우스를 보이게 한다 
	// Main Widget 을 viewport 에 띄운다
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
	startStoryWidget = CreateWidget<UD_StartStoryWidget>(GetWorld(), startStoryWidgetSource);
	startStoryWidget->AddToViewport(zOrder);
}

//ShowOnGameWidget
void AD_GameManager::ShowOnGameWidget(int zOrder = 0) {
	// #FunctionDescribtion
		// 마우스를 보이게 한다 
		// Main Widget 을 viewport 에 띄운다
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
	onGameWidget = CreateWidget<UD_OnGameWidget>(GetWorld(), onGameWidgetSource);
	onGameWidget->AddToViewport(zOrder);
	
}

#pragma endregion

#pragma region Controll WidgetElements
// Show skill GuideText
// 사용법   : ShowGuideText(EGuideText::Barrier);
// 사용위젯 : onGameWidget
void AD_GameManager::ShowGuideText(EGuideText guide) {

	// 요구한 것에 맞는 가이드를 표시한다
	// 종료조건이 충족되면 가이드를 종료한다
	// 종료조건의 종류 : 시간, 키입력
	// 
	// 여러 종류조건을 담을 수 있는 bool 함수가 필요하다


	switch (guide)
	{
	case Barrier:
		print("Press E to use Barrier");
		// E 키를 누르면 visible 이 꺼진다
		// 요구조건 : Key 입력을 인식해야 한다
		// 어떻게 해야 할지 모름 - 찾기

		break;
	default:
		break;
	}
}

// Set UI Panel for battle like Boss HP, Player HP
// 사용위젯 : onGameWidget
void AD_GameManager::SetBattlePanelVisibility(bool isVisible) {
	// onGameWidget의 battle Panel 을 활성화시킨다
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

