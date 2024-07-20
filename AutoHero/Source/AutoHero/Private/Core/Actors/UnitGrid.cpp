// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Actors/UnitGrid.h"

#include "Core/Actors/UnitCell.h"


// Sets default values
AUnitGrid::AUnitGrid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	LastHighlightedCell = nullptr;
}

// Called when the game starts or when spawned
void AUnitGrid::BeginPlay()
{
	Super::BeginPlay();
	InitializeGrid();
}

// Called every frame
void AUnitGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Rows = 5;
	Columns = 4;
}

void AUnitGrid::InitializeGrid()
{
	if (!GridCellClass) return;

	for (int32 Row = 0; Row < Rows; ++Row)
	{
		for (int32 Column = 0; Column < Columns; ++Column)
		{
			FVector Location = GetGridCellLocation(Row, Column);
			AUnitCell* NewCell = GetWorld()->SpawnActor<AUnitCell>(GridCellClass, Location, FRotator::ZeroRotator);
			if (NewCell)
			{
				NewCell->InitializeCell(Location, Row, Column);
				GridCells.Add(NewCell);
			}
		}
	}
}

FVector AUnitGrid::GetGridCellLocation(int32 Row, int32 Column)
{
	return StartLocation + FVector(Row * CellSize.X, Column * CellSize.Y, 0.0f);
}

AUnitCell* AUnitGrid::GetNearestCell()
{
	return LastHighlightedCell;
}

FVector AUnitGrid::GetNearestCellLocation(const FVector& WorldPosition)
{
	if (LastHighlightedCell)
	{
		return LastHighlightedCell->GetCellCenterLocation();
	}
	FVector LocalPosition = WorldPosition - StartLocation;
	int32 CellX = FMath::RoundToInt(LocalPosition.X / CellSize.X);
	int32 CellY = FMath::RoundToInt(LocalPosition.Y / CellSize.Y);
	int32 CellZ = FMath::RoundToInt(LocalPosition.Z / CellSize.Z);
	
	return StartLocation + FVector(CellX * CellSize.X, CellY * CellSize.Y, CellZ * CellSize.Z);
}

void AUnitGrid::HighlightNearestCell(const FVector& WorldPosition)
{
	AUnitCell* NearestCell = nullptr;
	float MinDistance = FLT_MAX;

	for (AActor* Actor : this->GridCells)
	{
		AUnitCell* Cell = Cast<AUnitCell>(Actor);
		if (Cell && !IsCellOccupied(Cell))
		{
			float Distance = FVector::Dist(WorldPosition, Cell->GetCellCenterLocation());
			if (Distance < MinDistance)
			{
				MinDistance = Distance;
				NearestCell = Cell;
			}
		}
	}

	if (NearestCell)
	{
		if (LastHighlightedCell && LastHighlightedCell != NearestCell)
		{
			LastHighlightedCell->HighlightCell(false);
		}

		NearestCell->HighlightCell(true);
		LastHighlightedCell = NearestCell;
	}
}

bool AUnitGrid::IsCellOccupied(AUnitCell* UnitCell) const
{
	return OccupiedCells.Contains(UnitCell);
}

void AUnitGrid::OccupyCell(AUnitCell* UnitCell)
{
	OccupiedCells.Add(UnitCell);
}

void AUnitGrid::VacateCell(AUnitCell* UnitCell)
{
	OccupiedCells.Remove(UnitCell);
}