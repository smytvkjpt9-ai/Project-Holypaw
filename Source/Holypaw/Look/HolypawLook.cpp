#include "Look/HolypawLook.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialInterface.h"
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
#include "GameFramework/Actor.h"

void HolypawLook::Paint(UMeshComponent* Mesh, const FLinearColor& Color)
{
	if (!Mesh)
	{
		return;
	}
	UMaterialInterface* Parent = Mesh->GetMaterial(0);
	if (!Parent)
	{
		Parent = LoadObject<UMaterialInterface>(nullptr, TEXT("/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial"));
		if (Parent)
		{
			Mesh->SetMaterial(0, Parent);
		}
	}
	if (!Parent)
	{
		return;
	}
	const FLinearColor Bright = Color * 1.18f + FLinearColor(0.05f, 0.04f, 0.05f);
	if (UMaterialInstanceDynamic* Mid = Mesh->CreateDynamicMaterialInstance(0, Parent))
	{
		Mid->SetVectorParameterValue(TEXT("Color"), Bright);
		Mid->SetVectorParameterValue(TEXT("BaseColor"), Bright);
		Mid->SetVectorParameterValue(TEXT("Base Color"), Bright);
		Mid->SetScalarParameterValue(TEXT("Roughness"), 0.82f);
		Mid->SetScalarParameterValue(TEXT("Metallic"), 0.f);
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
	Highlight->SetRelativeLocation(Local * 9.f + FVector(2.f, 0.f, 2.f));
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
		// Exact keyframes. Do not bucket — 8am used to return the noon look.
		HolypawLook::FHourLook K;
		const int32 H = FMath::RoundToInt(Hour * 10.f);
		if (H <= 1 || H >= 239)
		{
			K.SunInt = 0.16f;
			K.SkyInt = 0.18f;
			K.FogDensity = 0.016f;
			K.MoonInt = 1.15f;
			K.Pitch = 18.f;
			K.Bias = -0.20f;
			K.Temp = 8800.f;
			K.SunCol = FLinearColor(0.38f, 0.48f, 0.88f);
			K.FogCol = HolypawLook::NightFog;
			K.Tint = FLinearColor(0.92f, 0.94f, 1.06f);
		}
		else if (H <= 51)
		{
			K.SunInt = 0.20f;
			K.SkyInt = 0.22f;
			K.FogDensity = 0.016f;
			K.MoonInt = 1.0f;
			K.Pitch = 12.f;
			K.Bias = -0.16f;
			K.Temp = 8200.f;
			K.SunCol = FLinearColor(0.50f, 0.54f, 0.86f);
			K.FogCol = FLinearColor(0.24f, 0.28f, 0.44f);
			K.Tint = FLinearColor(0.94f, 0.94f, 1.04f);
		}
		else if (H <= 66)
		{
			K.SunInt = 5.2f;
			K.SkyInt = 0.72f;
			K.FogDensity = 0.011f;
			K.MoonInt = 0.12f;
			K.Pitch = -16.f;
			K.Bias = 0.14f;
			K.Temp = 5000.f;
			K.SunCol = FLinearColor(1.f, 0.76f, 0.54f);
			K.FogCol = FLinearColor(0.88f, 0.74f, 0.62f);
			K.Tint = FLinearColor(1.04f, 0.97f, 0.90f);
		}
		else if (H <= 81)
		{
			K.SunInt = 8.2f;
			K.SkyInt = 1.02f;
			K.FogDensity = 0.009f;
			K.MoonInt = 0.f;
			K.Pitch = -34.f;
			K.Bias = 0.28f;
			K.Temp = 6000.f;
			K.SunCol = FLinearColor(1.f, 0.90f, 0.76f);
			K.FogCol = FLinearColor(0.82f, 0.84f, 0.90f);
			K.Tint = FLinearColor(1.01f, 0.99f, 0.97f);
		}
		else if (H <= 121)
		{
			K.SunInt = 10.f;
			K.SkyInt = 1.16f;
			K.FogDensity = 0.008f;
			K.MoonInt = 0.f;
			K.Pitch = -58.f;
			K.Bias = 0.36f;
			K.Temp = 6500.f;
			K.SunCol = FLinearColor(1.f, 0.95f, 0.88f);
			K.FogCol = FLinearColor(0.80f, 0.86f, 0.94f);
			K.Tint = FLinearColor(1.00f, 0.995f, 0.99f);
		}
		else if (H <= 161)
		{
			K.SunInt = 8.6f;
			K.SkyInt = 1.04f;
			K.FogDensity = 0.009f;
			K.MoonInt = 0.f;
			K.Pitch = -36.f;
			K.Bias = 0.28f;
			K.Temp = 5800.f;
			K.SunCol = FLinearColor(1.f, 0.88f, 0.72f);
			K.FogCol = FLinearColor(0.86f, 0.80f, 0.70f);
			K.Tint = FLinearColor(1.02f, 0.98f, 0.94f);
		}
		else if (H <= 181)
		{
			K.SunInt = 5.8f;
			K.SkyInt = 0.82f;
			K.FogDensity = 0.011f;
			K.MoonInt = 0.18f;
			K.Pitch = -18.f;
			K.Bias = 0.16f;
			K.Temp = 4600.f;
			K.SunCol = FLinearColor(1.f, 0.72f, 0.48f);
			K.FogCol = FLinearColor(0.92f, 0.68f, 0.48f);
			K.Tint = FLinearColor(1.05f, 0.96f, 0.88f);
		}
		else if (H <= 201)
		{
			K.SunInt = 2.0f;
			K.SkyInt = 0.42f;
			K.FogDensity = 0.014f;
			K.MoonInt = 0.55f;
			K.Pitch = -6.f;
			K.Bias = 0.02f;
			K.Temp = 3800.f;
			K.SunCol = FLinearColor(1.f, 0.48f, 0.30f);
			K.FogCol = FLinearColor(0.72f, 0.42f, 0.38f);
			K.Tint = FLinearColor(1.04f, 0.92f, 0.88f);
		}
		else
		{
			K.SunInt = 0.18f;
			K.SkyInt = 0.20f;
			K.FogDensity = 0.016f;
			K.MoonInt = 1.1f;
			K.Pitch = 14.f;
			K.Bias = -0.18f;
			K.Temp = 9000.f;
			K.SunCol = FLinearColor(0.40f, 0.50f, 0.90f);
			K.FogCol = HolypawLook::NightFog;
			K.Tint = FLinearColor(0.90f, 0.92f, 1.08f);
		}
		return K;
	}
}

HolypawLook::FHourLook HolypawLook::SampleHour(float Hour)
{
	Hour = FMath::Fmod(Hour + 24.f, 24.f);
	static const float Keys[] = { 0.f, 5.f, 6.5f, 8.f, 12.f, 16.f, 18.f, 20.f, 22.f, 24.f };
	int32 I = 0;
	for (int32 N = 0; N < 9; ++N)
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
	S.AutoExposureBias = 0.92f;

	S.bOverride_ColorSaturation = true;
	S.ColorSaturation = FVector4(1.02f, 1.02f, 1.02f, 1.f);
	S.bOverride_ColorContrast = true;
	S.ColorContrast = FVector4(1.04f, 1.04f, 1.05f, 1.f);
	S.bOverride_ColorGamma = true;
	S.ColorGamma = FVector4(1.00f, 1.00f, 1.00f, 1.f);
	S.bOverride_ColorGain = true;
	S.ColorGain = FVector4(1.00f, 0.995f, 0.99f, 1.f);
	S.bOverride_ColorOffset = true;
	S.ColorOffset = FVector4(0.002f, 0.002f, 0.003f, 0.f);

	S.bOverride_SceneColorTint = true;
	S.SceneColorTint = FLinearColor(1.00f, 0.995f, 0.99f);

	S.bOverride_WhiteTemp = true;
	S.WhiteTemp = 6500.f;
	S.bOverride_WhiteTint = true;
	S.WhiteTint = 0.0f;

	S.bOverride_BloomIntensity = true;
	S.BloomIntensity = 0.18f;
	S.bOverride_BloomThreshold = true;
	S.BloomThreshold = 0.08f;

	S.bOverride_VignetteIntensity = true;
	S.VignetteIntensity = 0.18f;
	S.bOverride_FilmGrainIntensity = true;
	S.FilmGrainIntensity = 0.02f;

	S.bOverride_AmbientOcclusionIntensity = true;
	S.AmbientOcclusionIntensity = 0.42f;
	S.bOverride_AmbientOcclusionRadius = true;
	S.AmbientOcclusionRadius = 42.f;

	S.bOverride_IndirectLightingColor = true;
	S.IndirectLightingColor = FLinearColor(1.01f, 0.99f, 1.00f);
	S.bOverride_IndirectLightingIntensity = true;
	S.IndirectLightingIntensity = 1.04f;

	S.bOverride_SceneFringeIntensity = true;
	S.SceneFringeIntensity = 0.025f;

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
	S.AutoExposureBias = L.Bias + 0.42f;
	S.WhiteTemp = L.Temp;
	S.SceneColorTint = L.Tint;
}

void HolypawLook::DressSun(UDirectionalLightComponent* C, const bool bLinkAtmosphere)
{
	if (!C)
	{
		return;
	}
	C->SetIntensity(bLinkAtmosphere ? 10.5f : 14.f);
	C->SetLightColor(FLinearColor(1.f, 0.93f, 0.82f));
	C->SetAtmosphereSunLight(bLinkAtmosphere);
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
	C->SetIntensity(0.62f);
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
	C->SetIntensity(1.45f);
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
	C->SetFogDensity(0.0045f);
	C->SetFogHeightFalloff(0.10f);
	C->SetFogInscatteringColor(FLinearColor(0.78f, 0.84f, 0.95f));
	C->SetVolumetricFog(false);
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

void HolypawLook::ApplyViewExposure(UCameraComponent* Camera, const float Bias)
{
	if (!Camera)
	{
		return;
	}
	FPostProcessSettings& S = Camera->PostProcessSettings;
	S.bOverride_AutoExposureMethod = true;
	S.AutoExposureMethod = AEM_Manual;
	S.bOverride_AutoExposureBias = true;
	S.AutoExposureBias = Bias;
	Camera->PostProcessBlendWeight = FMath::Max(Camera->PostProcessBlendWeight, 0.65f);
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
