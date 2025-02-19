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
    //���� ������
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float damage;

    //���� ��ġ
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector weaponPosition;

    //���� ȸ�� ��
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FRotator weaponRotation;

    // ���� Ÿ�� (â, ��, �ָ�, Ȱ)
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EWeaponState WeaponType;

    // ���� �ִϸ��̼�
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UAnimMontage* AttackUpMontage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UAnimMontage* AttackDownMontage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UAnimMontage* AttackLeftMontage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UAnimMontage* AttackRightMontage;

    // ���� ���� �ִϸ��̼�
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UAnimMontage* ChangeWeaponMontage;

    // �ǰ� �ִϸ��̼�
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UAnimMontage* HitMontage;

    // ��� �ִϸ��̼�
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UAnimMontage* DieMontage;
    //��� �ִϸ��̼�
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UAnimMontage* GuardUpMontage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UAnimMontage* GuardDownMontage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UAnimMontage* GuardRightMontage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UAnimMontage* GuardLeftMontage;

    //Ȱ �ִϸ��̼�
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UAnimMontage* BowAim;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UAnimMontage* BowIdle;

};
