#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SPCoin.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class URotatingMovementComponent;

UCLASS()
class SHORTPERSON_API ASPCoin : public AActor
{
    GENERATED_BODY()

public:
    ASPCoin();

    UPROPERTY(EditAnywhere, Category = "Coin")
    int32 Value = 1;

protected:
    virtual void BeginPlay() override;

    // Невидима сфера, яка ловить дотик гравця
    UPROPERTY(VisibleAnywhere, Category = "Coin")
    TObjectPtr<USphereComponent> Sphere;

    // Те, що бачить гравець
    UPROPERTY(VisibleAnywhere, Category = "Coin")
    TObjectPtr<UStaticMeshComponent> Mesh;

    // Крутить монету, суто для краси
    UPROPERTY(VisibleAnywhere, Category = "Coin")
    TObjectPtr<URotatingMovementComponent> Rotating;

    // Має бути UFUNCTION, інакше AddDynamic не зможе його знайти
    UFUNCTION()
    void OnSphereOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
        bool bFromSweep, const FHitResult& SweepResult);
};