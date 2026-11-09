#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SPHUDWidget.generated.h"

class UTextBlock;

UCLASS()
class SHORTPERSON_API USPHUDWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TimeText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ScoreText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> CoinsText;
};