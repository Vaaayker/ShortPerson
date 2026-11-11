// DoorBase.cpp
#include "DoorBase.h"

ADoorBase::ADoorBase()
{
    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.bStartWithTickEnabled = false; // тікаємо лише під час руху

    SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("Root")));
    Hinge = CreateDefaultSubobject<USceneComponent>(TEXT("Hinge"));
    Hinge->SetupAttachment(GetRootComponent());
    DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
    DoorMesh->SetupAttachment(Hinge);
}

void ADoorBase::Interact_Implementation(AActor* Interactor)
{
    if (bLocked)
    {
        if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, LockedMessage.ToString());
        return;
    }
    bIsOpen = !bIsOpen;
    SetActorTickEnabled(true);
}

FText ADoorBase::GetInteractionText_Implementation() const
{
    return bIsOpen ? NSLOCTEXT("Door", "Close", "Зачинити двері") : NSLOCTEXT("Door", "Open", "Відчинити двері");
}

void ADoorBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    const float Target = bIsOpen ? OpenAngle : 0.f;
    CurrentYaw = FMath::FInterpTo(CurrentYaw, Target, DeltaTime, OpenSpeed);
    if (FMath::IsNearlyEqual(CurrentYaw, Target, 0.1f))
    {
        CurrentYaw = Target;
        SetActorTickEnabled(false);
    }
    Hinge->SetRelativeRotation(FRotator(0.f, CurrentYaw, 0.f));
}

