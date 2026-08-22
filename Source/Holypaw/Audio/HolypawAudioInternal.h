#pragma once

#include "CoreMinimal.h"
#include "Audio/HolypawAudioTypes.h"

namespace HolypawSynth
{
	constexpr int32 SampleRate = 24000;
	constexpr int32 Channels = 2;

	void InitLut();
	float FastSin(float Radians);
	float FastCos(float Radians);
	float MidiHz(float Midi);
	int32 ScaleSemitone(EHolypawScale Scale, int32 Degree);
	float NoteHz(int32 RootMidi, EHolypawScale Scale, int32 Degree, int32 Octaves);

	uint32 HashU(uint32 X);
	float White(uint32& Rng);
	float SoftClip(float X);
	float ExpDecay(float T, float Rate);
	float EnvADSR(float T, float Dur, float A, float D, float Sustain, float Rel);
	float OnePoleCoeff(float CutHz);

	float OscSine(float Cycles);
	float OscTri(float Cycles);
	float OscWarm(float Cycles, float Bright);
	float OscBell(float Hz, float T, float Index);

	struct FOnePole
	{
		float Z = 0.f;
		float Tick(float X, float Coeff);
	};

	struct FStereo
	{
		TArray<float> L;
		TArray<float> R;
		void Zero(int32 Count);
		int32 Num() const { return L.Num(); }
	};

	void ToPcm16(const FStereo& S, TArray<uint8>& Out, float PeakDb = -9.f);
	void AddDelayWidth(FStereo& S, int32 Samples, float Mix);
	void SoftLimit(FStereo& S);
}

namespace HolypawCues
{
	const TArray<FHolypawCueDef>& All();
	const FHolypawCueDef* Find(FName Id);
	EHolypawBus BusFor(FName Id);
	TArray<FName> RequiredIds();
}

namespace HolypawScore
{
	FHolypawScore ForZone(EHolypawZone Zone, bool bInterior, EHolypawDayPart Day);
	FHolypawScore Title();
	FHolypawScore Combat(EHolypawZone Zone, bool bBoss, bool bPhaseTwo);
	FHolypawScore Victory();
	void Render(const FHolypawScore& Score, float Seconds, bool bCombat, HolypawSynth::FStereo& Out);
	void RenderBed(EHolypawBed Kind, float Seconds, float Gain, HolypawSynth::FStereo& Out);
	void RenderMill(float Seconds, HolypawSynth::FStereo& Out);
}

namespace HolypawSfx
{
	void RenderCue(FName Cue, uint32 Seed, HolypawSynth::FStereo& Out);
	void RenderAbility(FName Kind, bool bCrit, bool bStagger, uint32 Seed, HolypawSynth::FStereo& Out);
	void RenderFootstep(EHolypawZone Zone, bool bInterior, uint32 Seed, HolypawSynth::FStereo& Out);
}
