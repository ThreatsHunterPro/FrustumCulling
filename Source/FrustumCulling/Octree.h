// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Octree.generated.h"

UCLASS()
class FRUSTUMCULLING_API AOctree : public AActor
{
	GENERATED_BODY()

	#pragma region Values

	/* Status of hiding */
	UPROPERTY(VisibleAnywhere, Category = "Octree values | Datas")
		bool bIsHidingActors = false;

	/* Status of debugs */
	UPROPERTY(EditAnywhere, Category = "Octree values | Debug")
		bool bShowDebug = true;

	/* Thickness of the box's lines when debugs are enabled */
	UPROPERTY(EditAnywhere, Category = "Octree values | Debug")
		float fThickness = 200.0f;

	/* Precision of this octree */
	UPROPERTY(VisibleAnywhere, Category = "Octree values | Datas")
		int iAccuracy = 1;

	/* Capacity max this octree can contains */
	UPROPERTY(EditAnywhere, Category = "Octree values | Datas", meta = (UIMin = "0", UIMax = "10000", ClampMin = "0", ClampMax = "10000"))
		int iCapacity = 50;

	/* Box color when debugs are enabled */
	UPROPERTY(EditAnywhere, Category = "Octree values | Debug")
		FColor lineColor = FColor::Cyan;

	/* Box color when debugs are enabled */
	UPROPERTY(EditAnywhere, Category = "Octree values | Debug")
		FColor debugColor = FColor::Green;

	/* Box scale */
	UPROPERTY(EditAnywhere, Category = "Octree values | Datas")
		FVector boxScale = FVector(100.0f);
	
	/* Box container */
	UPROPERTY(VisibleAnywhere, Category = "Octree values | Datas")
		FBox box = FBox();

	/* Type to octree to spawn */
	UPROPERTY(EditAnywhere, Category = "Octree values | Datas")
		TSubclassOf<AOctree> octreeType = TSubclassOf<AOctree>();

	/* All actors to ignore */
	UPROPERTY(EditAnywhere, Category = "Octree values | Datas")
		TArray<TSoftObjectPtr<AActor>> actorsToIgnore = TArray<TSoftObjectPtr<AActor>>();

	/* List of all actors contains */
	UPROPERTY(VisibleAnywhere, Category = "Octree values | Datas")
		TArray<TSoftObjectPtr<AActor>> actors = TArray<TSoftObjectPtr<AActor>>();

	#pragma endregion

	#pragma region Pointers

	/* Master of all octrees */
	UPROPERTY()
		AOctree* master = nullptr;

	/* List of all children */
	UPROPERTY()
		TArray<AOctree*> octreeChildren = TArray<AOctree*>();

	/* List of all octree without children */
	UPROPERTY()
		TArray<AOctree*> octreesWithoutChildren = TArray<AOctree*>();

	#pragma endregion 
	
public:
	#pragma region Setters

	/* Initialize a child */
	FORCEINLINE void Setup(const int& _accuracy, const FBox& _boundingBox, AOctree* _master, const TArray<TSoftObjectPtr<AActor>>& _actors)
	{
		iAccuracy = _accuracy;
		box = _boundingBox;
		master = _master;
		actors = _actors;
	}

	/* Store a new octree without children */
	FORCEINLINE void AddOctreeWithoutChildren(AOctree* _octree) { octreesWithoutChildren.Add(_octree); }

	#pragma endregion

	#pragma region Getters

	/* Ask if this octree has children */
	FORCEINLINE bool HasChildren() const { return octreeChildren.Num() > 0; }
	
	/* Get box of this octree */
	FORCEINLINE const FBox& GetBox() const { return box; }

	/* Get all children of this octree */
	FORCEINLINE const TArray<AOctree*>& GetChildren() const { return octreeChildren; }

	/* Get all octrees without children */
	FORCEINLINE const TArray<AOctree*>& GetOctreesWithoutChildren() const { return octreesWithoutChildren; }

	#pragma endregion

public:
	AOctree();

	#pragma region Methods

	#pragma region Engine
	
private:
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual bool ShouldTickIfViewportsOnly() const override;

	#pragma endregion 

	#pragma region Tool

	/* Update actors and possible children octree */
	UFUNCTION(CallInEditor, Category = "Octree actions") void Update();

	/* Unshow for each octree debugs */
	UFUNCTION(CallInEditor, Category = "Octree actions") void SwitchVisibility();

	/* Reset all actors (= only for the main) and children */
	UFUNCTION(CallInEditor, Category = "Octree actions") void Clear();

	#pragma endregion

	/* Draw current octree */
	void DrawOctrees() const;

	/* Get all actors which is into the octree */
	void GetActors();

	/* Spawn and setup 8 children */
	void SpawnChildren();
	
	/* Destroy all children */
	void ResetOctree();
	
public:
	/* Set the visibility status for all actors */
	void ChangeActorsVisibility(bool _visible);

	#pragma endregion 
};