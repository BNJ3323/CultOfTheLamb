#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyCharacter.generated.h"

UCLASS()
class AEnemyCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    AEnemyCharacter();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaSeconds) override;

    UPROPERTY(EditAnywhere, Category="AI")
    float AggroRange = 800.f;

    UPROPERTY(EditAnywhere, Category="Combat")
    float Damage = 10.f;

    AActor* TargetPlayer;
    void TryChasePlayer();

    FTimerHandle AttackTimer;
    void AttackPlayer();
    bool bCanAttack = true;
    UPROPERTY(EditAnywhere, Category="Combat")
    float AttackCooldown = 1.0f;
};
