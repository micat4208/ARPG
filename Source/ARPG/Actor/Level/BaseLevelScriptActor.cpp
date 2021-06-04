#include "BaseLevelScriptActor.h"

#include "Actor/PoolableParticle/PoolableParticle.h"

ABaseLevelScriptActor::ABaseLevelScriptActor()
{
	PoolableParticleClass = APoolableParticle::StaticClass();
}

void ABaseLevelScriptActor::BeginPlay()
{
	Super::BeginPlay();
	ParticlePool = NewObject<UObjectPool>();
}

APoolableParticle* ABaseLevelScriptActor::CreatePoolableParticleActor(
	UParticleSystem* particleTemplate, FVector location, FRotator rotation)
{
	APoolableParticle* poolableParticle = ParticlePool->GetRecycledObject<APoolableParticle>();

	poolableParticle = (poolableParticle != nullptr) ? 
		poolableParticle :
		GetWorld()->SpawnActor<APoolableParticle>(
		PoolableParticleClass);

	poolableParticle->SetActorLocationAndRotation(location, rotation);

	ParticlePool->RegisterRecyclableObject(poolableParticle);

	poolableParticle->GetParticleSystem()->SetTemplate(particleTemplate);
	poolableParticle->GetParticleSystem()->Activate(true);

	return poolableParticle;
}
