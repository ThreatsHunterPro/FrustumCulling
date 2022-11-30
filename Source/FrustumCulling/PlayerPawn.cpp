#include "PlayerPawn.h"

APlayerPawn::APlayerPawn()
{
	PrimaryActorTick.bCanEverTick = true;
}

void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();
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
	
}
void APlayerPawn::MoveHorizontal(float _value)
{
	
}