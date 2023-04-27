#include "PlayerAnimInstance.h"
#include "PlayerMovementComponent.h"

void UPlayerAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	
	if (const APlayerCharacter* _player = Cast<APlayerCharacter>(GetOwningActor()))
	{
		InitEvents(_player);
	}
}

void UPlayerAnimInstance::InitEvents(const APlayerCharacter* _player)
{
	if (UPlayerMovementComponent* _movement = _player->GetPlayerMovement())
	{
		_movement->OnMoveVertical().AddDynamic(this, &UPlayerAnimInstance::UpdateVerticalMovement);
	}
}

void UPlayerAnimInstance::UpdateVerticalMovement(float _verticalValue)
{
	fVerticalValue = _verticalValue;
}
