#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CultPlayerCharacter.generated.h"

UCLASS()
class ACultPlayerCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    ACultPlayerCharacter();

protected:
    virtual void BeginPlay() override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    void MoveForward(float Value);
    void MoveRight(float Value);
    void MeleeAttack();
    void DoMeleeSweep();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    class USpringArmComponent* CameraBoom;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    class UCameraComponent* TopDownCamera;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
    float MeleeRange = 200.f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
    float MeleeRadius = 100.f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
    float MeleeDamage = 25.f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
    TSubclassOf<UDamageType> DamageTypeClass;

    FTimerHandle MeleeTimerHandle;
    bool bCanAttack = true;

    UPROPERTY(EditAnywhere, Category="Combat")
    float AttackCooldown = 0.5f;
};
