#include "CultPlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "DrawDebugHelpers.h"

ACultPlayerCharacter::ACultPlayerCharacter()
{
    PrimaryActorTick.bCanEverTick = false;

    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 1200.f;
    CameraBoom->bDoCollisionTest = false;
    CameraBoom->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f));

    TopDownCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
    TopDownCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    TopDownCamera->bUsePawnControlRotation = false;

    GetCharacterMovement()->bOrientRotationToMovement = true;
    bUseControllerRotationYaw = false;
    GetCharacterMovement()->MaxWalkSpeed = 600.f;
}

void ACultPlayerCharacter::BeginPlay()
{
    Super::BeginPlay();
}

void ACultPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    check(PlayerInputComponent);

    PlayerInputComponent->BindAxis("MoveForward", this, &ACultPlayerCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &ACultPlayerCharacter::MoveRight);
    PlayerInputComponent->BindAction("Melee", IE_Pressed, this, &ACultPlayerCharacter::MeleeAttack);
}

void ACultPlayerCharacter::MoveForward(float Value)
{
    if (Controller && Value != 0.0f)
    {
        AddMovementInput(FVector::ForwardVector, Value);
    }
}

void ACultPlayerCharacter::MoveRight(float Value)
{
    if (Controller && Value != 0.0f)
    {
        AddMovementInput(FVector::RightVector, Value);
    }
}

void ACultPlayerCharacter::MeleeAttack()
{
    if (!bCanAttack) return;
    bCanAttack = false;
    DoMeleeSweep();

    GetWorldTimerManager().SetTimer(MeleeTimerHandle, [this]() { bCanAttack = true; }, AttackCooldown, false);
}

void ACultPlayerCharacter::DoMeleeSweep()
{
    FVector Start = GetActorLocation();
    FVector Center = Start + GetActorForwardVector() * (MeleeRange * 0.5f);

    TArray<FOverlapResult> Overlaps;
    FCollisionShape Shape = FCollisionShape::MakeSphere(MeleeRadius);
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);

    bool bHit = GetWorld()->OverlapMultiByChannel(
        Overlaps,
        Center,
        FQuat::Identity,
        ECollisionChannel::ECC_Pawn,
        Shape,
        Params
    );

    #if ENABLE_DRAW_DEBUG
    DrawDebugSphere(GetWorld(), Center, MeleeRadius, 12, FColor::Red, false, 0.8f);
    #endif

    if (bHit)
    {
        for (auto& R : Overlaps)
        {
            AActor* Other = R.GetActor();
            if (Other && Other != this)
            {
                UGameplayStatics::ApplyDamage(Other, MeleeDamage, GetController(), this, DamageTypeClass ? DamageTypeClass : UDamageType::StaticClass());
            }
        }
    }
}
