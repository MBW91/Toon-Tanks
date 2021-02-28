#include "HealthComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ToonTanks/GameModes/TankGameModeBase.h"

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	Health = DefaultHealth;
	GameMode = Cast<ATankGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::TakeDamage);
}

void UHealthComponent::TakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage == 0 ||Health <= 0) return;

	Health = FMath::Clamp(Health - Damage, 0.f, DefaultHealth);
	if (Health <= 0.f)
	{
		if (!GameMode)
		{
			UE_LOG(LogTemp, Error, TEXT("Health Component has no reference to the GameMode!"));
			return;
		}

		GameMode->ActorDied(GetOwner());
	}
}