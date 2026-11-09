#include "SPKillZone.h"
#include "Components/BoxComponent.h"
#include "SPHealthComponent.h"

ASPKillZone::ASPKillZone()
{
	PrimaryActorTick.bCanEverTick = false;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	Box->SetBoxExtent(FVector(3000.f, 3000.f, 200.f));
	Box->SetCollisionEnabled(ECollisionEnabled::QueryOnly);         // тільки перевірки
	Box->SetCollisionResponseToAllChannels(ECR_Ignore);
	Box->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);      // реагує лише на Pawn
	Box->SetGenerateOverlapEvents(true);
	RootComponent = Box;
}

void ASPKillZone::BeginPlay()
{
	Super::BeginPlay();
	Box->OnComponentBeginOverlap.AddDynamic(this, &ASPKillZone::OnBoxOverlap);
}

void ASPKillZone::OnBoxOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	if (!HasAuthority() || !OtherActor) return;   // шкоду рахує тільки сервер

	if (USPHealthComponent* Health = OtherActor->FindComponentByClass<USPHealthComponent>())
	{
		UE_LOG(LogTemp, Warning, TEXT("KillZone: %s entered"), *GetNameSafe(OtherActor));
		Health->ApplyDamage(Health->GetMaxHealth());
	}
}