#include "WeaponSmith.h"

Instance* TESObjectREFR::GetWeaponBase() {

	if (InstanceLinker[this->baseForm->typeID].find(this->baseForm->refID) != InstanceLinker[this->baseForm->typeID].end()) {
		return InstanceLinker[this->baseForm->typeID][this->baseForm->refID];
	}
	return nullptr;
}

UInt8 TESObjectREFR::GetWeaponModFlags()
{

	InventoryRef* invRef = InventoryRef::InventoryRefGetForID(this->refID);
	ExtraDataList* xData = invRef ? invRef->data.xData : &this->extraDataList;
	if (xData) {
		ExtraWeaponModFlags* xWeaponModFlags = static_cast<ExtraWeaponModFlags*>(xData->GetByType(kExtraData_WeaponModFlags));
		if (xWeaponModFlags) {
			return xWeaponModFlags->flags;
		}
	}

	return 0;

}


void TESObjectREFR::SetWeaponModFlags(UInt8 flags)
{

	InventoryRef* invRef = InventoryRef::InventoryRefGetForID(this->refID);

	ExtraDataList* xData = invRef ? invRef->data.xData : &this->extraDataList;
	if (xData) {

		ExtraWeaponModFlags* xWeaponModFlags = static_cast<ExtraWeaponModFlags*>(xData->GetByType(kExtraData_WeaponModFlags));

		// Modify existing flags
		if (xWeaponModFlags) {
			if (flags) {
				xWeaponModFlags->flags = (UInt8)flags;
			}
			else {
				xData->Remove(xWeaponModFlags, true);
			}

		} // Create new extra data
		else if (flags) {

			if (invRef) {
				if (invRef->GetCount() > 1) {
					xData = invRef->SplitFromStack(1);
				}
				xData->Add(ExtraWeaponModFlags::Create(flags));

			}
			else {
				xData->Add(ExtraWeaponModFlags::Create(flags));
			}

		}

	}

}
