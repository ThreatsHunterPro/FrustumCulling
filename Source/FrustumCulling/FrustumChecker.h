#pragma once
#include "CoreMinimal.h"

#include "Octree.h"
#include "Camera/CameraComponent.h"

#include "Components/ActorComponent.h"
#include "FrustumChecker.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FRUSTUMCULLING_API UFrustumChecker : public UActorComponent
{
	GENERATED_BODY()

	#pragma region Values

	/* Rate of the detection */
	UPROPERTY(EditAnywhere, Category = "Frutum checker values", meta = (UIMin = "0.1", UIMax = "60.0", ClampMin = "0.1", ClampMax = "60.0"))
		float fCheckRate = 0.1f;

	/* Range of checker detection */
	UPROPERTY(EditAnywhere, Category = "Frutum checker values", meta = (UIMin = "0.0", UIMax = "100000.0", ClampMin = "0.0", ClampMax = "100000.0"))
		float fRange = 500.0f;

	#pragma region

	#pragma region Pointers

	/* Current world */
	UPROPERTY()
		UWorld* world = nullptr;

	/* Current octree */
	UPROPERTY()
		AOctree* octree = nullptr;

	/* Current local player */
	UPROPERTY()
		ULocalPlayer* localPlayer = nullptr;

	/* Current viewportClient */
	UPROPERTY()
		TObjectPtr<UGameViewportClient> viewportClient = nullptr;

	/* Current viewport */
		FViewport* viewport = nullptr;

	#pragma endregion

public:	
	UFrustumChecker();

	#pragma region Engine
	
private:
	virtual void BeginPlay() override;

	#pragma endregion

	/* When the octree is setted, start a timer for checking method */
	UFUNCTION(BlueprintCallable) void InitTimer(AOctree* _octree);

	/* Check if the octree and its children is into the frustum */
	UFUNCTION(BlueprintCallable) void CheckOctree();

	/* Check if the current octree is into the camera's frustum */
	bool IsInFrustum(const AOctree* _target) const;	
};