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
		return FVector2D((Canvas.X - Panel.X) * 0.5f, FMath::Max(16.f, (Canvas.Y - Panel.Y) * 0.5f));
	}

	FVector2D Fit(FVector2D Canvas, FVector2D Desired, float Margin)
	{
		return FVector2D(
			FMath::Min(Desired.X, FMath::Max(280.f, Canvas.X - Margin * 2.f)),
			FMath::Min(Desired.Y, FMath::Max(220.f, Canvas.Y - Margin * 2.f)));
	}

	static float ApproxCharW(float Scale)
	{
		return FMath::Max(6.4f, 16.f * Scale * 0.54f);
	}

	float TextWidth(const FString& S, float Scale)
	{
		return S.Len() * ApproxCharW(Scale);
	}

	float KeyWidth(const FString& Key)
	{
		return FMath::Max(28.f, 14.f + Key.Len() * 8.5f);
	}

	int32 WrapLines(const FString& S, float MaxW, float Scale, int32 MaxLines, TArray<FString>& OutLines)
	{
		OutLines.Reset();
		if (S.IsEmpty() || MaxLines <= 0)
		{
			return 0;
		}
		const int32 MaxChars = FMath::Max(8, FMath::FloorToInt(MaxW / ApproxCharW(Scale)));
		TArray<FString> Words;
		S.ParseIntoArray(Words, TEXT(" "), true);
		if (Words.Num() == 0)
		{
			OutLines.Add(S.Left(MaxChars));
			return 1;
		}
		FString Line;
		for (int32 I = 0; I < Words.Num(); ++I)
		{
			FString Word = Words[I];
			if (Word.Len() > MaxChars)
			{
				Word = Word.Left(MaxChars - 1) + TEXT("…");
			}
			const FString Next = Line.IsEmpty() ? Word : Line + TEXT(" ") + Word;
			if (Next.Len() > MaxChars && !Line.IsEmpty())
			{
				OutLines.Add(Line);
				Line = Word;
				if (OutLines.Num() == MaxLines)
				{
					break;
				}
			}
			else
			{
				Line = Next;
			}
		}
		if (OutLines.Num() < MaxLines && !Line.IsEmpty())
		{
			OutLines.Add(Line);
		}
		else if (OutLines.Num() == MaxLines && !Line.IsEmpty() && OutLines.Last() != Line)
		{
			FString& Last = OutLines.Last();
			if (!Last.EndsWith(TEXT("…")))
			{
				Last = Ellipsize(Last, MaxW, Scale);
			}
		}
		return OutLines.Num();
	}

	FString Ellipsize(const FString& S, float MaxW, float Scale)
	{
		const int32 MaxChars = FMath::Max(4, FMath::FloorToInt(MaxW / ApproxCharW(Scale)));
		if (S.Len() <= MaxChars)
		{
			return S;
		}
		return S.Left(MaxChars - 1) + TEXT("…");
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

	float FPaint::TextBlock(FVector2D Pos, const FString& S, FLinearColor Color, float Scale, float MaxW, int32 MaxLines) const
	{
		TArray<FString> Lines;
		WrapLines(S, MaxW, Scale, MaxLines, Lines);
		const float Step = 18.f * FMath::Max(0.75f, Scale);
		for (int32 I = 0; I < Lines.Num(); ++I)
		{
			Text(Pos + FVector2D(0.f, I * Step), Lines[I], Color, Scale, MaxW);
		}
		return Lines.Num() * Step;
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

	void FPaint::Frame(FVector2D Pos, FVector2D Size, FLinearColor Color, float Thickness) const
	{
		Fill(Pos, FVector2D(Size.X, Thickness), Color);
		Fill(Pos + FVector2D(0.f, Size.Y - Thickness), FVector2D(Size.X, Thickness), Color);
		Fill(Pos, FVector2D(Thickness, Size.Y), Color);
		Fill(Pos + FVector2D(Size.X - Thickness, 0.f), FVector2D(Thickness, Size.Y), Color);
	}

	void FPaint::Chip(FVector2D Pos, FVector2D Size, EHolypawUiIcon IconId, const FString& Label, FLinearColor Accent) const
	{
		Fill(Pos, Size, Colors().FeltInner);
		Frame(Pos, Size, Accent, 1.6f);
		const float IconSize = FMath::Min(Size.Y - 10.f, 22.f);
		Icon(Pos + FVector2D(8.f, (Size.Y - IconSize) * 0.5f), IconSize, IconId, Accent);
		const float TextX = 8.f + IconSize + 8.f;
		Text(Pos + FVector2D(TextX, (Size.Y - 16.f) * 0.5f), Ellipsize(Label, Size.X - TextX - 10.f, 0.85f), Colors().Cream, 0.85f, Size.X - TextX - 8.f);
	}

	float FPaint::MeasureChip(const FString& Label, float Height) const
	{
		const float IconSize = FMath::Min(Height - 10.f, 22.f);
		return FMath::Clamp(8.f + IconSize + 8.f + TextWidth(Label, 0.85f) + 12.f, 64.f, 320.f);
	}

	float FPaint::ChipRow(FVector2D Pos, float MaxW, const TArray<FChipSpec>& Chips, float Height) const
	{
		float X = 0.f;
		float Y = 0.f;
		for (const FChipSpec& C : Chips)
		{
			const float W = FMath::Min(MeasureChip(C.Label, Height), MaxW);
			if (X > 0.f && X + W > MaxW)
			{
				X = 0.f;
				Y += Height + 8.f;
			}
			Chip(Pos + FVector2D(X, Y), FVector2D(W, Height), C.Icon, C.Label, C.Accent);
			X += W + 8.f;
		}
		return Chips.Num() > 0 ? Y + Height : 0.f;
	}

	float FPaint::Keycap(FVector2D Pos, const FString& Key) const
	{
		const float W = KeyWidth(Key);
		Fill(Pos, FVector2D(W, 22.f), Colors().Ink);
		Fill(Pos + FVector2D(1.f, 1.f), FVector2D(W - 2.f, 20.f), Colors().Gold);
		Text(Pos + FVector2D(6.f, 2.f), Key, Colors().Ink, 0.78f, W);
		return W;
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
			Fill(Pos, FVector2D(FMath::Max(3.f, Size.X * Pct), Size.Y), FillColor);
		}
		Frame(Pos, Size, WithAlpha(Colors().Cream, 0.45f), 1.4f);
	}

	void FPaint::Meter(FVector2D Pos, FVector2D Size, float Pct, const FString& Label, FLinearColor FillColor) const
	{
		Bar(Pos, Size, Pct, FillColor);
		if (!Label.IsEmpty())
		{
			Text(Pos + FVector2D(Size.X + 8.f, -4.f), Label, Colors().Cream, 0.8f, 90.f);
		}
	}

	void FPaint::Icon(FVector2D Pos, float Size, EHolypawUiIcon Id, FLinearColor Color) const
	{
		DrawIcon(Out, Layer + 2, Geo, Pos, Size, Id, Color);
	}

	void FPaint::HeartRow(FVector2D Pos, int32 Hearts, float Size, int32 MaxShow) const
	{
		if (Hearts <= 0)
		{
			Icon(Pos, Size, EHolypawUiIcon::Heart, WithAlpha(Colors().Powder, 0.35f));
			Text(Pos + FVector2D(Size + 6.f, 0.f), TEXT("0"), Colors().Muted, 0.8f, 40.f);
			return;
		}
		const int32 Show = FMath::Clamp(Hearts, 1, MaxShow);
		for (int32 I = 0; I < Show; ++I)
		{
			Icon(Pos + FVector2D(I * (Size + 4.f), 0.f), Size, EHolypawUiIcon::Heart, HeartsHeat(Hearts));
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

	void FPaint::Caption(FVector2D Pos, EHolypawUiIcon IconId, const FString& Title, FLinearColor Accent, float MaxW) const
	{
		Icon(Pos, 28.f, IconId, Accent);
		Text(Pos + FVector2D(36.f, 2.f), Ellipsize(Title, FMath::Max(80.f, MaxW - 40.f), 1.35f), Accent, 1.35f, FMath::Max(80.f, MaxW - 36.f));
	}

	void FPaint::Footer(FVector2D PanelPos, FVector2D PanelSize, const FString& Hint) const
	{
		Text(PanelPos + FVector2D(28.f, PanelSize.Y - 42.f), Ellipsize(Hint, PanelSize.X - 56.f, 0.85f), Colors().Muted, 0.85f, PanelSize.X - 50.f);
	}

	void FPaint::Toast(FVector2D CanvasSize, float Y, const FString& Msg) const
	{
		const FPalette& P = Colors();
		const float W = FMath::Min(520.f, FMath::Max(220.f, CanvasSize.X - 48.f));
		const FVector2D Pos((CanvasSize.X - W) * 0.5f, Y);
		Fill(Pos, FVector2D(W, 36.f), P.Felt);
		Frame(Pos, FVector2D(W, 36.f), P.Gold, 1.4f);
		Icon(Pos + FVector2D(10.f, 8.f), 20.f, EHolypawUiIcon::Halo, P.Gold);
		Text(Pos + FVector2D(36.f, 8.f), Ellipsize(Msg, W - 50.f, 0.85f), P.Cream, 0.85f, W - 46.f);
	}

	void FPaint::VerbRow(FVector2D Pos, float Width, const FString& Key, EHolypawUiIcon IconId, const FString& Label) const
	{
		const float W = FMath::Max(120.f, Width);
		Fill(Pos, FVector2D(W, 44.f), Colors().Idle);
		Frame(Pos, FVector2D(W, 44.f), Colors().Gold, 1.5f);
		const float KW = Keycap(Pos + FVector2D(8.f, 11.f), Key);
		const float IconX = 12.f + KW + 8.f;
		Icon(Pos + FVector2D(IconX, 10.f), 24.f, IconId, IconTint(IconId));
		const float TextX = IconX + 30.f;
		Text(Pos + FVector2D(TextX, 12.f), Ellipsize(Label, W - TextX - 10.f, 0.88f), Colors().Cream, 0.88f, W - TextX - 8.f);
	}

	void FPaint::SlotCard(FVector2D Pos, FVector2D Size, bool bSelected, const FString& Title, const FString& Body, const FString& Key) const
	{
		Fill(Pos, Size, bSelected ? Colors().Select : Colors().Idle);
		DashRect(Pos + FVector2D(5.f, 5.f), Size - FVector2D(10.f, 10.f), bSelected ? Colors().Gold : Colors().Rose, 1.5f, 7.f);
		float Inset = 16.f;
		if (!Key.IsEmpty())
		{
			const float KW = Keycap(Pos + FVector2D(12.f, (Size.Y - 22.f) * 0.5f), Key);
			Inset = 16.f + KW + 8.f;
		}
		Icon(Pos + FVector2D(Inset, (Size.Y - 32.f) * 0.5f), 32.f, EHolypawUiIcon::Teddy, bSelected ? Colors().Gold : Colors().Rose);
		const float TextX = Inset + 40.f;
		Text(Pos + FVector2D(TextX, 12.f), Ellipsize(Title, Size.X - TextX - 16.f, 1.05f), bSelected ? Colors().Gold : Colors().Cream, 1.05f, Size.X - TextX - 12.f);
		Text(Pos + FVector2D(TextX, 40.f), Ellipsize(Body, Size.X - TextX - 16.f, 0.85f), Colors().Powder, 0.85f, Size.X - TextX - 12.f);
	}
}
