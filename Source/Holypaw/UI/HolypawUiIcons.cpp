#include "UI/HolypawUiIcons.h"
#include "UI/HolypawUiTheme.h"
#include "Rendering/DrawElements.h"
#include "Layout/SlateLayoutTransform.h"
#include "Styling/SlateBrush.h"

namespace HolypawUi
{
	namespace
	{
		void Box(FSlateWindowElementList& Out, int32 Layer, const FGeometry& Geo,
			FVector2D Origin, float Size, float U, float V, float W, float H, FLinearColor C)
		{
			const FVector2D Pos = Origin + FVector2D(U, V) * Size;
			const FVector2D Dim = FVector2D(W, H) * Size;
			const FPaintGeometry Paint = Geo.ToPaintGeometry(
				FVector2f(Dim.X, Dim.Y),
				FSlateLayoutTransform(FVector2f(Pos.X, Pos.Y)));
			FSlateDrawElement::MakeBox(Out, Layer, Paint, WhiteBrush(), ESlateDrawEffect::None, C);
		}

		void Seg(FSlateWindowElementList& Out, int32 Layer, const FGeometry& Geo,
			FVector2D Origin, float Size, float X0, float Y0, float X1, float Y1, FLinearColor C, float Thick = 1.8f)
		{
			TArray<FVector2f> Pts;
			Pts.Add(FVector2f(Origin.X + X0 * Size, Origin.Y + Y0 * Size));
			Pts.Add(FVector2f(Origin.X + X1 * Size, Origin.Y + Y1 * Size));
			FSlateDrawElement::MakeLines(Out, Layer, Geo.ToPaintGeometry(), Pts, ESlateDrawEffect::None, C, true, Thick);
		}
	}

	void DrawIcon(FSlateWindowElementList& Out, int32 Layer, const FGeometry& Geo,
		FVector2D Pos, float Size, EHolypawUiIcon Id, FLinearColor Color)
	{
		if (Size < 6.f || Id == EHolypawUiIcon::None)
		{
			return;
		}
		const FLinearColor Dark = FLinearColor(Color.R * 0.45f, Color.G * 0.4f, Color.B * 0.45f, Color.A);
		const FLinearColor Lite = FLinearColor(
			FMath::Min(1.f, Color.R + 0.18f),
			FMath::Min(1.f, Color.G + 0.16f),
			FMath::Min(1.f, Color.B + 0.12f),
			Color.A);

		auto B = [&](float U, float V, float W, float H, FLinearColor C)
		{
			Box(Out, Layer, Geo, Pos, Size, U, V, W, H, C);
		};
		auto L = [&](float X0, float Y0, float X1, float Y1)
		{
			Seg(Out, Layer + 1, Geo, Pos, Size, X0, Y0, X1, Y1, Lite, FMath::Max(1.2f, Size * 0.06f));
		};

		switch (Id)
		{
		case EHolypawUiIcon::Heart:
			B(0.16f, 0.18f, 0.32f, 0.34f, Color);
			B(0.52f, 0.18f, 0.32f, 0.34f, Color);
			B(0.24f, 0.40f, 0.52f, 0.28f, Color);
			B(0.34f, 0.62f, 0.32f, 0.22f, Color);
			L(0.18f, 0.34f, 0.50f, 0.88f);
			L(0.82f, 0.34f, 0.50f, 0.88f);
			break;
		case EHolypawUiIcon::Paw:
			B(0.28f, 0.42f, 0.44f, 0.40f, Color);
			B(0.08f, 0.18f, 0.18f, 0.22f, Color);
			B(0.30f, 0.08f, 0.18f, 0.22f, Color);
			B(0.52f, 0.08f, 0.18f, 0.22f, Color);
			B(0.74f, 0.18f, 0.18f, 0.22f, Color);
			L(0.28f, 0.62f, 0.72f, 0.62f);
			break;
		case EHolypawUiIcon::Halo:
			B(0.18f, 0.22f, 0.64f, 0.16f, Color);
			B(0.12f, 0.34f, 0.16f, 0.28f, Color);
			B(0.72f, 0.34f, 0.16f, 0.28f, Color);
			B(0.28f, 0.58f, 0.44f, 0.12f, Lite);
			break;
		case EHolypawUiIcon::Coin:
			B(0.22f, 0.22f, 0.56f, 0.56f, Color);
			B(0.38f, 0.34f, 0.24f, 0.32f, Dark);
			L(0.36f, 0.28f, 0.36f, 0.72f);
			break;
		case EHolypawUiIcon::Jar:
			B(0.38f, 0.08f, 0.24f, 0.16f, Dark);
			B(0.26f, 0.22f, 0.48f, 0.62f, Color);
			B(0.34f, 0.34f, 0.32f, 0.22f, Lite);
			break;
		case EHolypawUiIcon::Health:
			B(0.38f, 0.12f, 0.24f, 0.76f, Color);
			B(0.12f, 0.38f, 0.76f, 0.24f, Color);
			break;
		case EHolypawUiIcon::Lantern:
			B(0.34f, 0.06f, 0.32f, 0.12f, Dark);
			B(0.24f, 0.18f, 0.52f, 0.52f, Color);
			B(0.34f, 0.30f, 0.32f, 0.28f, Lite);
			B(0.44f, 0.70f, 0.12f, 0.22f, Dark);
			break;
		case EHolypawUiIcon::Book:
			B(0.14f, 0.18f, 0.34f, 0.64f, Color);
			B(0.52f, 0.18f, 0.34f, 0.64f, Lite);
			B(0.46f, 0.18f, 0.08f, 0.64f, Dark);
			L(0.22f, 0.36f, 0.40f, 0.36f);
			L(0.60f, 0.36f, 0.78f, 0.36f);
			break;
		case EHolypawUiIcon::Speech:
			B(0.14f, 0.14f, 0.72f, 0.52f, Color);
			B(0.28f, 0.62f, 0.22f, 0.22f, Color);
			B(0.26f, 0.28f, 0.16f, 0.12f, Dark);
			B(0.48f, 0.28f, 0.16f, 0.12f, Dark);
			B(0.70f, 0.28f, 0.08f, 0.12f, Dark);
			break;
		case EHolypawUiIcon::Stall:
			B(0.08f, 0.28f, 0.84f, 0.12f, Color);
			B(0.18f, 0.40f, 0.64f, 0.42f, Lite);
			B(0.12f, 0.16f, 0.16f, 0.16f, Dark);
			B(0.72f, 0.16f, 0.16f, 0.16f, Dark);
			break;
		case EHolypawUiIcon::Slap:
			B(0.18f, 0.38f, 0.54f, 0.36f, Color);
			B(0.08f, 0.22f, 0.20f, 0.22f, Color);
			B(0.30f, 0.12f, 0.18f, 0.22f, Color);
			B(0.50f, 0.12f, 0.18f, 0.22f, Color);
			L(0.72f, 0.30f, 0.92f, 0.18f);
			break;
		case EHolypawUiIcon::Beam:
			B(0.42f, 0.08f, 0.16f, 0.22f, Lite);
			B(0.28f, 0.28f, 0.44f, 0.16f, Color);
			B(0.36f, 0.46f, 0.28f, 0.40f, Color);
			break;
		case EHolypawUiIcon::Party:
			B(0.12f, 0.38f, 0.24f, 0.36f, Color);
			B(0.38f, 0.22f, 0.24f, 0.52f, Lite);
			B(0.64f, 0.38f, 0.24f, 0.36f, Color);
			break;
		case EHolypawUiIcon::Flee:
			B(0.12f, 0.42f, 0.46f, 0.16f, Color);
			L(0.52f, 0.28f, 0.88f, 0.50f);
			L(0.52f, 0.72f, 0.88f, 0.50f);
			L(0.58f, 0.50f, 0.88f, 0.50f);
			break;
		case EHolypawUiIcon::Guard:
			B(0.22f, 0.16f, 0.56f, 0.62f, Color);
			B(0.34f, 0.28f, 0.32f, 0.36f, Dark);
			B(0.28f, 0.70f, 0.44f, 0.12f, Lite);
			break;
		case EHolypawUiIcon::Hymn:
			B(0.28f, 0.12f, 0.16f, 0.70f, Color);
			B(0.44f, 0.28f, 0.36f, 0.18f, Lite);
			B(0.44f, 0.52f, 0.28f, 0.14f, Lite);
			break;
		case EHolypawUiIcon::Stitch:
			L(0.18f, 0.18f, 0.82f, 0.82f);
			L(0.18f, 0.82f, 0.82f, 0.18f);
			B(0.42f, 0.42f, 0.16f, 0.16f, Color);
			break;
		case EHolypawUiIcon::Mill:
			B(0.14f, 0.48f, 0.72f, 0.36f, Dark);
			B(0.26f, 0.18f, 0.18f, 0.36f, Color);
			B(0.56f, 0.10f, 0.18f, 0.44f, Color);
			B(0.40f, 0.32f, 0.12f, 0.18f, Lite);
			break;
		case EHolypawUiIcon::Pause:
			B(0.22f, 0.16f, 0.20f, 0.68f, Color);
			B(0.58f, 0.16f, 0.20f, 0.68f, Color);
			break;
		case EHolypawUiIcon::Teddy:
			B(0.28f, 0.34f, 0.44f, 0.46f, Color);
			B(0.18f, 0.12f, 0.22f, 0.28f, Color);
			B(0.60f, 0.12f, 0.22f, 0.28f, Color);
			B(0.38f, 0.46f, 0.24f, 0.16f, Dark);
			B(0.32f, 0.40f, 0.10f, 0.10f, Lite);
			B(0.56f, 0.40f, 0.10f, 0.10f, Lite);
			break;
		case EHolypawUiIcon::Ribbon:
			B(0.42f, 0.08f, 0.16f, 0.84f, Color);
			B(0.18f, 0.22f, 0.28f, 0.18f, Lite);
			B(0.54f, 0.38f, 0.28f, 0.18f, Lite);
			break;
		case EHolypawUiIcon::Save:
			B(0.18f, 0.16f, 0.64f, 0.68f, Color);
			B(0.30f, 0.16f, 0.40f, 0.28f, Dark);
			B(0.34f, 0.52f, 0.32f, 0.22f, Lite);
			break;
		case EHolypawUiIcon::Mute:
			B(0.12f, 0.34f, 0.28f, 0.32f, Color);
			B(0.36f, 0.22f, 0.18f, 0.56f, Color);
			L(0.62f, 0.28f, 0.88f, 0.72f);
			L(0.62f, 0.72f, 0.88f, 0.28f);
			break;
		case EHolypawUiIcon::Check:
			L(0.18f, 0.52f, 0.40f, 0.78f);
			L(0.40f, 0.78f, 0.84f, 0.22f);
			break;
		case EHolypawUiIcon::Arrow:
			L(0.16f, 0.50f, 0.78f, 0.50f);
			L(0.56f, 0.26f, 0.84f, 0.50f);
			L(0.56f, 0.74f, 0.84f, 0.50f);
			break;
		case EHolypawUiIcon::Bun:
			B(0.18f, 0.38f, 0.64f, 0.36f, Color);
			B(0.26f, 0.22f, 0.48f, 0.22f, Lite);
			B(0.42f, 0.16f, 0.16f, 0.12f, Dark);
			break;
		case EHolypawUiIcon::Cocoa:
			B(0.28f, 0.28f, 0.44f, 0.44f, Color);
			B(0.38f, 0.18f, 0.10f, 0.16f, Dark);
			B(0.52f, 0.18f, 0.10f, 0.16f, Dark);
			B(0.40f, 0.42f, 0.20f, 0.16f, Lite);
			break;
		case EHolypawUiIcon::MapPin:
			B(0.34f, 0.12f, 0.32f, 0.40f, Color);
			B(0.42f, 0.48f, 0.16f, 0.40f, Color);
			B(0.42f, 0.22f, 0.16f, 0.16f, Dark);
			break;
		case EHolypawUiIcon::Key:
			B(0.18f, 0.28f, 0.28f, 0.40f, Color);
			B(0.42f, 0.40f, 0.42f, 0.16f, Color);
			B(0.70f, 0.40f, 0.10f, 0.28f, Lite);
			break;
		default:
			break;
		}
	}

	EHolypawUiIcon IconForAbility(FName Id)
	{
		if (Id == TEXT("slap") || Id == TEXT("fluffBurst"))
		{
			return EHolypawUiIcon::Slap;
		}
		if (Id == TEXT("beam") || Id == TEXT("buttonBeam"))
		{
			return EHolypawUiIcon::Beam;
		}
		if (Id == TEXT("party") || Id == TEXT("cheer"))
		{
			return EHolypawUiIcon::Party;
		}
		if (Id == TEXT("flee"))
		{
			return EHolypawUiIcon::Flee;
		}
		if (Id == TEXT("guard") || Id == TEXT("seamGuard") || Id == TEXT("tuck"))
		{
			return EHolypawUiIcon::Guard;
		}
		if (Id == TEXT("hymn") || Id == TEXT("lullaby") || Id == TEXT("millHymn"))
		{
			return EHolypawUiIcon::Hymn;
		}
		if (Id == TEXT("unstuff") || Id == TEXT("polyRip"))
		{
			return EHolypawUiIcon::Mill;
		}
		if (Id == TEXT("stitch") || Id == TEXT("spareBun"))
		{
			return EHolypawUiIcon::Stitch;
		}
		if (Id == TEXT("ribbonBind"))
		{
			return EHolypawUiIcon::Ribbon;
		}
		return EHolypawUiIcon::Paw;
	}

	EHolypawUiIcon IconForItem(FName Id)
	{
		if (Id == TEXT("stuffingBun") || Id == TEXT("brineTaffy") || Id == TEXT("facelessBun"))
		{
			return EHolypawUiIcon::Bun;
		}
		if (Id == TEXT("hymnRibbon") || Id == TEXT("cherryThread") || Id == TEXT("auroraThread") || Id == TEXT("moonThread"))
		{
			return EHolypawUiIcon::Ribbon;
		}
		if (Id == TEXT("cocoaButton"))
		{
			return EHolypawUiIcon::Cocoa;
		}
		if (Id == TEXT("hymnSheet") || Id == TEXT("reefLyric") || Id == TEXT("snowSermon"))
		{
			return EHolypawUiIcon::Book;
		}
		if (Id == TEXT("millScrap"))
		{
			return EHolypawUiIcon::Mill;
		}
		return EHolypawUiIcon::Jar;
	}

	FLinearColor IconTint(EHolypawUiIcon Id)
	{
		const FPalette& P = Colors();
		switch (Id)
		{
		case EHolypawUiIcon::Heart: return P.Heart;
		case EHolypawUiIcon::Halo:
		case EHolypawUiIcon::Hymn:
		case EHolypawUiIcon::Coin: return P.Gold;
		case EHolypawUiIcon::Jar:
		case EHolypawUiIcon::Speech: return P.Powder;
		case EHolypawUiIcon::Stall:
		case EHolypawUiIcon::Check:
		case EHolypawUiIcon::Guard: return P.Mint;
		case EHolypawUiIcon::Mill:
		case EHolypawUiIcon::Flee: return P.Danger;
		case EHolypawUiIcon::Ribbon:
		case EHolypawUiIcon::Teddy: return P.Rose;
		default: return P.Cream;
		}
	}
}
