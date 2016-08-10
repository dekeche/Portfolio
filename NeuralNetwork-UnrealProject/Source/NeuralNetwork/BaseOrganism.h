#pragma once

#include "GameFramework/Actor.h"
#include "BaseOrganism.generated.h"

USTRUCT()
struct FGene
{

	GENERATED_USTRUCT_BODY()
		float incoming;
	float calmDown;
	float outgoing;
	bool negative;
};
USTRUCT()
struct FGenome
{
	GENERATED_USTRUCT_BODY()

		FGene EyeRightHerb;
	FGene EyeLeftHerb;
	FGene EyeUpHerb;

	FGene EyeRightCarn;
	FGene EyeLeftCarn;
	FGene EyeUpCarn;

	FGene EyeRightPlant;
	FGene EyeLeftPlant;
	FGene EyeUpPlant;


	FGene HiddenRight;
	FGene HiddenLeft;
	FGene HiddenUp;
	FGene HiddenDown;

	FGene MoveRight;
	FGene MoveLeft;
	FGene MoveUp;
};
	UENUM(BlueprintType)
enum class EOrganismType : uint8
{
	BO_PLANT = 0,
	BO_HERB,
	BO_CARN
};

UCLASS()
class NEURALNETWORK_API ABaseOrganism : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABaseOrganism();

	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	EOrganismType GetType() { return type; };

	float GetEnergy() { return EnergyCurrent; };
	void AddEnergy(float add) { EnergyCurrent += add; };


	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Node")
		EOrganismType type;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Node")
		float EnergyMax;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Node")
		float EnergyDrain;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "InteractableObject")
	USceneComponent* SceneRoot;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "InteractableObject")
	UStaticMeshComponent* Mesh;

	UFUNCTION()
	void OnHit(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	virtual void DoOnHit(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

protected:
	UPROPERTY()
		float EnergyCurrent;
};