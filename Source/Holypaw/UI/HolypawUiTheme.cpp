#include "UI/HolypawUiTheme.h"
#include "Rendering/DrawElements.h"
#include "Styling/CoreStyle.h"
#include "Layout/SlateLayoutTransform.h"
#include "Brushes/SlateColorBrush.h"

namespace HolypawUi
{
	const FPalette& Colors()
	{
		static const FPalette P;
		return P;
	}

	const FSlateBrush* WhiteBrush()
	{
		static FSlateColorBrush Brush(FLinearColor::White);
		return &Brush;
	}

	FLinearColor HeartsHeat(int32 Hearts)
	{
		if (Hearts <= 0)
		{
			return FLinearColor(0.55f, 0.62f, 0.78f, 0.85f);
		}
		if (Hearts == 1)
		{
			return Colors().Rose;
		}
		if (Hearts == 2)
		{
			return FLinearColor(0.95f, 0.52f, 0.48f);
		}
		return Colors().Gold;
	}

	FLinearColor WithAlpha(FLinearColor C, float A)
	{
		C.A = A;
		return C;
	}

	FVector2D Centered(FVector2D Canvas, FVector2D Panel)
	{
		return FVector2D((Canvas.X - Panel.X) * 0.5f, FMath::Max(24.f, (Canvas.Y - Panel.Y) * 0.42f));
	}

	FVector2D FPaint::Canvas() const
	{
		return Geo.GetLocalSize();
	}

	void FPaint::Fill(FVector2D Pos, FVector2D Size, FLinearColor Color) const
	{
		const FPaintGeometry Paint = Geo.ToPaintGeometry(
			FVector2f(Size.X, Size.Y),
			FSlateLayoutTransform(FVector2f(Pos.X, Pos.Y)));
		FSlateDrawElement::MakeBox(Out, Layer, Paint, WhiteBrush(), ESlateDrawEffect::None, Color);
	}

	void FPaint::Text(FVector2D Pos, const FString& S, FLinearColor Color, float Scale, float MaxW) const
	{
		FSlateFontInfo Font = FCoreStyle::GetDefaultFontStyle("Bold", FMath::Max(10, FMath::RoundToInt(16.f * Scale)));
		const FPaintGeometry Paint = Geo.ToPaintGeometry(
			FVector2f(MaxW, 42.f),
			FSlateLayoutTransform(FVector2f(Pos.X, Pos.Y)));
		FSlateDrawElement::MakeText(Out, Layer + 2, Paint, S, Font, ESlateDrawEffect::None, Color);
	}

	void FPaint::Text(FVector2D Pos, const FText& T, FLinearColor Color, float Scale, float MaxW) const
	{
		Text(Pos, T.ToString(), Color, Scale, MaxW);
	}

	void FPaint::Line(FVector2D A, FVector2D B, FLinearColor Color, float Thickness) const
	{
		TArray<FVector2f> Pts;
		Pts.Add(FVector2f(A.X, A.Y));
		Pts.Add(FVector2f(B.X, B.Y));
		Poly(Pts, Color, Thickness);
	}

	void FPaint::Poly(const TArray<FVector2f>& Pts, FLinearColor Color, float Thickness) const
	{
		if (Pts.Num() < 2)
		{
			return;
		}
		FSlateDrawElement::MakeLines(Out, Layer + 1, Geo.ToPaintGeometry(), Pts, ESlateDrawEffect::None, Color, true, Thickness);
	}

	void FPaint::DashRect(FVector2D Pos, FVector2D Size, FLinearColor Color, float Thickness, float Dash) const
	{
		auto DashEdge = [&](FVector2D A, FVector2D B)
		{
			const FVector2D Delta = B - A;
			const float Len = Delta.Size();
			if (Len < 1.f)
			{
				return;
			}
			const FVector2D Dir = Delta / Len;
			const float Gap = Dash * 0.7f;
			float T = 0.f;
			while (T < Len)
			{
				const float T1 = FMath::Min(T + Dash, Len);
				Line(A + Dir * T, A + Dir * T1, Color, Thickness);
				T += Dash + Gap;
			}
		};
		const FVector2D A = Pos;
		const FVector2D B(Pos.X + Size.X, Pos.Y);
		const FVector2D C(Pos.X + Size.X, Pos.Y + Size.Y);
		const FVector2D D(Pos.X, Pos.Y + Size.Y);
		DashEdge(A, B);
		DashEdge(B, C);
		DashEdge(C, D);
		DashEdge(D, A);
	}

	void FPaint::CornerKnots(FVector2D Pos, FVector2D Size, FLinearColor Color) const
	{
		const float K = 8.f;
		const FVector2D Corners[4] = {
			Pos + FVector2D(6.f, 6.f),
			Pos + FVector2D(Size.X - 6.f, 6.f),
			Pos + FVector2D(Size.X - 6.f, Size.Y - 6.f),
			Pos + FVector2D(6.f, Size.Y - 6.f)
		};
		for (int32 I = 0; I < 4; ++I)
		{
			Line(Corners[I] + FVector2D(-K, -K), Corners[I] + FVector2D(K, K), Color, 1.6f);
			Line(Corners[I] + FVector2D(-K, K), Corners[I] + FVector2D(K, -K), Color, 1.6f);
		}
	}

	void FPaint::SoftShadow(FVector2D Pos, FVector2D Size) const
	{
		Fill(Pos + FVector2D(8.f, 10.f), Size, FLinearColor(0.02f, 0.01f, 0.03f, 0.35f));
	}

	void FPaint::Panel(FVector2D Pos, FVector2D Size, FLinearColor FillColor) const
	{
		const FPalette& P = Colors();
		if (FillColor.A <= 0.f)
		{
			FillColor = P.Felt;
		}
		SoftShadow(Pos, Size);
		Fill(Pos, Size, WithAlpha(P.Ink, 0.95f));
		Fill(Pos + FVector2D(5.f, 5.f), Size - FVector2D(10.f, 10.f), FillColor);
		Fill(Pos + FVector2D(8.f, 8.f), Size - FVector2D(16.f, 16.f), P.FeltInner);
		DashRect(Pos + FVector2D(10.f, 10.f), Size - FVector2D(20.f, 20.f), P.Gold, 1.8f, 8.f);
		CornerKnots(Pos, Size, P.Rose);
	}

	void FPaint::Chip(FVector2D Pos, FVector2D Size, EHolypawUiIcon IconId, const FString& Label, FLinearColor Accent) const
	{
		Fill(Pos, Size, Colors().FeltInner);
		DashRect(Pos + FVector2D(3.f, 3.f), Size - FVector2D(6.f, 6.f), Accent, 1.3f, 6.f);
		const float IconSize = FMath::Min(Size.Y - 10.f, 22.f);
		Icon(Pos + FVector2D(8.f, (Size.Y - IconSize) * 0.5f), IconSize, IconId, Accent);
		Text(Pos + FVector2D(8.f + IconSize + 8.f, Size.Y * 0.28f), Label, Colors().Cream, 0.85f, Size.X - 40.f);
	}

	void FPaint::Keycap(FVector2D Pos, const FString& Key) const
	{
		Fill(Pos, FVector2D(28.f, 22.f), Colors().Ink);
		Fill(Pos + FVector2D(1.f, 1.f), FVector2D(26.f, 20.f), Colors().Gold);
		Text(Pos + FVector2D(7.f, 2.f), Key, Colors().Ink, 0.8f, 40.f);
	}

	void FPaint::Bar(FVector2D Pos, FVector2D Size, float Pct, FLinearColor FillColor, FLinearColor Back) const
	{
		if (Back.A <= 0.f)
		{
			Back = Colors().BarBack;
		}
		Pct = FMath::Clamp(Pct, 0.f, 1.f);
		Fill(Pos, Size, Back);
		if (Pct > 0.01f)
		{
			Fill(Pos, FVector2D(Size.X * Pct, Size.Y), FillColor);
		}
		DashRect(Pos, Size, WithAlpha(Colors().Cream, 0.55f), 1.1f, 5.f);
	}

	void FPaint::Icon(FVector2D Pos, float Size, EHolypawUiIcon Id, FLinearColor Color) const
	{
		DrawIcon(Out, Layer + 2, Geo, Pos, Size, Id, Color);
	}

	void FPaint::HeartRow(FVector2D Pos, int32 Hearts, float Size, int32 MaxShow) const
	{
		const int32 Show = FMath::Clamp(Hearts, 0, MaxShow);
		for (int32 I = 0; I < MaxShow; ++I)
		{
			const FLinearColor C = I < Show ? HeartsHeat(Hearts) : WithAlpha(Colors().Powder, 0.28f);
			Icon(Pos + FVector2D(I * (Size + 4.f), 0.f), Size, EHolypawUiIcon::Heart, C);
		}
		if (Hearts > MaxShow)
		{
			Text(Pos + FVector2D(MaxShow * (Size + 4.f) + 4.f, 0.f),
				FString::Printf(TEXT("+%d"), Hearts - MaxShow), Colors().Gold, 0.8f, 80.f);
		}
	}

	void FPaint::Rule(FVector2D Pos, float Width, FLinearColor Color) const
	{
		Fill(Pos, FVector2D(Width, 2.f), Color);
	}

	void FPaint::Caption(FVector2D Pos, EHolypawUiIcon IconId, const FString& Title, FLinearColor Accent) const
	{
		Icon(Pos, 28.f, IconId, Accent);
		Text(Pos + FVector2D(36.f, 2.f), Title, Accent, 1.35f, 640.f);
	}

	void FPaint::Footer(FVector2D PanelPos, FVector2D PanelSize, const FString& Hint) const
	{
		Text(PanelPos + FVector2D(28.f, PanelSize.Y - 42.f), Hint, Colors().Muted, 0.85f, PanelSize.X - 50.f);
	}

	void FPaint::VerbRow(FVector2D Pos, const FString& Key, EHolypawUiIcon IconId, const FString& Label, bool bHot) const
	{
		Fill(Pos, FVector2D(210.f, 40.f), bHot ? Colors().Select : Colors().Idle);
		DashRect(Pos + FVector2D(3.f, 3.f), FVector2D(204.f, 34.f), bHot ? Colors().Gold : Colors().Muted, 1.2f, 5.f);
		Keycap(Pos + FVector2D(8.f, 9.f), Key);
		Icon(Pos + FVector2D(44.f, 8.f), 24.f, IconId, bHot ? Colors().Gold : Colors().Powder);
		Text(Pos + FVector2D(74.f, 10.f), Label, Colors().Cream, 0.9f, 130.f);
	}

	void FPaint::SlotCard(FVector2D Pos, FVector2D Size, bool bSelected, const FString& Title, const FString& Body) const
	{
		Fill(Pos, Size, bSelected ? Colors().Select : Colors().Idle);
		DashRect(Pos + FVector2D(5.f, 5.f), Size - FVector2D(10.f, 10.f), bSelected ? Colors().Gold : Colors().Rose, 1.5f, 7.f);
		Icon(Pos + FVector2D(16.f, (Size.Y - 32.f) * 0.5f), 32.f, EHolypawUiIcon::Teddy, bSelected ? Colors().Gold : Colors().Rose);
		Text(Pos + FVector2D(58.f, 12.f), Title, bSelected ? Colors().Gold : Colors().Cream, 1.05f, Size.X - 80.f);
		Text(Pos + FVector2D(58.f, 40.f), Body, Colors().Powder, 0.85f, Size.X - 80.f);
	}
}
