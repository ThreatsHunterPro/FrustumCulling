#include "PlayerPawn.h"

#include "Kismet/KismetMathLibrary.h"

APlayerPawn::APlayerPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	capsule->SetupAttachment(RootComponent);
	
	skeletal = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	skeletal->SetupAttachment(capsule);
	
	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent")),
	springArm->SetupAttachment(RootComponent);
	
	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	camera->SetupAttachment(springArm);
}

void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetFirstPlayerController()->SetViewTargetWithBlend(this);
}

void APlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	InputComponent->BindAxis(FName("Vertical"), this, &APlayerPawn::MoveVertical);
	InputComponent->BindAxis(FName("Horizontal"), this, &APlayerPawn::MoveHorizontal);
}

void APlayerPawn::MoveVertical(float _value)
{
	if (!bCanMove || _value == 0.0f) return;
	const FVector& _targetLocation = UKismetMathLibrary::VInterpTo(GetActorLocation(), GetActorLocation() + GetActorForwardVector(), GetWorld()->DeltaTimeSeconds, fMoveSpeed);
	SetActorLocation(_targetLocation);
}

void APlayerPawn::MoveHorizontal(float _value)
{
	if (!bCanMove || _value == 0.0f) return;
	const FVector& _targetLocation = UKismetMathLibrary::VInterpTo(GetActorLocation(), GetActorLocation() + GetActorRightVector(), GetWorld()->DeltaTimeSeconds, fMoveSpeed);
	SetActorLocation(_targetLocation);
}

void APlayerPawn::MoveYaw(float _value)
{
	
}

void APlayerPawn::MovePitch(float _value)
{
	// if (!bCanRotate || _value == 0.0f) return;
	// const FRotator& _lookAt = UKismetMathLibrary::FindLookAtRotation()
	// const FRotator& _targetRotation = UKismetMathLibrary::RInterpTo(GetActorRotation(), GetActorUpVector(), GetWorld()->DeltaTimeSeconds, fRotateSpeed);
}
