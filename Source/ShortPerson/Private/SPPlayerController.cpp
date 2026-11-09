#include "SPPlayerController.h"
#include "SPHUDWidget.h"
#include "Blueprint/UserWidget.h"

void ASPPlayerController::BeginPlay()
{
	Super::BeginPlay();   // шаблонний контролер тут додає mapping contexts

	if (!IsLocalController()) return;   // HUD тільки для локального гравця

	if (!HUDWidgetClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("SPPlayerController: HUDWidgetClass не задано в BP"));
		return;
	}

	HUDWidget = CreateWidget<USPHUDWidget>(this, HUDWidgetClass);
	if (HUDWidget)
	{
		HUDWidget->AddToViewport();
	}
}