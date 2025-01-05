// Fill out your copyright notice in the Description page of Project Settings.


#include "DT_UserWidget.h"

void UDT_UserWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	WidgetControllerSet();
}
