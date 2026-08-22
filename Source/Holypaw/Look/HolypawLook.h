#pragma once

#include "CoreMinimal.h"

class UMeshComponent;
class UStaticMeshComponent;
class UStaticMesh;
class UMaterialInterface;
class UDirectionalLightComponent;
class USkyLightComponent;
class UExponentialHeightFogComponent;
class USkyAtmosphereComponent;
class UVolumetricCloudComponent;
class UPointLightComponent;
class UCameraComponent;
class UActorComponent;
class APostProcessVolume;
class APointLight;
class AActor;
class UWorld;
class USceneComponent;

/**
 * Presentation bible for the painterly / plush look.
 * Palette, lighting grade, and kitbash helpers. No Megascans. No photoscans.
 * Named sockets on the teddy are the FBX contract for later art drops.
 */
namespace HolypawLook
{
	// Art bible: rose, mint, gold, powder blue, cream, velvet.
	const FLinearColor Rose(0.86f, 0.48f, 0.58f);
	const FLinearColor RoseDeep(0.72f, 0.32f, 0.42f);
	const FLinearColor Mint(0.48f, 0.78f, 0.70f);
	const FLinearColor MintDeep(0.28f, 0.52f, 0.42f);
	const FLinearColor Gold(0.98f, 0.82f, 0.42f);
	const FLinearColor GoldWarm(1.00f, 0.88f, 0.55f);
	const FLinearColor Powder(0.62f, 0.78f, 0.96f);
	const FLinearColor Cream(0.98f, 0.92f, 0.84f);
	const FLinearColor Fur(0.93f, 0.64f, 0.56f);
	const FLinearColor FurLight(0.96f, 0.76f, 0.70f);
	const FLinearColor Snout(0.98f, 0.86f, 0.80f);
	const FLinearColor Pad(0.82f, 0.42f, 0.48f);
	const FLinearColor InnerEar(0.96f, 0.62f, 0.68f);
	const FLinearColor Button(0.10f, 0.08f, 0.10f);
	const FLinearColor Catchlight(1.00f, 0.98f, 0.94f);
	const FLinearColor Velvet(0.42f, 0.22f, 0.38f);
	const FLinearColor Skin(0.96f, 0.80f, 0.74f);
	const FLinearColor BelieverGold(1.00f, 0.88f, 0.55f);
	const FLinearColor Polyester(0.42f, 0.40f, 0.38f);
	const FLinearColor NightFog(0.18f, 0.22f, 0.42f);
	const FLinearColor Lantern(1.00f, 0.82f, 0.38f);
	const FLinearColor Water(0.28f, 0.58f, 0.78f);
	const FLinearColor Foam(0.88f, 0.94f, 0.98f);
	const FLinearColor Wood(0.42f, 0.28f, 0.18f);
	const FLinearColor Roof(0.55f, 0.26f, 0.32f);
	const FLinearColor Glass(0.55f, 0.82f, 0.95f);
	const FLinearColor GrassTuft(0.38f, 0.68f, 0.36f);
	const FLinearColor BloomPink(0.92f, 0.48f, 0.62f);

	struct FHourLook
	{
		float SunInt = 10.f;
		float SkyInt = 1.16f;
		float FogDensity = 0.009f;
		float MoonInt = 0.f;
		float Pitch = -58.f;
		float Bias = 0.36f;
		float Temp = 6500.f;
		FLinearColor SunCol = FLinearColor(1.f, 0.95f, 0.88f);
		FLinearColor FogCol = FLinearColor(0.80f, 0.86f, 0.94f);
		FLinearColor Tint = FLinearColor(1.00f, 0.995f, 0.99f);
	};

	void Paint(UMeshComponent* Mesh, const FLinearColor& Color);
	void PrepPart(UStaticMeshComponent* Comp, UStaticMesh* Mesh);
	void TagSocket(UActorComponent* Comp, const TCHAR* Socket);
	void AimCatchlight(UStaticMeshComponent* Highlight, const USceneComponent* Eye, const FVector& CameraWorld);

	FHourLook SampleHour(float Hour);

	void GradeVolume(APostProcessVolume* PP);
	void TickGrade(APostProcessVolume* PP, float Hour, bool bIndoors, float MillWeight);
	void DressSun(UDirectionalLightComponent* C);
	void DressFill(UDirectionalLightComponent* C);
	void DressMoon(UDirectionalLightComponent* C);
	void DressSky(USkyLightComponent* C);
	void DressFog(UExponentialHeightFogComponent* C);
	void DressAtmosphere(USkyAtmosphereComponent* C);
	void DressClouds(UVolumetricCloudComponent* C);
	void DressLanternLight(UPointLightComponent* C, const FLinearColor& Color = Lantern);
	void DressCamera(UCameraComponent* Camera, bool bBattle, float DeltaSeconds);
	void ApplyViewExposure(UCameraComponent* Camera, float Bias = 1.85f);

	void SpawnGlow(UWorld* World, AActor* Owner, const FVector& Loc, const FLinearColor& Color, float Intensity, float Radius);

	FLinearColor HourSunColor(float Hour);
	float HourSunIntensity(float Hour);
	float HourSkyIntensity(float Hour);
	float HourSunPitch(float Hour);
	FLinearColor HourFogColor(float Hour);
	float HourFogDensity(float Hour);
	float HourMoonIntensity(float Hour);
}
