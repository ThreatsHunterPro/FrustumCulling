#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Camera.generated.h"

UCLASS()
class FRUSTUMCULLING_API ACamera : public AActor
{
	GENERATED_BODY()
	
public:	
	ACamera();

private:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};