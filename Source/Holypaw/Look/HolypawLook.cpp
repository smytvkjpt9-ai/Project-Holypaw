#include "Look/HolypawLook.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/StaticMeshComponent.h"
#include "Components/MeshComponent.h"
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
	S.AutoExposureBias = 0.42f;

	S.bOverride_ColorSaturation = true;
	S.ColorSaturation = FVector4(1.10f, 1.04f, 1.12f, 1.f);
	S.bOverride_ColorContrast = true;
	S.ColorContrast = FVector4(1.06f, 1.04f, 1.08f, 1.f);
	S.bOverride_ColorGamma = true;
	S.ColorGamma = FVector4(1.02f, 0.99f, 1.04f, 1.f);
	S.bOverride_ColorGain = true;
	S.ColorGain = FVector4(1.04f, 0.98f, 1.06f, 1.f);
	S.bOverride_ColorOffset = true;
	S.ColorOffset = FVector4(0.012f, 0.004f, 0.018f, 0.f);

	S.bOverride_SceneColorTint = true;
	S.SceneColorTint = FLinearColor(1.02f, 0.98f, 1.04f);

	S.bOverride_WhiteTemp = true;
	S.WhiteTemp = 6800.f;
	S.bOverride_WhiteTint = true;
	S.WhiteTint = 0.04f;

	S.bOverride_BloomIntensity = true;
	S.BloomIntensity = 0.48f;
	S.bOverride_BloomThreshold = true;
	S.BloomThreshold = -1.1f;

	S.bOverride_VignetteIntensity = true;
	S.VignetteIntensity = 0.32f;
	S.bOverride_FilmGrainIntensity = true;
	S.FilmGrainIntensity = 0.035f;

	S.bOverride_AmbientOcclusionIntensity = true;
	S.AmbientOcclusionIntensity = 0.55f;
	S.bOverride_AmbientOcclusionRadius = true;
	S.AmbientOcclusionRadius = 48.f;

	S.bOverride_IndirectLightingColor = true;
	S.IndirectLightingColor = FLinearColor(1.05f, 0.96f, 1.08f);
	S.bOverride_IndirectLightingIntensity = true;
	S.IndirectLightingIntensity = 1.15f;

	S.bOverride_SceneFringeIntensity = true;
	S.SceneFringeIntensity = 0.35f;

	S.bOverride_MotionBlurAmount = true;
	S.MotionBlurAmount = 0.f;

	S.bOverride_LensFlareIntensity = true;
	S.LensFlareIntensity = 0.12f;
}

void HolypawLook::TickGrade(APostProcessVolume* PP, const float Hour, const bool bIndoors, const float MillWeight)
{
	if (!PP)
	{
		return;
	}
	FPostProcessSettings& S = PP->Settings;
	float Bias = 0.42f;
	float Temp = 6800.f;
	FLinearColor Tint(1.02f, 0.98f, 1.04f);
	if (Hour >= 17.f && Hour < 20.f)
	{
		Bias = 0.18f;
		Temp = 4800.f;
		Tint = FLinearColor(1.08f, 0.92f, 0.88f);
	}
	else if (Hour < 5.f || Hour >= 20.f)
	{
		Bias = -0.35f;
		Temp = 9200.f;
		Tint = FLinearColor(0.88f, 0.90f, 1.12f);
	}
	else if (Hour >= 5.f && Hour < 8.f)
	{
		Bias = 0.22f;
		Temp = 5600.f;
		Tint = FLinearColor(1.06f, 0.96f, 0.90f);
	}
	if (bIndoors)
	{
		Bias -= 0.18f;
		Temp = FMath::Lerp(Temp, 4200.f, 0.45f);
		Tint = FMath::Lerp(Tint, FLinearColor(1.08f, 0.94f, 0.82f), 0.4f);
	}
	if (MillWeight > 0.f)
	{
		Bias -= 0.22f * MillWeight;
		Tint = FMath::Lerp(Tint, FLinearColor(0.92f, 0.88f, 0.78f), MillWeight);
	}
	S.AutoExposureBias = Bias;
	S.WhiteTemp = Temp;
	S.SceneColorTint = Tint;
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
	C->SetIntensity(1.35f);
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
	const float WantFov = bBattle ? 54.f : 67.f;
	Camera->SetFieldOfView(FMath::FInterpTo(Camera->FieldOfView, WantFov, DeltaSeconds, 5.5f));
	Camera->PostProcessBlendWeight = FMath::FInterpTo(Camera->PostProcessBlendWeight, bBattle ? 0.62f : 0.12f, DeltaSeconds, 4.5f);
	FPostProcessSettings& S = Camera->PostProcessSettings;
	S.bOverride_DepthOfFieldFstop = true;
	S.DepthOfFieldFstop = bBattle ? 2.2f : 5.6f;
	S.bOverride_DepthOfFieldMinFstop = true;
	S.DepthOfFieldMinFstop = 1.6f;
	S.bOverride_DepthOfFieldFocalDistance = true;
	S.DepthOfFieldFocalDistance = bBattle ? 280.f : 620.f;
	S.bOverride_BloomIntensity = true;
	S.BloomIntensity = bBattle ? 0.22f : 0.08f;
	S.bOverride_VignetteIntensity = true;
	S.VignetteIntensity = bBattle ? 0.22f : 0.08f;
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
	if (Hour >= 17.f && Hour < 20.f)
	{
		return FLinearColor(1.f, 0.52f, 0.34f);
	}
	if (Hour < 5.f || Hour >= 20.f)
	{
		return FLinearColor(0.42f, 0.52f, 0.95f);
	}
	if (Hour >= 5.f && Hour < 8.f)
	{
		return FLinearColor(1.f, 0.78f, 0.58f);
	}
	return FLinearColor(1.f, 0.93f, 0.82f);
}

float HolypawLook::HourSunIntensity(const float Hour)
{
	if (Hour >= 17.f && Hour < 20.f)
	{
		return 4.6f;
	}
	if (Hour < 5.f || Hour >= 20.f)
	{
		return 0.35f;
	}
	if (Hour >= 5.f && Hour < 8.f)
	{
		return 7.2f;
	}
	return 10.5f;
}

float HolypawLook::HourSkyIntensity(const float Hour)
{
	if (Hour >= 17.f && Hour < 20.f)
	{
		return 0.72f;
	}
	if (Hour < 5.f || Hour >= 20.f)
	{
		return 0.24f;
	}
	if (Hour >= 5.f && Hour < 8.f)
	{
		return 0.92f;
	}
	return 1.22f;
}

float HolypawLook::HourSunPitch(const float Hour)
{
	if (Hour >= 5.f && Hour < 20.f)
	{
		const float T = (Hour - 5.f) / 15.f;
		return FMath::Lerp(-10.f, -72.f, FMath::Sin(T * PI));
	}
	return 18.f;
}

FLinearColor HolypawLook::HourFogColor(const float Hour)
{
	if (Hour >= 17.f && Hour < 20.f)
	{
		return FLinearColor(0.94f, 0.58f, 0.42f);
	}
	if (Hour < 5.f || Hour >= 20.f)
	{
		return NightFog;
	}
	if (Hour >= 5.f && Hour < 8.f)
	{
		return FLinearColor(0.90f, 0.78f, 0.68f);
	}
	return FLinearColor(0.78f, 0.84f, 0.95f);
}

float HolypawLook::HourFogDensity(const float Hour)
{
	if (Hour >= 17.f && Hour < 20.f)
	{
		return 0.020f;
	}
	if (Hour < 5.f || Hour >= 20.f)
	{
		return 0.026f;
	}
	return 0.014f;
}

float HolypawLook::HourMoonIntensity(const float Hour)
{
	if (Hour < 5.f || Hour >= 20.f)
	{
		return 2.4f;
	}
	if (Hour >= 17.f && Hour < 20.f)
	{
		return 0.55f;
	}
	return 0.f;
}
