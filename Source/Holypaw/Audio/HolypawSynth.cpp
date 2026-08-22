#include "Audio/HolypawAudioInternal.h"

namespace HolypawSynth
{
	static float Lut[4096];
	static bool bLutReady = false;

	void InitLut()
	{
		if (bLutReady)
		{
			return;
		}
		for (int32 I = 0; I < 4096; ++I)
		{
			Lut[I] = FMath::Sin(2.f * PI * static_cast<float>(I) / 4096.f);
		}
		bLutReady = true;
	}

	float FastSin(float Radians)
	{
		InitLut();
		float N = Radians * (4096.f / (2.f * PI));
		N -= FMath::FloorToFloat(N / 4096.f) * 4096.f;
		if (N < 0.f)
		{
			N += 4096.f;
		}
		const int32 I = static_cast<int32>(N) & 4095;
		const float F = N - FMath::FloorToFloat(N);
		const float A = Lut[I];
		const float B = Lut[(I + 1) & 4095];
		return A + (B - A) * F;
	}

	float FastCos(float Radians)
	{
		return FastSin(Radians + 0.5f * PI);
	}

	float MidiHz(float Midi)
	{
		return 440.f * FMath::Pow(2.f, (Midi - 69.f) / 12.f);
	}

	int32 ScaleSemitone(const EHolypawScale Scale, int32 Degree)
	{
		static const int32 Major[7] = {0, 2, 4, 5, 7, 9, 11};
		static const int32 Minor[7] = {0, 2, 3, 5, 7, 8, 10};
		static const int32 Dorian[7] = {0, 2, 3, 5, 7, 9, 10};
		static const int32 Mixo[7] = {0, 2, 4, 5, 7, 9, 10};
		static const int32 Lyd[7] = {0, 2, 4, 6, 7, 9, 11};
		static const int32 MajPent[7] = {0, 2, 4, 7, 9, 12, 14};
		static const int32 MinPent[7] = {0, 3, 5, 7, 10, 12, 15};
		static const int32 HarmMin[7] = {0, 2, 3, 5, 7, 8, 11};
		const int32 Oct = Degree >= 0 ? Degree / 7 : (Degree - 6) / 7;
		int32 D = Degree - Oct * 7;
		while (D < 0)
		{
			D += 7;
		}
		D %= 7;
		const int32* T = Major;
		switch (Scale)
		{
		case EHolypawScale::Minor: T = Minor; break;
		case EHolypawScale::Dorian: T = Dorian; break;
		case EHolypawScale::Mixolydian: T = Mixo; break;
		case EHolypawScale::Lydian: T = Lyd; break;
		case EHolypawScale::MajorPent: T = MajPent; break;
		case EHolypawScale::MinorPent: T = MinPent; break;
		case EHolypawScale::HarmonicMinor: T = HarmMin; break;
		default: break;
		}
		return T[D] + Oct * 12;
	}

	float NoteHz(const int32 RootMidi, const EHolypawScale Scale, const int32 Degree, const int32 Octaves)
	{
		return MidiHz(static_cast<float>(RootMidi + ScaleSemitone(Scale, Degree) + Octaves * 12));
	}

	uint32 HashU(uint32 X)
	{
		X ^= X >> 16;
		X *= 0x7feb352du;
		X ^= X >> 15;
		X *= 0x846ca68bu;
		X ^= X >> 16;
		return X;
	}

	float White(uint32& Rng)
	{
		Rng = HashU(Rng + 0x9E3779B9u);
		return (static_cast<int32>(Rng >> 8) / 8388608.f) - 1.f;
	}

	float SoftClip(float X)
	{
		return FMath::Tanh(X);
	}

	float ExpDecay(const float T, const float Rate)
	{
		return T < 0.f ? 0.f : FMath::Exp(-T * Rate);
	}

	float EnvADSR(const float T, const float Dur, const float A, const float D, const float Sustain, const float Rel)
	{
		if (T < 0.f || Dur <= 0.f)
		{
			return 0.f;
		}
		if (T < A)
		{
			return T / FMath::Max(A, 0.0001f);
		}
		if (T < A + D)
		{
			const float U = (T - A) / FMath::Max(D, 0.0001f);
			return 1.f + (Sustain - 1.f) * U;
		}
		const float TailStart = Dur - Rel;
		if (T >= Dur)
		{
			return 0.f;
		}
		if (T > TailStart && Rel > 0.f)
		{
			return Sustain * (1.f - (T - TailStart) / Rel);
		}
		return Sustain;
	}

	float OnePoleCoeff(const float CutHz)
	{
		const float X = FMath::Exp(-2.f * PI * FMath::Clamp(CutHz, 20.f, 12000.f) / static_cast<float>(SampleRate));
		return 1.f - X;
	}

	float OscSine(const float Cycles)
	{
		return FastSin(Cycles * 2.f * PI);
	}

	float OscTri(const float Cycles)
	{
		const float F = Cycles - FMath::FloorToFloat(Cycles);
		return 4.f * FMath::Abs(F - 0.5f) - 1.f;
	}

	float OscWarm(const float Cycles, const float Bright)
	{
		const float B = FMath::Clamp(Bright, 0.f, 1.f);
		return OscSine(Cycles)
			+ B * 0.42f * OscSine(Cycles * 2.f)
			+ B * 0.18f * OscSine(Cycles * 3.f)
			+ B * 0.08f * OscSine(Cycles * 5.f);
	}

	float OscBell(const float Hz, const float T, const float Index)
	{
		const float Mod = OscSine(Hz * 2.015f * T) * Index * ExpDecay(T, 4.5f);
		return OscSine(Hz * T + Mod / (2.f * PI)) * ExpDecay(T, 3.2f);
	}

	float FOnePole::Tick(const float X, const float Coeff)
	{
		Z += Coeff * (X - Z);
		return Z;
	}

	void FStereo::Zero(const int32 Count)
	{
		L.SetNumZeroed(Count);
		R.SetNumZeroed(Count);
	}

	void ToPcm16(const FStereo& S, TArray<uint8>& Out, const float PeakDb)
	{
		const int32 N = S.Num();
		float Peak = 0.0001f;
		for (int32 I = 0; I < N; ++I)
		{
			Peak = FMath::Max3(Peak, FMath::Abs(S.L[I]), FMath::Abs(S.R[I]));
		}
		const float Target = FMath::Pow(10.f, PeakDb / 20.f);
		const float G = Target / Peak;
		Out.SetNumUninitialized(N * 4);
		int16* Dst = reinterpret_cast<int16*>(Out.GetData());
		uint32 Dither = 0xC0FFEEu;
		for (int32 I = 0; I < N; ++I)
		{
			const float DL = White(Dither) * (1.f / 32768.f);
			const float DR = White(Dither) * (1.f / 32768.f);
			Dst[I * 2] = static_cast<int16>(FMath::Clamp(SoftClip(S.L[I] * G) + DL, -1.f, 1.f) * 32700.f);
			Dst[I * 2 + 1] = static_cast<int16>(FMath::Clamp(SoftClip(S.R[I] * G) + DR, -1.f, 1.f) * 32700.f);
		}
	}

	void AddDelayWidth(FStereo& S, const int32 Samples, const float Mix)
	{
		const int32 N = S.Num();
		const int32 D = FMath::Clamp(Samples, 1, 4000);
		for (int32 I = N - 1; I >= D; --I)
		{
			S.R[I] += S.L[I - D] * Mix;
			S.L[I] += S.R[I - D / 2] * Mix * 0.65f;
		}
	}

	void SoftLimit(FStereo& S)
	{
		const int32 N = S.Num();
		for (int32 I = 0; I < N; ++I)
		{
			S.L[I] = SoftClip(S.L[I]);
			S.R[I] = SoftClip(S.R[I]);
		}
	}
}
