#include "PlayerCharacter.h"
#include "Kismet/KismetMathLibrary.h"

#include "PlayerMovementComponent.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent")),
	springArm->SetupAttachment(RootComponent);
	
	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	camera->SetupAttachment(springArm);

	movement = CreateDefaultSubobject<UPlayerMovementComponent>(TEXT("PlayerMovementComponent"));
	AddOwnedComponent(movement);
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	InputComponent->BindAxis(FName("Vertical"), movement, &UPlayerMovementComponent::MoveVertical);
	InputComponent->BindAxis(FName("Horizontal"), movement, &UPlayerMovementComponent::MoveHorizontal);
	InputComponent->BindAxis(FName("Yaw"), movement, &UPlayerMovementComponent::MoveYaw);
	InputComponent->BindAxis(FName("Pitch"), movement, &UPlayerMovementComponent::MovePitch);
}