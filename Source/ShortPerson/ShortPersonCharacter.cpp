// Copyright Epic Games, Inc. All Rights Reserved.

#include "ShortPersonCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "ShortPerson.h"
#include "SPHealthComponent.h"
#include "ShortPersonGameMode.h"
#include "Interactable.h"
#include "DrawDebugHelpers.h"
#include "WeaponBase.h"


AShortPersonCharacter::AShortPersonCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	HealthComponent = CreateDefaultSubobject<USPHealthComponent>(TEXT("HealthComponent"));
}

void AShortPersonCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AShortPersonCharacter::Move);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &AShortPersonCharacter::Look);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AShortPersonCharacter::Look);
	
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &AShortPersonCharacter::Interact);
		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Triggered, this, &AShortPersonCharacter::Dash);
	}
	else
	{
		UE_LOG(LogShortPerson, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AShortPersonCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	// route the input
	DoMove(MovementVector.X, MovementVector.Y);
}

void AShortPersonCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// route the input
	DoLook(LookAxisVector.X, LookAxisVector.Y);
}

void AShortPersonCharacter::DoMove(float Right, float Forward)
{
	if (GetController() != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = GetController()->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, Forward);
		AddMovementInput(RightDirection, Right);
	}
}

void AShortPersonCharacter::DoLook(float Yaw, float Pitch)
{
	if (GetController() != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void AShortPersonCharacter::DoJumpStart()
{
	// signal the character to jump
	Jump();
}

void AShortPersonCharacter::DoJumpEnd()
{
	// signal the character to stop jumping
	StopJumping();
}

void AShortPersonCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (HealthComponent)
	{
		HealthComponent->OnDeath.AddDynamic(this, &AShortPersonCharacter::HandleDeath);
	}
}

void AShortPersonCharacter::HandleDeath(AActor* DeadActor)
{
	UE_LOG(LogTemp, Warning, TEXT("OnDeath: %s died"), *GetNameSafe(DeadActor));

	if (!HasAuthority()) return;   // смерть обробляє тільки сервер

	if (AShortPersonGameMode* GM = GetWorld()->GetAuthGameMode<AShortPersonGameMode>())
	{
		GM->OnPlayerDied(GetController());
	}
}

void AShortPersonCharacter::BeginPlay()
{
	Super::BeginPlay(); // тут відпрацює BP Event BeginPlay
	GetWorldTimerManager().SetTimer(InteractTraceTimer, this,
		&AShortPersonCharacter::TraceForInteractable, TraceInterval, true);

	FActorSpawnParameters P;
	P.Owner = this;
	P.Instigator = this;
	for (const TSubclassOf<AWeaponBase>& WeaponClass : StartingWeapons)
	{
		if (!WeaponClass) continue;
		AWeaponBase* W = GetWorld()->SpawnActor<AWeaponBase>(WeaponClass, GetActorTransform(), P);
		W->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);
		W->SetActorHiddenInGame(true);
		Weapons.Add(W);
	}
	if (Weapons.Num() > 0) EquipWeapon(0);
}

void AShortPersonCharacter::TraceForInteractable()
{
	const FVector Start = FollowCamera->GetComponentLocation();
	const FVector End = Start + FollowCamera->GetForwardVector() * (CameraBoom->TargetArmLength + InteractRange);

	FCollisionQueryParams Params(SCENE_QUERY_STAT(InteractTrace), false, this);
	FHitResult Hit;
	AActor* NewFocus = nullptr;

	if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params))
	{
		AActor* HitActor = Hit.GetActor();
		if (HitActor && HitActor->Implements<UInteractable>()
			&& FVector::Dist(GetActorLocation(), Hit.ImpactPoint) <= InteractRange)
		{
			NewFocus = HitActor;
		}
	}

	if (bDrawDebugTrace)
		DrawDebugLine(GetWorld(), Start, End, NewFocus ? FColor::Green : FColor::Red, false, TraceInterval);

	FocusedActor = NewFocus;
	if (NewFocus && GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, TraceInterval + 0.05f, FColor::Yellow,
			IInteractable::Execute_GetInteractionText(NewFocus).ToString());
	}
}

void AShortPersonCharacter::Interact()
{
	if (AActor* Target = FocusedActor.Get())
		IInteractable::Execute_Interact(Target, this);
}

void AShortPersonCharacter::Dash()
{
	if (!bCanDash) return;
	FVector Dir = GetLastMovementInputVector().GetSafeNormal2D();
	if (Dir.IsNearlyZero()) Dir = GetActorForwardVector();
	LaunchCharacter(Dir * DashStrength + FVector(0, 0, 100), true, false);
}

void AShortPersonCharacter::EquipWeapon(int32 Index)
{
	if (!Weapons.IsValidIndex(Index)) return;
	if (CurrentWeapon)
	{
		CurrentWeapon->StopFire();
		CurrentWeapon->SetActorHiddenInGame(true);
	}
	CurrentWeaponIndex = Index;
	CurrentWeapon = Weapons[Index];
	CurrentWeapon->SetActorHiddenInGame(false);
	OnWeaponChanged.Broadcast(CurrentWeapon);
}

void AShortPersonCharacter::NextWeapon() { if (Weapons.Num() > 1) EquipWeapon((CurrentWeaponIndex + 1) % Weapons.Num()); }
void AShortPersonCharacter::StartFire() { if (CurrentWeapon) CurrentWeapon->StartFire(); }
void AShortPersonCharacter::StopFire() { if (CurrentWeapon) CurrentWeapon->StopFire(); }
void AShortPersonCharacter::Reload() { if (CurrentWeapon) CurrentWeapon->Reload(); }

