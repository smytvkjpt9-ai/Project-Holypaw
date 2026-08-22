#include "Look/HolypawLook.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/StaticMeshComponent.h"
#include "Components/MeshComponent.h"
#include "Components/SceneComponent.h"
#include "Components/ActorComponent.h"
#include "Components/DirectionalLightComponent.h"
#include "Engine/Scene.h"
#include "Components/SkyLightComponent.h"
#include "Components/ExponentialHeightFogComponent.h"
#include "Components/SkyAtmosphereComponent.h"
#include "Components/VolumetricCloudComponent.h"
#include "Components/PointLightComponent.h"
#include "Camera/CameraComponent.h"
#include "Engine/PostProcessVolume.h"
#include "Engine/PointLight.h"
#include "Engine/World.h"

void HolypawLook::Paint(UMeshComponent* Mesh, const FLinearColor& Color)
{
	if (!Mesh)
	{
		return;
	}
	UMaterialInterface* Parent = Mesh->GetMaterial(0);
	if (!Parent)
	{
		return;
	}
	if (UMaterialInstanceDynamic* Mid = Mesh->CreateDynamicMaterialInstance(0, Parent))
	{
		Mid->SetVectorParameterValue(TEXT("Color"), Color);
	}
}

void HolypawLook::PrepPart(UStaticMeshComponent* Comp, UStaticMesh* Mesh)
{
	if (!Comp)
	{
		return;
	}
	Comp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Comp->SetCastShadow(true);
	Comp->SetReceivesDecals(false);
	if (Mesh)
	{
		Comp->SetStaticMesh(Mesh);
	}
}

void HolypawLook::TagSocket(UActorComponent* Comp, const TCHAR* Socket)
{
	if (!Comp || !Socket)
	{
		return;
	}
	Comp->ComponentTags.AddUnique(FName(Socket));
}

void HolypawLook::AimCatchlight(UStaticMeshComponent* Highlight, const USceneComponent* Eye, const FVector& CameraWorld)
{
	if (!Highlight || !Eye)
	{
		return;
	}
	const FVector ToCam = (CameraWorld - Eye->GetComponentLocation()).GetSafeNormal();
	if (ToCam.IsNearlyZero())
	{
		return;
	}
	const FVector Local = Eye->GetComponentTransform().InverseTransformVector(ToCam).GetSafeNormal();
	Highlight->SetRelativeLocation(Local * 11.f + FVector(3.f, 0.f, 3.f));
}

namespace
{
	HolypawLook::FHourLook LerpHour(const HolypawLook::FHourLook& A, const HolypawLook::FHourLook& B, const float T)
	{
		HolypawLook::FHourLook O;
		O.SunInt = FMath::Lerp(A.SunInt, B.SunInt, T);
		O.SkyInt = FMath::Lerp(A.SkyInt, B.SkyInt, T);
		O.FogDensity = FMath::Lerp(A.FogDensity, B.FogDensity, T);
		O.MoonInt = FMath::Lerp(A.MoonInt, B.MoonInt, T);
		O.Pitch = FMath::Lerp(A.Pitch, B.Pitch, T);
		O.Bias = FMath::Lerp(A.Bias, B.Bias, T);
		O.Temp = FMath::Lerp(A.Temp, B.Temp, T);
		O.SunCol = FMath::Lerp(A.SunCol, B.SunCol, T);
		O.FogCol = FMath::Lerp(A.FogCol, B.FogCol, T);
		O.Tint = FMath::Lerp(A.Tint, B.Tint, T);
		return O;
	}

	HolypawLook::FHourLook KeyAt(const float Hour)
	{
		HolypawLook::FHourLook K;
		if (Hour <= 0.01f || Hour >= 23.99f)
		{
			K.SunInt = 0.22f;
			K.SkyInt = 0.22f;
			K.FogDensity = 0.024f;
			K.MoonInt = 2.5f;
			K.Pitch = 16.f;
			K.Bias = -0.28f;
			K.Temp = 9000.f;
			K.SunCol = FLinearColor(0.40f, 0.50f, 0.92f);
			K.FogCol = HolypawLook::NightFog;
			K.Tint = FLinearColor(0.90f, 0.92f, 1.08f);
		}
		else if (Hour < 6.f)
		{
			K.SunInt = 0.28f;
			K.SkyInt = 0.26f;
			K.FogDensity = 0.025f;
			K.MoonInt = 2.2f;
			K.Pitch = 14.f;
			K.Bias = -0.22f;
			K.Temp = 8600.f;
			K.SunCol = FLinearColor(0.48f, 0.52f, 0.90f);
			K.FogCol = FLinearColor(0.22f, 0.26f, 0.46f);
			K.Tint = FLinearColor(0.92f, 0.93f, 1.06f);
		}
		else if (Hour < 8.f)
		{
			K.SunInt = 6.8f;
			K.SkyInt = 0.88f;
			K.FogDensity = 0.016f;
			K.MoonInt = 0.15f;
			K.Pitch = -18.f;
			K.Bias = 0.18f;
			K.Temp = 5400.f;
			K.SunCol = FLinearColor(1.f, 0.78f, 0.58f);
			K.FogCol = FLinearColor(0.90f, 0.76f, 0.66f);
			K.Tint = FLinearColor(1.05f, 0.97f, 0.92f);
		}
		else if (Hour < 16.f)
		{
			K.SunInt = 10.2f;
			K.SkyInt = 1.18f;
			K.FogDensity = 0.013f;
			K.MoonInt = 0.f;
			K.Pitch = -55.f;
			K.Bias = 0.38f;
			K.Temp = 6600.f;
			K.SunCol = FLinearColor(1.f, 0.93f, 0.82f);
			K.FogCol = FLinearColor(0.78f, 0.84f, 0.95f);
			K.Tint = FLinearColor(1.02f, 0.99f, 1.03f);
		}
		else if (Hour < 18.f)
		{
			K.SunInt = 7.4f;
			K.SkyInt = 0.95f;
			K.FogDensity = 0.016f;
			K.MoonInt = 0.2f;
			K.Pitch = -28.f;
			K.Bias = 0.22f;
			K.Temp = 5600.f;
			K.SunCol = FLinearColor(1.f, 0.80f, 0.58f);
			K.FogCol = FLinearColor(0.90f, 0.72f, 0.58f);
			K.Tint = FLinearColor(1.05f, 0.96f, 0.90f);
		}
		else if (Hour < 20.f)
		{
			K.SunInt = 3.8f;
			K.SkyInt = 0.62f;
			K.FogDensity = 0.020f;
			K.MoonInt = 0.7f;
			K.Pitch = -8.f;
			K.Bias = 0.08f;
			K.Temp = 4600.f;
			K.SunCol = FLinearColor(1.f, 0.52f, 0.34f);
			K.FogCol = FLinearColor(0.94f, 0.56f, 0.42f);
			K.Tint = FLinearColor(1.08f, 0.92f, 0.88f);
		}
		else
		{
			K.SunInt = 0.28f;
			K.SkyInt = 0.24f;
			K.FogDensity = 0.025f;
			K.MoonInt = 2.3f;
			K.Pitch = 16.f;
			K.Bias = -0.30f;
			K.Temp = 9100.f;
			K.SunCol = FLinearColor(0.42f, 0.52f, 0.95f);
			K.FogCol = HolypawLook::NightFog;
			K.Tint = FLinearColor(0.88f, 0.90f, 1.10f);
		}
		return K;
	}
}

HolypawLook::FHourLook HolypawLook::SampleHour(float Hour)
{
	Hour = FMath::Fmod(Hour + 24.f, 24.f);
	static const float Keys[] = { 0.f, 5.f, 6.5f, 8.f, 12.f, 16.f, 18.f, 20.f, 24.f };
	int32 I = 0;
	for (int32 N = 0; N < 8; ++N)
	{
		if (Hour >= Keys[N] && Hour <= Keys[N + 1])
		{
			I = N;
			break;
		}
	}
	const float Span = Keys[I + 1] - Keys[I];
	const float T = Span > 0.f ? (Hour - Keys[I]) / Span : 0.f;
	return LerpHour(KeyAt(Keys[I]), KeyAt(Keys[I + 1] >= 24.f ? 0.f : Keys[I + 1]), T);
}

void HolypawLook::GradeVolume(APostProcessVolume* PP)
{
	if (!PP)
	{
		return;
	}
	PP->bUnbound = true;
	PP->BlendWeight = 1.f;
	FPostProcessSettings& S = PP->Settings;

	S.bOverride_AutoExposureMethod = true;
	S.AutoExposureMethod = AEM_Manual;
	S.bOverride_AutoExposureBias = true;
	S.AutoExposureBias = 0.38f;

	S.bOverride_ColorSaturation = true;
	S.ColorSaturation = FVector4(1.04f, 1.02f, 1.06f, 1.f);
	S.bOverride_ColorContrast = true;
	S.ColorContrast = FVector4(1.05f, 1.03f, 1.06f, 1.f);
	S.bOverride_ColorGamma = true;
	S.ColorGamma = FVector4(1.01f, 1.00f, 1.02f, 1.f);
	S.bOverride_ColorGain = true;
	S.ColorGain = FVector4(1.02f, 0.99f, 1.03f, 1.f);
	S.bOverride_ColorOffset = true;
	S.ColorOffset = FVector4(0.006f, 0.002f, 0.008f, 0.f);

	S.bOverride_SceneColorTint = true;
	S.SceneColorTint = FLinearColor(1.02f, 0.99f, 1.03f);

	S.bOverride_WhiteTemp = true;
	S.WhiteTemp = 6600.f;
	S.bOverride_WhiteTint = true;
	S.WhiteTint = 0.02f;

	S.bOverride_BloomIntensity = true;
	S.BloomIntensity = 0.28f;
	S.bOverride_BloomThreshold = true;
	S.BloomThreshold = -0.15f;

	S.bOverride_VignetteIntensity = true;
	S.VignetteIntensity = 0.22f;
	S.bOverride_FilmGrainIntensity = true;
	S.FilmGrainIntensity = 0.025f;

	S.bOverride_AmbientOcclusionIntensity = true;
	S.AmbientOcclusionIntensity = 0.48f;
	S.bOverride_AmbientOcclusionRadius = true;
	S.AmbientOcclusionRadius = 42.f;

	S.bOverride_IndirectLightingColor = true;
	S.IndirectLightingColor = FLinearColor(1.03f, 0.97f, 1.05f);
	S.bOverride_IndirectLightingIntensity = true;
	S.IndirectLightingIntensity = 1.08f;

	S.bOverride_SceneFringeIntensity = true;
	S.SceneFringeIntensity = 0.06f;

	S.bOverride_MotionBlurAmount = true;
	S.MotionBlurAmount = 0.f;

	S.bOverride_LensFlareIntensity = true;
	S.LensFlareIntensity = 0.06f;
}

void HolypawLook::TickGrade(APostProcessVolume* PP, const float Hour, const bool bIndoors, const float MillWeight)
{
	if (!PP)
	{
		return;
	}
	FPostProcessSettings& S = PP->Settings;
	FHourLook L = SampleHour(Hour);
	if (bIndoors)
	{
		L.Bias -= 0.16f;
		L.Temp = FMath::Lerp(L.Temp, 4200.f, 0.45f);
		L.Tint = FMath::Lerp(L.Tint, FLinearColor(1.06f, 0.95f, 0.84f), 0.4f);
	}
	if (MillWeight > 0.f)
	{
		L.Bias -= 0.20f * MillWeight;
		L.Tint = FMath::Lerp(L.Tint, FLinearColor(0.92f, 0.88f, 0.78f), MillWeight);
	}
	S.AutoExposureBias = L.Bias;
	S.WhiteTemp = L.Temp;
	S.SceneColorTint = L.Tint;
}

void HolypawLook::DressSun(UDirectionalLightComponent* C)
{
	if (!C)
	{
		return;
	}
	C->SetIntensity(10.5f);
	C->SetLightColor(FLinearColor(1.f, 0.93f, 0.82f));
	C->SetAtmosphereSunLight(true);
	C->SetAtmosphereSunLightIndex(0);
	C->SetCastShadows(true);
	C->SetDynamicShadowDistanceMovableLight(110000.f);
	C->DynamicShadowCascades = 4;
	C->CascadeDistributionExponent = 2.4f;
	C->LightSourceAngle = 1.15f;
	C->LightSourceSoftAngle = 1.8f;
	C->ShadowBias = 0.12f;
	C->ShadowSlopeBias = 0.22f;
	C->ContactShadowLength = 0.12f;
	C->bCastCloudShadows = true;
	C->bEnableLightShaftBloom = true;
	C->BloomScale = 0.08f;
	C->BloomThreshold = 0.2f;
	C->SpecularScale = 0.55f;
	C->IndirectLightingIntensity = 1.25f;
	C->VolumetricScatteringIntensity = 1.15f;
}

void HolypawLook::DressFill(UDirectionalLightComponent* C)
{
	if (!C)
	{
		return;
	}
	C->SetIntensity(0.65f);
	C->SetLightColor(Powder);
	C->SetCastShadows(false);
	C->SetSpecularScale(0.08f);
	C->SetAtmosphereSunLight(false);
	C->IndirectLightingIntensity = 0.4f;
	C->VolumetricScatteringIntensity = 0.2f;
}

void HolypawLook::DressMoon(UDirectionalLightComponent* C)
{
	if (!C)
	{
		return;
	}
	C->SetIntensity(0.f);
	C->SetLightColor(FLinearColor(0.62f, 0.72f, 1.f));
	C->SetAtmosphereSunLight(true);
	C->SetAtmosphereSunLightIndex(1);
	C->SetCastShadows(true);
	C->SetDynamicShadowDistanceMovableLight(60000.f);
	C->LightSourceAngle = 0.4f;
	C->ContactShadowLength = 0.06f;
	C->SpecularScale = 0.25f;
	C->bEnableLightShaftBloom = true;
	C->BloomScale = 0.04f;
	C->IndirectLightingIntensity = 0.6f;
	C->VolumetricScatteringIntensity = 0.55f;
}

void HolypawLook::DressSky(USkyLightComponent* C)
{
	if (!C)
	{
		return;
	}
	C->SetIntensity(1.22f);
	C->SetLightColor(FLinearColor(0.74f, 0.84f, 1.f));
	C->bRealTimeCapture = true;
	C->IndirectLightingIntensity = 1.1f;
	C->VolumetricScatteringIntensity = 0.9f;
	C->LowerHemisphereColor = FLinearColor(0.42f, 0.32f, 0.28f);
	C->bLowerHemisphereIsBlack = false;
}

void HolypawLook::DressFog(UExponentialHeightFogComponent* C)
{
	if (!C)
	{
		return;
	}
	C->SetFogDensity(0.014f);
	C->SetFogHeightFalloff(0.10f);
	C->SetFogInscatteringColor(FLinearColor(0.78f, 0.84f, 0.95f));
	C->SetVolumetricFog(true);
	C->VolumetricFogScatteringDistribution = 0.42f;
	C->VolumetricFogExtinctionScale = 0.85f;
	C->VolumetricFogDistance = 22000.f;
	C->StartDistance = 400.f;
}

void HolypawLook::DressAtmosphere(USkyAtmosphereComponent* C)
{
	if (!C)
	{
		return;
	}
	C->SetAtmosphereHeight(55.f);
	C->SetMultiScatteringFactor(1.35f);
	C->SetRayleighScatteringScale(0.055f);
	C->SetRayleighScattering(FLinearColor(0.18f, 0.38f, 0.92f));
	C->SetMieScatteringScale(0.0048f);
	C->SetMieAbsorptionScale(0.0012f);
	C->SetMieAnisotropy(0.72f);
	C->SetMieScattering(FLinearColor(0.95f, 0.82f, 0.72f));
	C->SetAerialPespectiveViewDistanceScale(0.85f);
	C->SetHeightFogContribution(0.55f);
}

void HolypawLook::DressClouds(UVolumetricCloudComponent* C)
{
	if (!C)
	{
		return;
	}
	C->SetLayerBottomAltitude(4.2f);
	C->SetLayerHeight(9.5f);
	C->TracingStartMaxDistance = 280.f;
	C->TracingMaxDistance = 42.f;
	C->ViewSampleCountScale = 1.35f;
	C->ShadowViewSampleCountScale = 1.15f;
	C->ShadowTracingDistance = 12.f;
	C->bUsePerSampleAtmosphericLightTransmittance = true;
	C->SkyLightCloudBottomOcclusion = 0.42f;
}

void HolypawLook::DressLanternLight(UPointLightComponent* C, const FLinearColor& Color)
{
	if (!C)
	{
		return;
	}
	C->SetIntensity(2400.f);
	C->SetLightColor(Color);
	C->SetAttenuationRadius(720.f);
	C->SetSourceRadius(12.f);
	C->SetSoftSourceRadius(18.f);
	C->SetCastShadows(false);
	C->SetSpecularScale(0.35f);
	C->VolumetricScatteringIntensity = 0.8f;
	C->bUseInverseSquaredFalloff = true;
}

void HolypawLook::DressCamera(UCameraComponent* Camera, const bool bBattle, const float DeltaSeconds)
{
	if (!Camera)
	{
		return;
	}
	const float WantFov = bBattle ? 56.f : 66.f;
	Camera->SetFieldOfView(FMath::FInterpTo(Camera->FieldOfView, WantFov, DeltaSeconds, 5.5f));
	Camera->PostProcessBlendWeight = FMath::FInterpTo(Camera->PostProcessBlendWeight, bBattle ? 0.48f : 0.04f, DeltaSeconds, 4.5f);
	FPostProcessSettings& S = Camera->PostProcessSettings;
	S.bOverride_DepthOfFieldFstop = true;
	S.DepthOfFieldFstop = bBattle ? 3.4f : 8.5f;
	S.bOverride_DepthOfFieldMinFstop = true;
	S.DepthOfFieldMinFstop = 2.0f;
	S.bOverride_DepthOfFieldFocalDistance = true;
	S.DepthOfFieldFocalDistance = bBattle ? 270.f : 700.f;
	S.bOverride_BloomIntensity = true;
	S.BloomIntensity = bBattle ? 0.16f : 0.05f;
	S.bOverride_VignetteIntensity = true;
	S.VignetteIntensity = bBattle ? 0.16f : 0.04f;
}

void HolypawLook::SpawnGlow(UWorld* World, AActor* Owner, const FVector& Loc, const FLinearColor& Color, const float Intensity, const float Radius)
{
	if (!World)
	{
		return;
	}
	FActorSpawnParameters Sp;
	Sp.Owner = Owner;
	Sp.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	if (APointLight* Lamp = World->SpawnActor<APointLight>(Loc, FRotator::ZeroRotator, Sp))
	{
		if (UPointLightComponent* C = Lamp->FindComponentByClass<UPointLightComponent>())
		{
			C->SetIntensity(Intensity);
			C->SetLightColor(Color);
			C->SetAttenuationRadius(Radius);
			C->SetSourceRadius(8.f);
			C->SetCastShadows(false);
			C->VolumetricScatteringIntensity = 0.7f;
		}
	}
}

FLinearColor HolypawLook::HourSunColor(const float Hour)
{
	return SampleHour(Hour).SunCol;
}

float HolypawLook::HourSunIntensity(const float Hour)
{
	return SampleHour(Hour).SunInt;
}

float HolypawLook::HourSkyIntensity(const float Hour)
{
	return SampleHour(Hour).SkyInt;
}

float HolypawLook::HourSunPitch(const float Hour)
{
	return SampleHour(Hour).Pitch;
}

FLinearColor HolypawLook::HourFogColor(const float Hour)
{
	return SampleHour(Hour).FogCol;
}

float HolypawLook::HourFogDensity(const float Hour)
{
	return SampleHour(Hour).FogDensity;
}

float HolypawLook::HourMoonIntensity(const float Hour)
{
	return SampleHour(Hour).MoonInt;
}
