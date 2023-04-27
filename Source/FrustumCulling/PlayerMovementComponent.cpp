#include "PlayerMovementComponent.h"

#include "PlayerCharacter.h"
#include "Kismet/KismetMathLibrary.h"

class UCameraComponent;

UPlayerMovementComponent::UPlayerMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UPlayerMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	owner = GetOwner();
	
	if (owner)
	{
		const APlayerCharacter* _player = Cast<APlayerCharacter>(owner);
		if (!_player) return;
	
		camera = _player->GetCamera();
		if (camera)
		{
			camera->FieldOfView = fDefaultFOV;
		}
	}
}

void UPlayerMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	const float _additionalSpeed = fFOVSpeed * 50.0f / 100.0f;
	camera->FieldOfView = FMath::Lerp(camera->FieldOfView, fDefaultFOV, (fFOVSpeed + _additionalSpeed) * DeltaTime / 10.0f);
}


void UPlayerMovementComponent::MoveVertical(float _value)
{
	if (!bCanMove || !owner) return;
	
	if (_value <= 0.0f)
	{
		onMoveVertical.Broadcast(0.0f);
		return;
	}
	
	const float _speed = bIsSprinting ? fSprintSpeed : fMoveSpeed;
	const FVector& _ownerLocation = owner->GetActorLocation();
	const FVector& _targetLocation = _ownerLocation + owner->GetActorForwardVector() * _value * 50.0f;
	const FVector& _newLocation = UKismetMathLibrary::VInterpTo(_ownerLocation, _targetLocation, GetWorld()->DeltaTimeSeconds, _speed);
	owner->SetActorLocation(_newLocation);
	onMoveVertical.Broadcast(bIsSprinting ? _value : _value / 2);
	
	const float _targetFOV = bIsSprinting ? fSprintFOV : fDefaultFOV;
	camera->FieldOfView = FMath::Lerp(camera->FieldOfView, _targetFOV, fFOVSpeed * GetWorld()->DeltaTimeSeconds);
}

void UPlayerMovementComponent::ToggleSprint()
{
	bIsSprinting = !bIsSprinting;
}

void UPlayerMovementComponent::MoveYaw(float _value)
{
	if (!bCanRotate || !owner || _value == 0.0f) return;
	owner->SetActorRotation(owner->GetActorRotation() + FRotator(0.0f, _value * fRotateSpeed / 5.0f, 0.0f));
}

void UPlayerMovementComponent::MovePitch(float _value)
{
	if (!bCanRotate || !owner || !camera || _value == 0.0f) return;
	
	const float _previousPitch = camera->GetRelativeRotation().Pitch;
	const float _newPitch = _previousPitch + _value * fRotateSpeed / 5.0f;
	const float _clampedPitch = _newPitch > fPitchMax ? fPitchMax
							  : _newPitch < fPitchMin ? fPitchMin
							  : _newPitch;
	camera->SetRelativeRotation(FRotator(_clampedPitch, 0.0f, 0.0f));
}