// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/PlaceholderUnit.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Actors/UnitGrid.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
APlaceholderUnit::APlaceholderUnit()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	UnitMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("UnitMesh"));
	RootComponent = UnitMesh;
}

void APlaceholderUnit::SetUnitPosition(FVector NewPosition)
{
	TargetPosition = NewPosition;
}

// Called every frame
void APlaceholderUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	FVector2D MousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());

	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, FString::Printf(TEXT("Mouse X = %f"), MousePosition.X));
	FVector WorldLocation, WorldDirection;
	PlayerController->DeprojectScreenPositionToWorld(MousePosition.X, MousePosition.Y, WorldLocation, WorldDirection);
	FVector End = WorldLocation + (WorldDirection * 10000.0f);
	FHitResult HitResult;
	if (GetWorld()->LineTraceSingleByChannel(HitResult, WorldLocation, End, ECC_Visibility))
	{
		SetActorLocation(HitResult.Location);
		AUnitGrid* UnitGrid = Cast<AUnitGrid>(UGameplayStatics::GetActorOfClass(GetWorld(), AUnitGrid::StaticClass()));
		// Highlight the nearest cell in the grid manager
		if (UnitGrid)
		{
			UnitGrid->HighlightNearestCell(GetActorLocation());
		}
	}
}

