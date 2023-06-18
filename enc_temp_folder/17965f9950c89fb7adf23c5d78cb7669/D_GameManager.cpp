// Fill out your copyright notice in the Description page of Project Settings.


#include "D_GameManager.h"
#include "D_OnGameWidget.h"
#include "D_StartStoryWidget.h"
#include "MainMenuWidget.h"
#include "DebugMessages.h"


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
		mainMenuWidget = tmpMainWidget.Class;
	}

	// #StartStoryWidget 
	// ������ ó�� ������ �� �ó׸����� ���ʷ� �����ϰ� �ִ� ����
	// �Ŀ� �ó׸�ƽ���� �÷����ϴ� �뵵�� ����� ��ȹ
	ConstructorHelpers::FClassFinder<UD_StartStoryWidget> tmpStoryWidget(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/DKW/UI/BP_StoryStartWidget.BP_StoryStartWidget_C'"));
	if (tmpStoryWidget.Succeeded()) {
		startStoryWidget = tmpStoryWidget.Class;
	}

	// #OnGameWidget
	// ���� ���� ��ȣ�ۿ뿡 ���� �ȳ�
	// ������ UI�� �Բ� �����Ѵ�/ �гη� �и��Ͽ� ����
	ConstructorHelpers::FClassFinder<UD_OnGameWidget> tmpOnGameWidget(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/DKW/UI/BP_OnGameWidget.BP_OnGameWidget_C'"));
	if (tmpOnGameWidget.Succeeded()) {
		onGameWidget = tmpOnGameWidget.Class;
	}

#pragma endregion
	
}

// Called when the game starts or when spawned
void AD_GameManager::BeginPlay()
{
	Super::BeginPlay();
	// #RoadDefaultWidget
	// ������ ���� �� ������ �⺻ ������ ��ȯ�Ѵ�
	// Level : Start      Widget :  mainMenuWidget
	// Level : Main       Widget :  startStoryWidget

	// map �̸��� �˻��Ѵ�
	// map �̸��� ����  switch  �б⸦ ������
	// �׿� �´� ������ ����

	// current Map Name
	FString curMapName = GetWorld()->GetMapName();
	printf("%s", *curMapName);
	if (curMapName.Contains(MainMenuLevelName, ESearchCase::IgnoreCase)){
		if (mainMenuWidget) {
			ShowMainWidget();
		}
	}
	else if(curMapName.Contains(OnGameLevelName, ESearchCase::IgnoreCase)){
		if (startStoryWidget) {
			ShowStoryWidget();
		}
	}
}

// Called every frame
void AD_GameManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Main widgets
void AD_GameManager::ShowMainWidget() {
	// #FunctionDescribtion
	// ���콺�� ���̰� �Ѵ� 
	// Main Widget �� viewport �� ����
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
	UMainMenuWidget* ui = CreateWidget<UMainMenuWidget>(GetWorld(), mainMenuWidget);
	ui->AddToViewport();
}

void AD_GameManager::ShowStoryWidget() {
	// #FunctionDescribtion
		// ���콺�� ���̰� �Ѵ� 
		// Main Widget �� viewport �� ����
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
	UD_StartStoryWidget* ui = CreateWidget<UD_StartStoryWidget>(GetWorld(), startStoryWidget);
	ui->AddToViewport();
}

void AD_GameManager::ShowOnGameWidget() {
	// #FunctionDescribtion
		// ���콺�� ���̰� �Ѵ� 
		// Main Widget �� viewport �� ����
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
	UD_OnGameWidget* ui = CreateWidget<UD_OnGameWidget>(GetWorld(), onGameWidget);
	ui->AddToViewport();
}


// Sub widgets

void AD_GameManager::ShowUIDifficulty() {
	
}

void AD_GameManager::StartGame() {

}
