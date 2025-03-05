#include "GameInstances/MBGameInstance.h"
#include "MBSettings.h"
#include "Kismet/GameplayStatics.h"

UMBGameInstance::UMBGameInstance()
{
	InitData();

	static ConstructorHelpers::FObjectFinder<USoundCue> SoundFinder(TEXT("/Script/Engine.SoundCue'/Game/LHW/mp3/SoundCue/walk.walk'"));
	static ConstructorHelpers::FObjectFinder<USoundAttenuation> soundSettingFinder(TEXT("/Script/Engine.SoundAttenuation'/Game/LHW/mp3/SoundCue/AT.AT'"));
	if (SoundFinder.Succeeded())
	{
		AttenuationSettings = soundSettingFinder.Object;
		foodSound = SoundFinder.Object;
	}
}

void UMBGameInstance::InitData()
{
	PlayerTroopCount = Data::AI::Testing::PlayerTroopTestCount;
	CurrentBattleEnemyTroopCount = Data::AI::Testing::EnemyTroopTestCount;
}

void UMBGameInstance::AnimNotify_footSound()
{
	UE_LOG(LogTemp, Log, TEXT("asdasd"));
	UGameplayStatics::PlaySoundAtLocation(this, foodSound, GetAnimationActorLocation(), 1.0f, 1.0f, 0.0f, AttenuationSettings);

}

FVector UMBGameInstance::GetAnimationActorLocation() const
{
	if (const UAnimInstance* AnimInstance = Cast<UAnimInstance>(this))
	{
		if (const AActor* OwningActor = AnimInstance->GetOwningActor())
		{
			return OwningActor->GetActorLocation();
		}
	}

	return FVector::ZeroVector;
}

void UMBGameInstance::LoadBattleData(int& InPlayerData, int& InEnemyData)
{
	InPlayerData = PlayerTroopCount;
	InEnemyData = CurrentBattleEnemyTroopCount;
}

