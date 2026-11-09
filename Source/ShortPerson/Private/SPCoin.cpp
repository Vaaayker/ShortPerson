#include "SPCoin.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "GameFramework/Pawn.h"
#include "SPPlayerState.h"
#include "ShortPersonGameMode.h"

ASPCoin::ASPCoin()
{
    PrimaryActorTick.bCanEverTick = false;   // Tick не потрібен
    bReplicates = true;                       // щоб Destroy() дійшов до клієнтів

    Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
    Sphere->InitSphereRadius(50.f);
    Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);  // тільки перевірки, без фізики
    Sphere->SetCollisionResponseToAllChannels(ECR_Ignore);
    Sphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap); // реагує лише на Pawn
    RootComponent = Sphere;

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    Mesh->SetupAttachment(Sphere);
    Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);  // меш не заважає ходити

    Rotating = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("Rotating"));
    Rotating->RotationRate = FRotator(0.f, 180.f, 0.f);
}

void ASPCoin::BeginPlay()
{
    Super::BeginPlay();
    Sphere->OnComponentBeginOverlap.AddDynamic(this, &ASPCoin::OnSphereOverlap);
}

void ASPCoin::OnSphereOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult& SweepResult)
{
    if (!HasAuthority()) return;   // очки рахує тільки сервер

    APawn* Pawn = Cast<APawn>(OtherActor);
    if (!Pawn || !Pawn->IsPlayerControlled()) return;   // NPC монету не збирають

    if (ASPPlayerState* PS = Pawn->GetPlayerState<ASPPlayerState>())
    {
        PS->SetScore(PS->GetScore() + Value);
        PS->CoinsCollected++;
        UE_LOG(LogTemp, Warning, TEXT("Coin! Score = %f"), PS->GetScore());
    }

    if (AShortPersonGameMode* GM = GetWorld()->GetAuthGameMode<AShortPersonGameMode>())
    {
        GM->OnCoinCollected(this);
    }

    Destroy();
}

