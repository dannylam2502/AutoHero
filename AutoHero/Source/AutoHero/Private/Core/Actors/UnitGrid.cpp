// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Actors/UnitGrid.h"

#include "Core/Actors/UnitCell.h"


// Sets default values
AUnitGrid::AUnitGrid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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

