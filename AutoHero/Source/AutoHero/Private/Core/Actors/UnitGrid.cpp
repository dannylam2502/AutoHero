// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Actors/UnitGrid.h"

#include "Core/Actors/BaseUnit.h"
#include "Core/Actors/UnitCell.h"
#include "Kismet/KismetMathLibrary.h"


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
	BottomGridCells.SetNum(Rows);
	TopGridCells.SetNum(Rows);
	for (int32 Row = 0; Row < Rows; ++Row)
	{
		BottomGridCells[Row].SetNum(Columns);
		TopGridCells[Row].SetNum(Columns);
	}
	
	for (int32 Row = 0; Row < Rows; ++Row)
	{
		for (int32 Column = 0; Column < Columns; ++Column)
		{
			FVector Location = GetGridCellLocation(Row, Column);
			AUnitCell* NewBottomCell = GetWorld()->SpawnActor<AUnitCell>(GridCellClass, Location, FRotator::ZeroRotator);
			if (NewBottomCell)
			{
				NewBottomCell->InitializeCell(Location, Row, Column);
				BottomGridCells[Row][Column] = NewBottomCell;
			}
			Location.Y = -Location.Y;
			AUnitCell* NewTopCell = GetWorld()->SpawnActor<AUnitCell>(GridCellClass, Location, FRotator::ZeroRotator);
			if (NewTopCell)
			{
				NewTopCell->InitializeCell(Location, Row, Column);
				TopGridCells[Row][Column] = NewTopCell;
			}
		}
	}
	
	// TODO: configurable
	HideRandomCells(2);
}

FVector AUnitGrid::GetGridCellLocation(int32 Row, int32 Column)
{
	return StartLocation + FVector(Column * (CellOffset.X + CellSize.X), Row * (CellOffset.Y + CellSize.Y), 0.0f);
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

	for (int32 Row = 0; Row < BottomGridCells.Num(); ++Row)
	{
		for (int32 Column = 0; Column < BottomGridCells[Row].Num(); ++Column)
		{
			AUnitCell* Cell = BottomGridCells[Row][Column];
			if (Cell && !Cell->IsHidden())
			{
				float Distance = FVector::Dist(WorldPosition, Cell->GetCellCenterLocation());
				if (Distance < MinDistance)
				{
					MinDistance = Distance;
					NearestCell = Cell;
				}
			}
		}
	}
	
	if (NearestCell)
	{
		if (LastHighlightedCell && LastHighlightedCell != NearestCell)
		{
			LastHighlightedCell->HighlightCell(false);
			NearestCell->HighlightCell(true);
		}

		LastHighlightedCell = NearestCell;
	}
}

bool AUnitGrid::IsCellOccupied(AUnitCell* UnitCell) const
{
	return OccupiedCells.Contains(UnitCell) || UnitCell->IsHidden();
}

void AUnitGrid::OccupyCell(AUnitCell* UnitCell, ABaseUnit* Unit)
{
	OccupiedCells.Add(UnitCell, Unit);
}

void AUnitGrid::VacateCell(AUnitCell* UnitCell)
{
	OccupiedCells.Remove(UnitCell);
}

ABaseUnit* AUnitGrid::GetUnitInCell(AUnitCell* Cell)
{
	return *OccupiedCells.Find(Cell);
}

void AUnitGrid::HideRandomCells(int32 num)
{
	// Calculate total cells (assuming a 2D grid)
	int32 TotalCells = Rows * Columns;

	// Set to track generated random numbers
	TSet<int32> GeneratedRandomNum;

	for (int32 i = 0; i < num; ++i)
	{
		int32 randomIndex;
		do 
		{
			// Generate a unique random index
			randomIndex = UKismetMathLibrary::RandomIntegerInRange(0, TotalCells - 1);
		}
		while (GeneratedRandomNum.Contains(randomIndex));  // Ensure it hasn't been used before

		// Add the unique index to the set
		GeneratedRandomNum.Add(randomIndex);

		// Convert the 1D random index to a 2D grid position (row, column)
		int32 Row = randomIndex / Columns;
		int32 Column = randomIndex % Columns;

		// Hide the corresponding cell in the BottomGridCells
		if (BottomGridCells.IsValidIndex(Row) && BottomGridCells[Row].IsValidIndex(Column))
		{
			BottomGridCells[Row][Column]->SetActorHiddenInGame(true);
		}

		// Optionally hide the corresponding cell in TopGridCells (mirrored position)
		if (TopGridCells.IsValidIndex(Row) && TopGridCells[Row].IsValidIndex(Column))
		{
			TopGridCells[Row][Columns - 1 - Column]->SetActorHiddenInGame(true);
		}
	}
}

void AUnitGrid::OnUnitRemovedFromField(ABaseUnit* Unit)
{
	AUnitCell* CurCell = Unit->GetCurrentCell();
	if (CurCell)
	{
		CurCell->SelectCell(false);
		CurCell->HighlightCell(false);
		VacateCell(CurCell);
		if (LastHighlightedCell)
		{
			LastHighlightedCell->HighlightCell(false);
		}
	}
}
