#include "EnemyCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"

AEnemyCharacter::AEnemyCharacter()
{
    PrimaryActorTick.bCanEverTick = true;
    GetCharacterMovement()->MaxWalkSpeed = 350.f;
}

void AEnemyCharacter::BeginPlay()
{
    Super::BeginPlay();
    TargetPlayer = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
}

void AEnemyCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    TryChasePlayer();
}

void AEnemyCharacter::TryChasePlayer()
{
    if (!TargetPlayer) return;

    float Dist = FVector::Dist(GetActorLocation(), TargetPlayer->GetActorLocation());
    if (Dist <= AggroRange)
    {
        // simple move to player
        FVector Dir = (TargetPlayer->GetActorLocation() - GetActorLocation()).GetSafeNormal2D();
        AddMovementInput(Dir, 1.0f);

        if (Dist <= 150.f && bCanAttack)
        {
            AttackPlayer();
        }
    }
}

void AEnemyCharacter::AttackPlayer()
{
    bCanAttack = false;
    // apply damage directly
    UGameplayStatics::ApplyDamage(TargetPlayer, Damage, GetController(), this, nullptr);
    GetWorldTimerManager().SetTimer(AttackTimer, [this]() { bCanAttack = true; }, AttackCooldown, false);
}
