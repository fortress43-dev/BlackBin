// Fill out your copyright notice in the Description page of Project Settings.


#include "D_StartStoryWidget.h"
#include <UMG/Public/Components/Border.h>
#include "Components/Widget.h"

 void UD_StartStoryWidget::NativeConstruct() {
	
	 Super::NativeConstruct();

	 StartCinamaPlayer->OnMouseButtonDownEvent.BindUFunction(this, FName("OnClickScreen"));


}

 void UD_StartStoryWidget::OnClickScreen() {
	
 }

