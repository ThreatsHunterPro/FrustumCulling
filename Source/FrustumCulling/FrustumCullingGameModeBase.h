#pragma once
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FrustumCullingGameModeBase.generated.h"

class AOctree;

UCLASS()
class FRUSTUMCULLING_API AFrustumCullingGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnOctreeSetted, AOctree*, octree);
	
	UPROPERTY(EditAnywhere, Category = "GameMode values")
		AOctree* octree = nullptr;

	UPROPERTY()
		FOnOctreeSetted onOctreeSetted = FOnOctreeSetted();

public:
	#pragma region Getters
	
	UFUNCTION(BlueprintCallable) FORCEINLINE AOctree* GetOctree() const { return octree; }
	FORCEINLINE FOnOctreeSetted* OnOctreeSetted() { return &onOctreeSetted; }
	
	#pragma endregion

	#pragma region Setters
	
	FORCEINLINE void SetOctree(AOctree* _octree)
	{
		octree = _octree;
		onOctreeSetted.Broadcast(octree);
	}

	#pragma  endregion
};