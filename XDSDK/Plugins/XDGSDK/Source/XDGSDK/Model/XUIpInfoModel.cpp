#include "XUIpInfoModel.h"
#include "XUStorage.h"

void FXUIpInfoModel::SaveToLocal() {
	TUDataStorage<FXUStorage>::SaveStruct(FXUStorage::IpInfo, *this, true);
}

TSharedPtr<FXUIpInfoModel> FXUIpInfoModel::GetLocalModel() {
	return  TUDataStorage<FXUStorage>::LoadStruct<FXUIpInfoModel>(FXUStorage::IpInfo);
}
