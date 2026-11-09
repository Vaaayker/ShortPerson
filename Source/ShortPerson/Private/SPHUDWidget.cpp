#include "SPHUDWidget.h"
#include "Components/TextBlock.h"
#include "SPGameState.h"
#include "SPPlayerState.h"

void USPHUDWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	UWorld* World = GetWorld();
	if (!World) return;

	if (ASPGameState* GS = World->GetGameState<ASPGameState>())
	{
		TimeText->SetText(FText::FromString(
			FString::Printf(TEXT("Time: %d"), static_cast<int32>(GS->RemainingTime))));

		CoinsText->SetText(FText::FromString(
			FString::Printf(TEXT("Coins left: %d"), static_cast<int32>(GS->CoinsLeft))));
	}

	if (ASPPlayerState* PS = GetOwningPlayerState<ASPPlayerState>())
	{
		ScoreText->SetText(FText::FromString(
			FString::Printf(TEXT("Score: %d"), static_cast<int32>(PS->GetScore()))));
	}
}