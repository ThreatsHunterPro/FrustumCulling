// ReSharper disable All
#include "PlayerMovementComponent.h"

UPlayerMovementComponent::UPlayerMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UPlayerMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	location = GetOwner()->GetActorLocation();
	rotation = GetOwner()->GetActorRotation();
}

void UPlayerMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	GetOwner()->SetActorLocationAndRotation(location, rotation);
}

void UPlayerMovementComponent::MoveVertical(float _value)
{
	if (!bCanMove || _value == 0.0f) return;
	
	const float _factor = _value * fMoveSpeed * GetWorld()->DeltaTimeSeconds * 50.0f;
	location += GetOwner()->GetActorForwardVector() * _factor;
	onMoveVertical.Broadcast(_factor);
}

void UPlayerMovementComponent::MoveHorizontal(float _value)
{
	if (!bCanMove || _value == 0.0f) return;
	
	location += GetOwner()->GetActorRightVector() * _value * fMoveSpeed * GetWorld()->DeltaTimeSeconds * 50.0f;
}

void UPlayerMovementComponent::MoveYaw(float _value)
{
	if (!bCanRotate || _value == 0.0f) return;
	rotation.Yaw += _value * fRotateSpeed / 5.0f;
}

void UPlayerMovementComponent::MovePitch(float _value)
{
	if (!bCanRotate || _value == 0.0f) return;
	rotation.Pitch += _value * fRotateSpeed / 5.0f;
	rotation.Pitch = rotation.Pitch > fPitchMax ? fPitchMax
				   : rotation.Pitch < fPitchMin ? fPitchMin
				   : rotation.Pitch;
}