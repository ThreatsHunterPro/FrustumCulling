#include "PlayerCharacter.h"

#include "FrustumChecker.h"
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

	checker = CreateDefaultSubobject<UFrustumChecker>(TEXT("FrustumChecker"));
	AddOwnedComponent(movement);
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	InputComponent->BindAxis(FName("Vertical"), movement, &UPlayerMovementComponent::MoveVertical);
	InputComponent->BindAction(FName("Sprint"), IE_Pressed, movement, &UPlayerMovementComponent::ToggleSprint);
	InputComponent->BindAxis(FName("Yaw"), movement, &UPlayerMovementComponent::MoveYaw);
	InputComponent->BindAxis(FName("Pitch"), movement, &UPlayerMovementComponent::MovePitch);
}