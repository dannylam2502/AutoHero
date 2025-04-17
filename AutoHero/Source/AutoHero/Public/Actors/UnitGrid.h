// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameEnums.h"
#include "GameFramework/Actor.h"
#include "UnitGrid.generated.h"

UCLASS()
class AUTOHERO_API AUnitGrid : public AActor
{
	GENERATED_BODY()

public:
	// Start Location, Top-Left
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	FVector StartLocation;

	// Size of each cell
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	FVector CellSize;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	FVector CellOffset;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	int32 Rows;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	int32 Columns;

	// Sets default values for this actor's properties
	AUnitGrid();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Grid")
	TSubclassOf<class AUnitCell> GridCellClass;

	UFUNCTION(BlueprintCallable, Category = "Grid")
	void InitializeGrid();

	UFUNCTION(BlueprintCallable, Category = "Grid")
	FVector GetGridCellLocation(int32 Row, int32 Column);

	UFUNCTION(BlueprintCallable, Category = "Grid")
	AUnitCell* GetNearestCell();

	UFUNCTION(BlueprintCallable, Category = "Grid")
	FVector GetNearestCellLocation(const FVector& WorldPosition);

	UFUNCTION(BlueprintCallable, Category = "Grid")
	void HighlightNearestCell(EActorTeam Team, const FVector& WorldPosition);

	bool IsCellOccupied(AUnitCell* UnitCell) const;
	void OccupyCell(AUnitCell* UnitCell, class ABaseUnit* Unit);
	void VacateCell(AUnitCell* UnitCell);
	class ABaseUnit* GetUnitInCell(AUnitCell* Cell);
	// Hide random cells to sastify design formation team
	void HideRandomCells(int num);

	UFUNCTION()
	void OnUnitRemovedFromField(ABaseUnit* Unit);

	void PlaceUnitOnCellLocally(ABaseUnit* BaseUnit);

	// Need to construct data as: UnitID, In CellID?
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
private:
	UPROPERTY(Replicated)
	TArray<AUnitCell*> TopGridCells;
	UPROPERTY(Replicated)
	TArray<AUnitCell*> BottomGridCells;
	UPROPERTY(VisibleAnywhere)
	AUnitCell* LastHighlightedCell;
	TMap<AUnitCell*, class ABaseUnit*> OccupiedCells;

	UFUNCTION()
	void OnClientUnitDropped(ABaseUnit* BaseUnit, FVector2D UnitLocation);
	UFUNCTION()
	void OnClientUnitDragging(ABaseUnit* BaseUnit);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
