// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Actors/UnitCell.h"


// Sets default values
AUnitCell::AUnitCell()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AUnitCell::BeginPlay()
{
	Super::BeginPlay();
	
}

void AUnitCell::InitializeCell(FVector Location, int32 Row, int32 Column)
{
	SetActorLocation(Location);
	CellRow = Row;
	CellColumn = Column;
}

void AUnitCell::HighlightCell(bool bHighlight)
{
	
}

// Called every frame
void AUnitCell::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

