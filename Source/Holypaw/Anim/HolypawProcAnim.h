#pragma once

#include "CoreMinimal.h"

class UStaticMeshComponent;

/**
 * Procedural motion for the conversion fantasy.
 * Graphics stay in actor constructors. These clips are the hug: wrap, blink,
 * ear flop, kneel, party trail, victory. Swap for skeletal anims later by name.
 */
namespace HolypawAnim
{
	constexpr float HugLockSeconds = 0.32f;
	constexpr float HugSeconds = 0.70f;
	constexpr float ConvertHoldSeconds = 0.70f;
	constexpr float HugStandOff = 96.f;
	constexpr float BlinkCloseSeconds = 0.12f;
	constexpr float KneelSeconds = 0.72f;
	constexpr float ConvertBowSeconds = 1.20f;
	constexpr float VictorySeconds = 0.95f;
	constexpr float HurtSeconds = 0.34f;
	constexpr float LandSeconds = 0.22f;
	constexpr float CelebrateSeconds = 0.90f;
	constexpr float PartyHopPeriod = 0.62f;
	constexpr float EarStiffness = 48.f;
	constexpr float EarDamping = 9.2f;
	constexpr float WalkRefSpeed = 700.f;

	enum class EClip : uint8
	{
		Idle = 0,
		Walk,
		Wrap,
		Blink,
		EarFlop,
		Kneel,
		PartyTrail,
		Victory,
		Hurt,
		Land,
		ConvertBow
	};

	enum class EHumanKneel : uint8
	{
		None = 0,
		ConvertBow,
		Worship
	};

	struct FClipKey
	{
		float Time = 0.f;
		float Value = 0.f;
	};

	extern const FClipKey WrapKeys[7];
	extern const int32 WrapKeyCount;
	extern const FClipKey ReachKeys[5];
	extern const int32 ReachKeyCount;
	extern const FClipKey SqueezeKeys[6];
	extern const int32 SqueezeKeyCount;
	extern const FClipKey BlinkKeys[4];
	extern const int32 BlinkKeyCount;
	extern const FClipKey KneelKeys[5];
	extern const int32 KneelKeyCount;
	extern const FClipKey BowKeys[6];
	extern const int32 BowKeyCount;
	extern const FClipKey VictorySpinKeys[5];
	extern const int32 VictorySpinKeyCount;
	extern const FClipKey VictoryHopKeys[5];
	extern const int32 VictoryHopKeyCount;
	extern const FClipKey LandSquashKeys[4];
	extern const int32 LandSquashKeyCount;
	extern const FClipKey PartyHopKeys[5];
	extern const int32 PartyHopKeyCount;
	extern const FClipKey PartyLandKeys[5];
	extern const int32 PartyLandKeyCount;
	extern const FClipKey CelebrateSpinKeys[4];
	extern const int32 CelebrateSpinKeyCount;

	FName ClipName(EClip Clip);
	float SampleClip(const FClipKey* Keys, int32 Num, float Time, bool bLoop = false);
	float Smooth01(float X);
	void Spring(float& Value, float& Vel, float Target, float Stiffness, float Damping, float Dt);

	struct FTeddyRest
	{
		FVector BodyLoc = FVector::ZeroVector;
		FVector BodyScale = FVector(0.85f, 0.7f, 0.75f);
		FRotator BodyRot = FRotator::ZeroRotator;
		FVector HeadLoc = FVector::ZeroVector;
		FRotator HeadRot = FRotator::ZeroRotator;
		FVector EarLLoc = FVector::ZeroVector;
		FVector EarRLoc = FVector::ZeroVector;
		FRotator EarLRot = FRotator::ZeroRotator;
		FRotator EarRRot = FRotator::ZeroRotator;
		FVector PawLLoc = FVector::ZeroVector;
		FVector PawRLoc = FVector::ZeroVector;
		FRotator PawLRot = FRotator::ZeroRotator;
		FRotator PawRRot = FRotator::ZeroRotator;
		FVector SnoutLoc = FVector::ZeroVector;
		FVector BellyScale = FVector(0.55f, 0.45f, 0.45f);
		FVector EyeLScale = FVector::OneVector;
		FVector EyeRScale = FVector::OneVector;
		FRotator HaloRot = FRotator::ZeroRotator;
	};

	struct FTeddyInput
	{
		float Speed = 0.f;
		bool bAirborne = false;
		bool bInBattle = false;
		float DeltaSeconds = 0.f;
	};

	struct FTeddyState
	{
		float Clock = 0.f;
		float BlinkT = 2.4f;
		float ExtraBlink = 0.f;
		float EarL = 0.f;
		float EarR = 0.f;
		float EarVL = 0.f;
		float EarVR = 0.f;
		float HugAge = 0.f;
		bool bHugging = false;
		bool bConvert = false;
		bool bPendingVictory = false;
		float ConvertT = 0.f;
		FVector HugDir = FVector::ForwardVector;
		FVector HugTarget = FVector::ZeroVector;
		bool bHasHugTarget = false;
		float VictoryT = 0.f;
		float HurtT = 0.f;
		float LandT = 0.f;
		float JumpStretch = 0.f;
		float Walk = 0.f;
		bool bInBattle = false;
		bool bWasAir = false;
		bool bDoubleBlink = false;
	};

	struct FTeddyPose
	{
		FVector BodyLoc = FVector::ZeroVector;
		FVector BodyScale = FVector::OneVector;
		FRotator BodyRot = FRotator::ZeroRotator;
		FVector HeadLoc = FVector::ZeroVector;
		FRotator HeadRot = FRotator::ZeroRotator;
		FVector EarLLoc = FVector::ZeroVector;
		FVector EarRLoc = FVector::ZeroVector;
		FRotator EarLRot = FRotator::ZeroRotator;
		FRotator EarRRot = FRotator::ZeroRotator;
		FVector PawLLoc = FVector::ZeroVector;
		FVector PawRLoc = FVector::ZeroVector;
		FRotator PawLRot = FRotator::ZeroRotator;
		FRotator PawRRot = FRotator::ZeroRotator;
		FVector SnoutLoc = FVector::ZeroVector;
		FVector BellyScale = FVector::OneVector;
		FVector EyeLScale = FVector::OneVector;
		FVector EyeRScale = FVector::OneVector;
		FRotator HaloRot = FRotator::ZeroRotator;
	};

	struct FTeddyParts
	{
		UStaticMeshComponent* Body = nullptr;
		UStaticMeshComponent* Head = nullptr;
		UStaticMeshComponent* EarL = nullptr;
		UStaticMeshComponent* EarR = nullptr;
		UStaticMeshComponent* PawL = nullptr;
		UStaticMeshComponent* PawR = nullptr;
		UStaticMeshComponent* Snout = nullptr;
		UStaticMeshComponent* Belly = nullptr;
		UStaticMeshComponent* EyeL = nullptr;
		UStaticMeshComponent* EyeR = nullptr;
		UStaticMeshComponent* Halo = nullptr;
	};

	void CaptureTeddyRest(FTeddyRest& Rest, const FTeddyParts& Parts);
	void PlayHug(FTeddyState& S, const FVector& WorldToTarget);
	void PlayHug(FTeddyState& S, const FVector& WorldToTarget, const FVector& WorldTarget);
	void PlayConvert(FTeddyState& S);
	bool IsHugging(const FTeddyState& S);
	void PlayVictory(FTeddyState& S);
	void PlayHurt(FTeddyState& S);
	void PlayJump(FTeddyState& S);
	void PlayLand(FTeddyState& S);
	void PlayBlink(FTeddyState& S, bool bDouble);
	bool IsWrapLocked(const FTeddyState& S);
	float WrapAmount(const FTeddyState& S);
	float ReachAmount(const FTeddyState& S);
	void TickTeddy(FTeddyState& S, const FTeddyInput& In);
	FTeddyPose EvaluateTeddy(const FTeddyState& S, const FTeddyRest& Rest);
	void ApplyTeddyPose(const FTeddyPose& Pose, const FTeddyParts& Parts);

	struct FHumanRest
	{
		FVector Scale = FVector::OneVector;
		FRotator ActorRot = FRotator::ZeroRotator;
		FVector HeadLoc = FVector::ZeroVector;
		FRotator HeadRot = FRotator::ZeroRotator;
		FVector ArmLLoc = FVector::ZeroVector;
		FVector ArmRLoc = FVector::ZeroVector;
	};

	struct FHumanState
	{
		float Clock = 0.f;
		float HugT = 0.f;
		float KneelT = 0.f;
		EHumanKneel Kneel = EHumanKneel::None;
		bool bBeliever = false;
		float ConvertBurst = 0.f;
		float HugYaw = 0.f;
		float BowDelay = 0.f;
	};

	struct FHumanPose
	{
		FVector Scale = FVector::OneVector;
		FRotator ActorRot = FRotator::ZeroRotator;
		FRotator HeadRot = FRotator::ZeroRotator;
		FRotator ArmL = FRotator::ZeroRotator;
		FRotator ArmR = FRotator::ZeroRotator;
		FVector HeadLoc = FVector::ZeroVector;
		FVector ArmLLoc = FVector::ZeroVector;
		FVector ArmRLoc = FVector::ZeroVector;
		float DropZ = 0.f;
		bool bHoldFeet = false;
	};

	void PlayHumanHug(FHumanState& S);
	void PlayHumanHug(FHumanState& S, const FVector& WorldFromHumanToTeddy);
	void PlayConvertBow(FHumanState& S);
	void PlayWorshipKneel(FHumanState& S);
	void ResetHumanMotion(FHumanState& S);
	void TickHuman(FHumanState& S, float DeltaSeconds);
	FHumanPose EvaluateHuman(const FHumanState& S, const FHumanRest& Rest);

	struct FPartyState
	{
		float Clock = 0.f;
		float CelebrateT = 0.f;
	};

	struct FPartySlotPose
	{
		FVector Location = FVector::ZeroVector;
		FVector Scale = FVector(0.35f);
		FRotator Rot = FRotator::ZeroRotator;
	};

	void PlayCelebrate(FPartyState& S);
	void TickParty(FPartyState& S, float DeltaSeconds);
	FPartySlotPose EvaluateParty(
		const TArray<FVector>& Trail,
		int32 Index,
		float Clock,
		float Celebrate,
		const FVector& TeddyLoc,
		const FVector& TeddyForward);

	struct FFluffyRest
	{
		FRotator EarL = FRotator::ZeroRotator;
		FRotator EarR = FRotator::ZeroRotator;
		FVector TailLoc = FVector::ZeroVector;
		FVector BodyScale = FVector(0.55f, 0.4f, 0.4f);
	};

	struct FFluffyPose
	{
		FRotator EarL = FRotator::ZeroRotator;
		FRotator EarR = FRotator::ZeroRotator;
		FVector TailLoc = FVector::ZeroVector;
		FRotator TailRot = FRotator::ZeroRotator;
		FVector BodyScale = FVector::OneVector;
		float BobZ = 0.f;
	};

	FFluffyPose EvaluateFluffy(const FFluffyRest& Rest, float Clock, const FVector& Velocity, bool bRecruited);
}
