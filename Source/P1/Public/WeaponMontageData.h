// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Animation/AnimMontage.h"
#include "EWeaponState.h"
#include "WeaponMontageData.generated.h"
/**
 *
 */
USTRUCT(BlueprintType)
struct FWeaponMontageData : public FTableRowBase
{
    GENERATED_BODY()

public:
    //무기 데미지
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float damage;

    //무기 위치
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector weaponPosition;

    //무기 회전 값
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FRotator weaponRotation;

    // 무기 타입 (창, 검, 주먹, 활)
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EWeaponState WeaponType;

    // 공격 애니메이션
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UAnimMontage* AttackUpMontage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UAnimMontage* AttackDownMontage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UAnimMontage* AttackLeftMontage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UAnimMontage* AttackRightMontage;

    // 무기 변경 애니메이션
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UAnimMontage* ChangeWeaponMontage;

    // 피격 애니메이션
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UAnimMontage* HitMontage;

    // 사망 애니메이션
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UAnimMontage* DieMontage;
    //방어 애니메이션
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UAnimMontage* GuardUpMontage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UAnimMontage* GuardDownMontage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UAnimMontage* GuardRightMontage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UAnimMontage* GuardLeftMontage;

    //활 애니메이션
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UAnimMontage* BowAim;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UAnimMontage* BowIdle;

};
