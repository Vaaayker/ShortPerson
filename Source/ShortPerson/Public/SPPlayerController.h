#pragma once

#include "CoreMinimal.h"
#include "ShortPersonPlayerController.h"
#include "SPPlayerController.generated.h"

class USPHUDWidget;

UCLASS()
class SHORTPERSON_API ASPPlayerController : public AShortPersonPlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<USPHUDWidget> HUDWidgetClass;

	UPROPERTY()
	TObjectPtr<USPHUDWidget> HUDWidget;
};