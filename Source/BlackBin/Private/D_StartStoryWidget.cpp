// Fill out your copyright notice in the Description page of Project Settings.


#include "D_StartStoryWidget.h"
#include <UMG/Public/Components/Border.h>
#include "Components/Widget.h"
#include <UMG/Public/Components/Image.h>

 void UD_StartStoryWidget::NativeConstruct() {
	
	 Super::NativeConstruct();

	 StartCinamaPlayer->OnMouseButtonDownEvent.BindUFunction(this, FName("OnClickScreen"));

	 
}

 void UD_StartStoryWidget::OnClickScreen() {
	// 영상플레이 도중 화면을 클릭하면 플레이어를 끈다
	RemoveFromParent();
 }

