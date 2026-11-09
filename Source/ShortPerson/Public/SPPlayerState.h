#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SPPlayerState.generated.h"

UCLASS()
class SHORTPERSON_API ASPPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	// Score вже є в APlayerState: GetScore() / SetScore()

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	int32 Deaths = 0;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Coins")
	int32 CoinsCollected = 0;

protected:
	virtual void BeginPlay() override;
};
