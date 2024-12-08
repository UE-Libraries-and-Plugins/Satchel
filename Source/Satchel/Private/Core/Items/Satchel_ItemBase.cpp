#include "Core/Items/Satchel_ItemBase.h"

#include "Core/Items/Fragments/Satchel_ItemFragment_Base.h"
#include "Misc/DataValidation.h"


void USatchel_ItemBase::GetGiverContextEntries_Implementation(TSet<UContext_ActionEntry*>& OutContextEntries) {
	OutContextEntries.Append(ContextEntries);
}

USatchel_ItemFragment_Base* USatchel_ItemBase::FindFragment(
	TSubclassOf<USatchel_ItemFragment_Base> FragmentClass) const {

	if(FragmentClass == nullptr) return nullptr;

	for (const auto Fragment : Fragments) {
		if (Fragment && Fragment.IsA(FragmentClass)) {
			return Fragment;
		}
	}

	return nullptr;
}

#if WITH_EDITOR
EDataValidationResult USatchel_ItemBase::IsDataValid(FDataValidationContext& Context) const {

	if (!ItemId.IsValid()) {
		Context.AddError(FText::FromString(TEXT("Item ID has not been set, rendering this item invalid!")));
	}

	const EDataValidationResult BaseResult = Super::IsDataValid(Context);
	return Context.GetNumErrors() > 0 ? EDataValidationResult::Invalid : BaseResult;
}
#endif
