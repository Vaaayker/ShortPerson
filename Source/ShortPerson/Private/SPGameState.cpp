#include "SPGameState.h"
#include "Net/UnrealNetwork.h"

void ASPGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ASPGameState, RemainingTime);
	DOREPLIFETIME(ASPGameState, CoinsLeft);
	DOREPLIFETIME(ASPGameState, bRoundActive);
}

void ASPGameState::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("ASPGameState::BeginPlay"));
}