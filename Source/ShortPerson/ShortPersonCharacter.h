// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "ShortPersonCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputAction;
struct FInputActionValue;
class USPHealthComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponChanged, AWeaponBase*, NewWeapon);
DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

/**
 *  A simple player-controllable third person character
 *  Implements a controllable orbiting camera
 */
UCLASS(abstract)
class AShortPersonCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
protected:

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* LookAction;

	/** Mouse Look Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MouseLookAction;

public:

	/** Constructor */
	AShortPersonCharacter();	

protected:

	/** Initialize input action bindings */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

public:

	/** Handles move inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoMove(float Right, float Forward);

	/** Handles look inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoLook(float Yaw, float Pitch);

	/** Handles jump pressed inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpStart();

	/** Handles jump pressed inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpEnd();

public:

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

public:
	USPHealthComponent* GetHealthComponent() const { return HealthComponent; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
	TObjectPtr<USPHealthComponent> HealthComponent;

	virtual void PostInitializeComponents() override;

	UFUNCTION()
	void HandleDeath(AActor* DeadActor);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Gameplay")
	TObjectPtr<UInputAction> InteractAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Gameplay")
	TObjectPtr<UInputAction> DashAction;

	UPROPERTY(EditDefaultsOnly, Category = "Interaction", meta = (ClampMin = "50", Units = "cm"))
	float InteractRange = 250.f;

	UPROPERTY(EditDefaultsOnly, Category = "Interaction", meta = (ClampMin = "0.02", ClampMax = "1", Units = "s"))
	float TraceInterval = 0.1f;

	UPROPERTY(EditDefaultsOnly, Category = "Interaction|Debug")
	bool bDrawDebugTrace = false;

	UPROPERTY(EditDefaultsOnly, Category = "Movement|Dash")
	bool bCanDash = true;

	UPROPERTY(EditDefaultsOnly, Category = "Movement|Dash", meta = (EditCondition = "bCanDash", ClampMin = "0"))
	float DashStrength = 1800.f;

	virtual void BeginPlay() override; // якщо в шаблоні його немає

private:
	void TraceForInteractable();
	void Interact();
	void Dash();

	FTimerHandle InteractTraceTimer;
	TWeakObjectPtr<AActor> FocusedActor;

	public:
		UPROPERTY(BlueprintAssignable, Category = "Weapon")
		FOnWeaponChanged OnWeaponChanged;

		UFUNCTION(BlueprintPure, Category = "Weapon")
		AWeaponBase* GetCurrentWeapon() const { return CurrentWeapon; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Gameplay") TObjectPtr<UInputAction> FireAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Gameplay") TObjectPtr<UInputAction> ReloadAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Gameplay") TObjectPtr<UInputAction> NextWeaponAction;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TArray<TSubclassOf<AWeaponBase>> StartingWeapons;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	FName WeaponSocket = TEXT("hand_r");

private:
	void StartFire();
	void StopFire();
	void Reload();
	void NextWeapon();
	void EquipWeapon(int32 Index);

	UPROPERTY() TArray<TObjectPtr<AWeaponBase>> Weapons;
	UPROPERTY() TObjectPtr<AWeaponBase> CurrentWeapon;
	int32 CurrentWeaponIndex = INDEX_NONE;
};

