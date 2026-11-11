// DoorBase.h
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.h"
#include "DoorBase.generated.h"

UCLASS()
class SHORTPERSON_API ADoorBase : public AActor, public IInteractable
{
    GENERATED_BODY()
public:
    ADoorBase();
    virtual void Tick(float DeltaTime) override;

    virtual void Interact_Implementation(AActor* Interactor) override;
    virtual FText GetInteractionText_Implementation() const override;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Door")
    TObjectPtr<USceneComponent> Hinge;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Door")
    TObjectPtr<UStaticMeshComponent> DoorMesh;

    UPROPERTY(EditAnywhere, Category = "Door", meta = (ClampMin = "-170", ClampMax = "170", Units = "Degrees"))
    float OpenAngle = 90.f;

    UPROPERTY(EditAnywhere, Category = "Door", meta = (ClampMin = "0.5", ClampMax = "20"))
    float OpenSpeed = 6.f;

    UPROPERTY(EditAnywhere, Category = "Door|Lock")
    bool bLocked = false;

    UPROPERTY(EditAnywhere, Category = "Door|Lock", meta = (EditCondition = "bLocked", EditConditionHides))
    FText LockedMessage = NSLOCTEXT("Door", "Locked", "Зачинено");

private:
    bool bIsOpen = false;
    float CurrentYaw = 0.f;
};
