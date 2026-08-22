#include "HolypawCatalog.h"

namespace HolypawCatalog
{
	TArray<FFluffyTypeDef> MakeFluffyTypes()
	{
		TArray<FFluffyTypeDef> Out;
		auto Add = [&](EFluffyId Id, const TCHAR* Name, FLinearColor Color, int32 Atk, const TCHAR* Rarity)
		{
			FFluffyTypeDef D;
			D.Id = Id;
			D.DisplayName = FText::FromString(Name);
			D.Color = Color;
			D.Attack = Atk;
			D.Rarity = Rarity;
			Out.Add(D);
		};
		Add(EFluffyId::Bunny, TEXT("Cotton Bunny"), FLinearColor(0.99f, 0.85f, 0.93f), 5, TEXT("common"));
		Add(EFluffyId::Pup, TEXT("Plush Pup"), FLinearColor(0.99f, 0.78f, 0.55f), 7, TEXT("common"));
		Add(EFluffyId::Duck, TEXT("Bath Duck"), FLinearColor(0.99f, 0.92f, 0.45f), 4, TEXT("common"));
		Add(EFluffyId::Panda, TEXT("Ink Panda"), FLinearColor(0.88f, 0.89f, 0.91f), 9, TEXT("rare"));
		Add(EFluffyId::Fox, TEXT("Ember Fox"), FLinearColor(0.98f, 0.68f, 0.38f), 10, TEXT("rare"));
		Add(EFluffyId::Dragon, TEXT("Velvet Dragon"), FLinearColor(0.72f, 0.65f, 0.95f), 14, TEXT("legend"));
		return Out;
	}
}
