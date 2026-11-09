#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SPKillZone.generated.h"

class UBoxComponent;

UCLASS()
class SHORTPERSON_API ASPKillZone : public AActor
{
	GENERATED_BODY()

public:
	ASPKillZone();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "KillZone")
	TObjectPtr<UBoxComponent> Box;

	UFUNCTION()
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);
};
