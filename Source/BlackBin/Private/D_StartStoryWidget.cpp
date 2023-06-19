// Fill out your copyright notice in the Description page of Project Settings.


#include "D_StartStoryWidget.h"
#include <UMG/Public/Components/Border.h>
#include "Components/Widget.h"

 void UD_StartStoryWidget::NativeConstruct() {
	
	 Super::NativeConstruct();

	 borderCinematic->SetVisibility(ESlateVisibility::Hidden);
	 borderStoryText->OnMouseButtonDownEvent.BindUFunction(this, FName("OnClickScreen"));
	 borderCinematic->OnMouseButtonDownEvent.BindUFunction(this, FName("OnClickScreen"));

	 // Add PlayListImage/Video
	 playList.Add(borderStoryText);
	 playList.Add(borderCinematic);
}

 void UD_StartStoryWidget::OnClickScreen() {
	// Show Next Border from List
	// 1. Hide Current Border
	// 2. make Visible Next Border
	// condition : if idx > playList element count -> hide current
	 if (idx < playList.Num()-1) {
		 playList[idx]->SetVisibility(ESlateVisibility::Hidden);
		 playList[idx + 1]->SetVisibility(ESlateVisibility::Visible);
		 idx++;
	 }
	 else {
		 playList[idx]->SetVisibility(ESlateVisibility::Hidden);
	 }
 }

