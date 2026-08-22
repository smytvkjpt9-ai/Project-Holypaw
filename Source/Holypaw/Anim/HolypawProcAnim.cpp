#include "Anim/HolypawProcAnim.h"
#include "Components/StaticMeshComponent.h"

namespace HolypawAnim
{
	const FClipKey WrapKeys[7] = {
		{0.00f, 0.00f},
		{0.08f, 0.22f},
		{0.18f, 0.70f},
		{0.28f, 1.00f},
		{0.42f, 1.00f},
		{0.55f, 0.48f},
		{0.70f, 0.00f},
	};
	const int32 WrapKeyCount = UE_ARRAY_COUNT(WrapKeys);

	const FClipKey SqueezeKeys[6] = {
		{0.00f, 0.00f},
		{0.16f, 0.20f},
		{0.28f, 1.00f},
		{0.40f, 0.85f},
		{0.52f, 0.25f},
		{0.70f, 0.00f},
	};
	const int32 SqueezeKeyCount = UE_ARRAY_COUNT(SqueezeKeys);

	const FClipKey BlinkKeys[4] = {
		{0.00f, 0.00f},
		{0.04f, 1.00f},
		{0.08f, 1.00f},
		{0.12f, 0.00f},
	};
	const int32 BlinkKeyCount = UE_ARRAY_COUNT(BlinkKeys);

	const FClipKey KneelKeys[5] = {
		{0.00f, 0.00f},
		{0.18f, 0.20f},
		{0.40f, 0.72f},
		{0.58f, 0.94f},
		{0.72f, 1.00f},
	};
	const int32 KneelKeyCount = UE_ARRAY_COUNT(KneelKeys);

	const FClipKey BowKeys[6] = {
		{0.00f, 0.00f},
		{0.22f, 0.55f},
		{0.40f, 1.00f},
		{0.70f, 1.00f},
		{0.95f, 0.35f},
		{1.20f, 0.00f},
	};
	const int32 BowKeyCount = UE_ARRAY_COUNT(BowKeys);

	const FClipKey VictorySpinKeys[5] = {
		{0.00f, 0.00f},
		{0.18f, 70.00f},
		{0.42f, 210.00f},
		{0.70f, 330.00f},
		{0.95f, 360.00f},
	};
	const int32 VictorySpinKeyCount = UE_ARRAY_COUNT(VictorySpinKeys);

	const FClipKey VictoryHopKeys[5] = {
		{0.00f, 0.00f},
		{0.12f, 16.00f},
		{0.32f, 28.00f},
		{0.58f, 10.00f},
		{0.95f, 0.00f},
	};
	const int32 VictoryHopKeyCount = UE_ARRAY_COUNT(VictoryHopKeys);

	const FClipKey LandSquashKeys[4] = {
		{0.00f, 0.00f},
		{0.06f, 1.00f},
		{0.14f, 0.45f},
		{0.22f, 0.00f},
	};
	const int32 LandSquashKeyCount = UE_ARRAY_COUNT(LandSquashKeys);

	FName ClipName(const EClip Clip)
	{
		switch (Clip)
		{
		case EClip::Walk: return TEXT("Walk");
		case EClip::Wrap: return TEXT("Wrap");
		case EClip::Blink: return TEXT("Blink");
		case EClip::EarFlop: return TEXT("EarFlop");
		case EClip::Kneel: return TEXT("Kneel");
		case EClip::PartyTrail: return TEXT("PartyTrail");
		case EClip::Victory: return TEXT("Victory");
		case EClip::Hurt: return TEXT("Hurt");
		case EClip::Land: return TEXT("Land");
		case EClip::ConvertBow: return TEXT("ConvertBow");
		default: return TEXT("Idle");
		}
	}

	float Smooth01(float X)
	{
		X = FMath::Clamp(X, 0.f, 1.f);
		return X * X * (3.f - 2.f * X);
	}

	float SampleClip(const FClipKey* Keys, const int32 Num, const float Time, const bool bLoop)
	{
		if (!Keys || Num <= 0)
		{
			return 0.f;
		}
		if (Num == 1)
		{
			return Keys[0].Value;
		}
		float T = Time;
		const float LastT = Keys[Num - 1].Time;
		if (bLoop && LastT > 0.f)
		{
			T = FMath::Fmod(T, LastT);
			if (T < 0.f)
			{
				T += LastT;
			}
		}
		if (T <= Keys[0].Time)
		{
			return Keys[0].Value;
		}
		if (T >= LastT)
		{
			return Keys[Num - 1].Value;
		}
		for (int32 I = 0; I < Num - 1; ++I)
		{
			if (T <= Keys[I + 1].Time)
			{
				const float Span = Keys[I + 1].Time - Keys[I].Time;
				const float A = Span > KINDA_SMALL_NUMBER ? (T - Keys[I].Time) / Span : 1.f;
				return FMath::Lerp(Keys[I].Value, Keys[I + 1].Value, Smooth01(A));
			}
		}
		return Keys[Num - 1].Value;
	}

	void Spring(float& Value, float& Vel, const float Target, const float Stiffness, const float Damping, const float Dt)
	{
		const float Accel = (Target - Value) * Stiffness - Vel * Damping;
		Vel += Accel * Dt;
		Value += Vel * Dt;
	}

	static float Decay(float& Value, const float Dt, const float Rate)
	{
		if (Value > 0.f)
		{
			Value = FMath::Max(0.f, Value - Dt * Rate);
		}
		return Value;
	}

	void CaptureTeddyRest(FTeddyRest& Rest, const FTeddyParts& Parts)
	{
		if (Parts.Body)
		{
			Rest.BodyLoc = Parts.Body->GetRelativeLocation();
			Rest.BodyScale = Parts.Body->GetRelativeScale3D();
			Rest.BodyRot = Parts.Body->GetRelativeRotation();
		}
		if (Parts.Head)
		{
			Rest.HeadLoc = Parts.Head->GetRelativeLocation();
			Rest.HeadRot = Parts.Head->GetRelativeRotation();
		}
		if (Parts.EarL)
		{
			Rest.EarLLoc = Parts.EarL->GetRelativeLocation();
			Rest.EarLRot = Parts.EarL->GetRelativeRotation();
		}
		if (Parts.EarR)
		{
			Rest.EarRLoc = Parts.EarR->GetRelativeLocation();
			Rest.EarRRot = Parts.EarR->GetRelativeRotation();
		}
		if (Parts.PawL)
		{
			Rest.PawLLoc = Parts.PawL->GetRelativeLocation();
			Rest.PawLRot = Parts.PawL->GetRelativeRotation();
		}
		if (Parts.PawR)
		{
			Rest.PawRLoc = Parts.PawR->GetRelativeLocation();
			Rest.PawRRot = Parts.PawR->GetRelativeRotation();
		}
		if (Parts.Snout)
		{
			Rest.SnoutLoc = Parts.Snout->GetRelativeLocation();
		}
		if (Parts.Belly)
		{
			Rest.BellyScale = Parts.Belly->GetRelativeScale3D();
		}
		if (Parts.EyeL)
		{
			Rest.EyeLScale = Parts.EyeL->GetRelativeScale3D();
		}
		if (Parts.EyeR)
		{
			Rest.EyeRScale = Parts.EyeR->GetRelativeScale3D();
		}
		if (Parts.Halo)
		{
			Rest.HaloRot = Parts.Halo->GetRelativeRotation();
		}
	}

	void PlayHug(FTeddyState& S, const FVector& WorldToTarget)
	{
		S.HugT = HugSeconds;
		S.HugDir = WorldToTarget.GetSafeNormal2D();
		if (S.HugDir.IsNearlyZero())
		{
			S.HugDir = FVector::ForwardVector;
		}
		S.ExtraBlink = BlinkCloseSeconds;
		S.EarVL -= 90.f;
		S.EarVR += 90.f;
	}

	void PlayVictory(FTeddyState& S)
	{
		S.VictoryT = VictorySeconds;
		S.EarVL -= 70.f;
		S.EarVR += 70.f;
		S.ExtraBlink = BlinkCloseSeconds * 0.6f;
		S.bDoubleBlink = true;
	}

	void PlayHurt(FTeddyState& S)
	{
		S.HurtT = HurtSeconds;
		S.EarVL += 55.f;
		S.EarVR -= 55.f;
	}

	void PlayJump(FTeddyState& S)
	{
		S.JumpStretch = 1.f;
		S.EarVL -= 40.f;
		S.EarVR += 40.f;
	}

	void PlayLand(FTeddyState& S)
	{
		S.LandT = LandSeconds;
		S.JumpStretch = 0.f;
		S.EarVL += 110.f;
		S.EarVR -= 110.f;
	}

	void PlayBlink(FTeddyState& S, const bool bDouble)
	{
		S.BlinkT = BlinkCloseSeconds;
		S.bDoubleBlink = bDouble;
	}

	bool IsWrapLocked(const FTeddyState& S)
	{
		return S.HugT > (HugSeconds - HugLockSeconds);
	}

	float WrapAmount(const FTeddyState& S)
	{
		if (S.HugT <= 0.f)
		{
			return 0.f;
		}
		return SampleClip(WrapKeys, WrapKeyCount, HugSeconds - S.HugT);
	}

	void TickTeddy(FTeddyState& S, const FTeddyInput& In)
	{
		const float Dt = FMath::Max(0.f, In.DeltaSeconds);
		S.Clock += Dt;
		Decay(S.HugT, Dt, 1.f);
		Decay(S.VictoryT, Dt, 1.f);
		Decay(S.HurtT, Dt, 1.f);
		Decay(S.LandT, Dt, 1.f);
		Decay(S.JumpStretch, Dt, 2.4f);
		Decay(S.ExtraBlink, Dt, 1.f);

		S.BlinkT -= Dt;
		if (S.BlinkT < 0.f)
		{
			if (S.bDoubleBlink)
			{
				S.bDoubleBlink = false;
				S.BlinkT = BlinkCloseSeconds * 0.55f;
			}
			else
			{
				S.BlinkT = 2.1f + FMath::FRandRange(0.f, 2.6f);
				S.bDoubleBlink = FMath::FRand() < 0.12f;
			}
		}

		if (S.bWasAir && !In.bAirborne)
		{
			PlayLand(S);
		}
		S.bWasAir = In.bAirborne;
		S.Walk = FMath::Clamp(In.Speed / WalkRefSpeed, 0.f, 1.25f);
		S.bInBattle = In.bInBattle;
		if (S.bInBattle)
		{
			S.Walk *= 0.35f;
		}

		const float Walk = S.Walk;
		const float Wrap = WrapAmount(S);
		const float EarWalk = FMath::Sin(S.Clock * (5.4f + Walk * 6.f)) * (8.f + Walk * 14.f);
		const float EarHug = Wrap * -22.f;
		const float EarVictory = (S.VictoryT > 0.f) ? FMath::Sin(S.Clock * 14.f) * 18.f : 0.f;
		Spring(S.EarL, S.EarVL, EarWalk + EarHug + EarVictory, EarStiffness, EarDamping, Dt);
		Spring(S.EarR, S.EarVR, -EarWalk - EarHug - EarVictory, EarStiffness, EarDamping, Dt);
	}

	FTeddyPose EvaluateTeddy(const FTeddyState& S, const FTeddyRest& Rest)
	{
		FTeddyPose P;
		const float Wrap = WrapAmount(S);
		const float Squeeze = (S.HugT > 0.f) ? SampleClip(SqueezeKeys, SqueezeKeyCount, HugSeconds - S.HugT) : 0.f;
		const float HurtN = S.HurtT > 0.f ? S.HurtT / HurtSeconds : 0.f;
		const float LandN = S.LandT > 0.f ? SampleClip(LandSquashKeys, LandSquashKeyCount, LandSeconds - S.LandT) : 0.f;
		const float VicAge = S.VictoryT > 0.f ? VictorySeconds - S.VictoryT : 0.f;
		const float VicSpin = S.VictoryT > 0.f ? SampleClip(VictorySpinKeys, VictorySpinKeyCount, VicAge) : 0.f;
		const float VicHop = S.VictoryT > 0.f ? SampleClip(VictoryHopKeys, VictoryHopKeyCount, VicAge) : 0.f;
		const float WalkAmt = FMath::Max(0.15f, S.Walk);
		const float WalkBob = FMath::Sin(S.Clock * (7.2f + S.Walk * 5.f)) * 6.f * WalkAmt;
		const float AirStretch = S.bWasAir ? 0.12f + S.JumpStretch * 0.18f : S.JumpStretch * 0.10f;
		const float Squash = HurtN * 0.16f + LandN * 0.14f + Squeeze * 0.12f;
		const float StretchZ = AirStretch - Squash;

		P.BodyLoc = Rest.BodyLoc + FVector(Wrap * 6.f, 0.f, WalkBob * 0.35f + VicHop - Squash * 8.f + AirStretch * 10.f);
		P.BodyScale = Rest.BodyScale * FVector(1.f + Squash - AirStretch * 0.35f, 1.f + Squash - AirStretch * 0.25f, 1.f + StretchZ);
		P.BodyRot = Rest.BodyRot + FRotator(Wrap * 14.f, VicSpin * 0.35f, FMath::Sin(S.Clock * 2.2f) * 2.f);

		P.HeadLoc = Rest.HeadLoc + FVector(Wrap * 4.f, 0.f, WalkBob * 0.22f + VicHop * 0.45f);
		P.HeadRot = Rest.HeadRot + FRotator(Wrap * 8.f - Squeeze * 6.f, VicSpin, 0.f);

		P.EarLLoc = Rest.EarLLoc + FVector(0.f, Wrap * 3.f, S.EarL * 0.04f);
		P.EarRLoc = Rest.EarRLoc + FVector(0.f, Wrap * -3.f, S.EarR * 0.04f);
		P.EarLRot = Rest.EarLRot + FRotator(S.EarL, Wrap * -8.f, Wrap * -16.f + S.EarL * 0.35f);
		P.EarRRot = Rest.EarRRot + FRotator(S.EarR, Wrap * 8.f, Wrap * 16.f + S.EarR * 0.35f);

		P.PawLLoc = Rest.PawLLoc + FVector(Wrap * 10.f, Wrap * 8.f, Wrap * 12.f + Squeeze * 4.f);
		P.PawRLoc = Rest.PawRLoc + FVector(Wrap * 10.f, Wrap * -8.f, Wrap * 12.f + Squeeze * 4.f);
		P.PawLRot = Rest.PawLRot + FRotator(Wrap * 52.f + Squeeze * 10.f, Wrap * 6.f, Wrap * 38.f);
		P.PawRRot = Rest.PawRRot + FRotator(Wrap * 52.f + Squeeze * 10.f, Wrap * -6.f, Wrap * -38.f);

		P.SnoutLoc = Rest.SnoutLoc + FVector(Squeeze * 4.f + Wrap * 2.f, 0.f, Squeeze * -1.5f);
		P.BellyScale = Rest.BellyScale * FVector(1.f + Squeeze * 0.18f, 1.f + Squeeze * 0.12f, 1.f - Squeeze * 0.08f);

		float Close = 0.f;
		if (S.BlinkT >= 0.f && S.BlinkT <= BlinkCloseSeconds)
		{
			Close = SampleClip(BlinkKeys, BlinkKeyCount, BlinkCloseSeconds - S.BlinkT);
		}
		Close = FMath::Max(Close, S.ExtraBlink > 0.f ? SampleClip(BlinkKeys, BlinkKeyCount, BlinkCloseSeconds - S.ExtraBlink) : 0.f);
		Close = FMath::Max(Close, Squeeze * 0.85f);
		const float EyeY = 1.f - Close * 0.88f;
		P.EyeLScale = FVector(Rest.EyeLScale.X, Rest.EyeLScale.Y, Rest.EyeLScale.Z * EyeY);
		P.EyeRScale = FVector(Rest.EyeRScale.X, Rest.EyeRScale.Y, Rest.EyeRScale.Z * EyeY);

		P.HaloRot = Rest.HaloRot + FRotator(0.f, S.Clock * 40.f + VicSpin * 0.5f, 0.f);
		return P;
	}

	void ApplyTeddyPose(const FTeddyPose& Pose, const FTeddyParts& Parts)
	{
		if (Parts.Body)
		{
			Parts.Body->SetRelativeLocation(Pose.BodyLoc);
			Parts.Body->SetRelativeScale3D(Pose.BodyScale);
			Parts.Body->SetRelativeRotation(Pose.BodyRot);
		}
		if (Parts.Head)
		{
			Parts.Head->SetRelativeLocation(Pose.HeadLoc);
			Parts.Head->SetRelativeRotation(Pose.HeadRot);
		}
		if (Parts.EarL)
		{
			Parts.EarL->SetRelativeLocation(Pose.EarLLoc);
			Parts.EarL->SetRelativeRotation(Pose.EarLRot);
		}
		if (Parts.EarR)
		{
			Parts.EarR->SetRelativeLocation(Pose.EarRLoc);
			Parts.EarR->SetRelativeRotation(Pose.EarRRot);
		}
		if (Parts.PawL)
		{
			Parts.PawL->SetRelativeLocation(Pose.PawLLoc);
			Parts.PawL->SetRelativeRotation(Pose.PawLRot);
		}
		if (Parts.PawR)
		{
			Parts.PawR->SetRelativeLocation(Pose.PawRLoc);
			Parts.PawR->SetRelativeRotation(Pose.PawRRot);
		}
		if (Parts.Snout)
		{
			Parts.Snout->SetRelativeLocation(Pose.SnoutLoc);
		}
		if (Parts.Belly)
		{
			Parts.Belly->SetRelativeScale3D(Pose.BellyScale);
		}
		if (Parts.EyeL)
		{
			Parts.EyeL->SetRelativeScale3D(Pose.EyeLScale);
		}
		if (Parts.EyeR)
		{
			Parts.EyeR->SetRelativeScale3D(Pose.EyeRScale);
		}
		if (Parts.Halo)
		{
			Parts.Halo->SetRelativeRotation(Pose.HaloRot);
		}
	}

	void PlayHumanHug(FHumanState& S)
	{
		S.HugT = HugSeconds;
	}

	void PlayConvertBow(FHumanState& S)
	{
		S.Kneel = EHumanKneel::ConvertBow;
		S.KneelT = ConvertBowSeconds;
		S.ConvertBurst = 1.f;
		S.bBeliever = true;
	}

	void PlayWorshipKneel(FHumanState& S)
	{
		S.Kneel = EHumanKneel::Worship;
		S.KneelT = 0.f;
		S.bBeliever = true;
	}

	void ResetHumanMotion(FHumanState& S)
	{
		S.HugT = 0.f;
		S.KneelT = 0.f;
		S.Kneel = EHumanKneel::None;
		S.bBeliever = false;
		S.ConvertBurst = 0.f;
	}

	void TickHuman(FHumanState& S, const float DeltaSeconds)
	{
		const float Dt = FMath::Max(0.f, DeltaSeconds);
		S.Clock += Dt;
		Decay(S.HugT, Dt, 1.f);
		Decay(S.ConvertBurst, Dt, 1.35f);
		if (S.Kneel == EHumanKneel::ConvertBow)
		{
			S.KneelT = FMath::Max(0.f, S.KneelT - Dt);
			if (S.KneelT <= 0.f)
			{
				S.Kneel = EHumanKneel::None;
			}
		}
		else if (S.Kneel == EHumanKneel::Worship)
		{
			S.KneelT = FMath::Min(KneelSeconds, S.KneelT + Dt);
		}
	}

	static float HumanKneelAlpha(const FHumanState& S)
	{
		if (S.Kneel == EHumanKneel::Worship)
		{
			return SampleClip(KneelKeys, KneelKeyCount, S.KneelT);
		}
		if (S.Kneel == EHumanKneel::ConvertBow)
		{
			return SampleClip(BowKeys, BowKeyCount, ConvertBowSeconds - S.KneelT);
		}
		return 0.f;
	}

	FHumanPose EvaluateHuman(const FHumanState& S, const FHumanRest& Rest)
	{
		FHumanPose P;
		const float Wrap = (S.HugT > 0.f) ? SampleClip(WrapKeys, WrapKeyCount, HugSeconds - S.HugT) : 0.f;
		const float Squeeze = (S.HugT > 0.f) ? SampleClip(SqueezeKeys, SqueezeKeyCount, HugSeconds - S.HugT) : 0.f;
		const float Kneel = HumanKneelAlpha(S);
		const float Bounce = S.bBeliever && S.Kneel != EHumanKneel::Worship
			? 0.055f * FMath::Sin(S.Clock * 6.2f)
			: 0.018f * FMath::Sin(S.Clock * 2.4f);
		const float Burst = S.ConvertBurst * 0.08f;

		P.Scale = Rest.Scale * FVector(
			1.f + Squeeze * 0.14f + Burst,
			1.f + Squeeze * 0.14f + Burst,
			1.f + Bounce - Squeeze * 0.42f - Kneel * 0.28f);
		P.ActorRot = Rest.ActorRot + FRotator(Kneel * 36.f, 0.f, Wrap * 4.f);

		const float Clap = (S.bBeliever && Kneel < 0.35f) ? FMath::Sin(S.Clock * 9.f) * 16.f : 0.f;
		const float WorshipArms = Kneel * 42.f;
		P.ArmL = FRotator(WorshipArms + Clap, Wrap * 12.f, 8.f + Wrap * 46.f + Kneel * 18.f);
		P.ArmR = FRotator(WorshipArms + Clap, Wrap * -12.f, -8.f - Wrap * 46.f - Kneel * 18.f);
		P.HeadLoc = Rest.HeadLoc + FVector(Wrap * 6.f + Kneel * 8.f, 0.f, Kneel * -10.f + Bounce * 20.f);
		P.ArmLLoc = Rest.ArmLLoc + FVector(Wrap * 10.f, Wrap * 6.f, Wrap * 4.f + Kneel * 8.f);
		P.ArmRLoc = Rest.ArmRLoc + FVector(Wrap * 10.f, Wrap * -6.f, Wrap * 4.f + Kneel * 8.f);
		return P;
	}

	void PlayCelebrate(FPartyState& S)
	{
		S.CelebrateT = CelebrateSeconds;
	}

	void TickParty(FPartyState& S, const float DeltaSeconds)
	{
		S.Clock += FMath::Max(0.f, DeltaSeconds);
		Decay(S.CelebrateT, DeltaSeconds, 1.f);
	}

	FPartySlotPose EvaluateParty(
		const TArray<FVector>& Trail,
		const int32 Index,
		const float Clock,
		const float Celebrate,
		const FVector& TeddyLoc,
		const FVector& TeddyForward)
	{
		FPartySlotPose P;
		const int32 Sample = FMath::Min(Trail.Num() - 1, (Index + 1) * 12);
		FVector Path = TeddyLoc;
		FVector Tangent = TeddyForward.GetSafeNormal2D();
		if (Trail.IsValidIndex(Sample))
		{
			Path = Trail[Sample];
		}
		if (Trail.IsValidIndex(Sample + 1) && Trail.IsValidIndex(Sample))
		{
			Tangent = (Trail[Sample] - Trail[Sample + 1]).GetSafeNormal2D();
		}
		if (Tangent.IsNearlyZero())
		{
			Tangent = FVector::ForwardVector;
		}
		const FVector Side = FVector::CrossProduct(FVector::UpVector, Tangent).GetSafeNormal();
		const float Lane = (Index % 2 == 0) ? 34.f : -34.f;
		const float LaneScale = 1.f + (Index / 2) * 0.15f;

		const float Phase = Index * 1.17f + 0.31f;
		const float Wave = FMath::Sin(Clock * PartyHopHz * 2.f * PI + Phase);
		const float Hop = FMath::Max(0.f, Wave) * (16.f + Celebrate * 22.f);
		const float Ground = Wave < 0.f ? FMath::Abs(Wave) : 0.f;
		const float Squash = Ground * 0.22f * (1.f - Celebrate * 0.4f);

		P.Location = Path + Side * Lane * LaneScale + FVector(0.f, 0.f, 28.f + Hop);
		P.Scale = FVector(0.35f + Squash * 0.12f, 0.35f + Squash * 0.12f, 0.35f - Squash * 0.16f + Hop * 0.004f);
		const float Spin = Celebrate * (120.f + Index * 40.f);
		P.Rot = Tangent.Rotation() + FRotator(0.f, Clock * Spin, Wave * 8.f);
		return P;
	}

	FFluffyPose EvaluateFluffy(const FFluffyRest& Rest, const float Clock, const FVector& Velocity, const bool bRecruited)
	{
		FFluffyPose P;
		if (bRecruited)
		{
			P.EarL = Rest.EarL;
			P.EarR = Rest.EarR;
			P.TailLoc = Rest.TailLoc;
			P.BodyScale = Rest.BodyScale;
			return P;
		}
		const float Speed = Velocity.Size2D();
		const float Walk = FMath::Clamp(Speed / 90.f, 0.f, 1.4f);
		const float Flop = FMath::Sin(Clock * (4.8f + Walk * 5.f)) * (10.f + Walk * 16.f);
		P.EarL = Rest.EarL + FRotator(Flop, 0.f, Flop * 0.4f);
		P.EarR = Rest.EarR + FRotator(-Flop, 0.f, -Flop * 0.4f);
		P.TailLoc = Rest.TailLoc;
		P.TailRot = FRotator(FMath::Sin(Clock * 7.5f) * 18.f, FMath::Sin(Clock * 5.2f) * 22.f, 0.f);
		const float Bob = FMath::Abs(FMath::Sin(Clock * 6.4f)) * 0.06f * FMath::Max(0.2f, Walk);
		P.BodyScale = Rest.BodyScale * FVector(1.f + Bob * 0.4f, 1.f + Bob * 0.3f, 1.f - Bob);
		P.BobZ = FMath::Abs(FMath::Sin(Clock * 6.4f)) * 6.f * Walk;
		return P;
	}
}
