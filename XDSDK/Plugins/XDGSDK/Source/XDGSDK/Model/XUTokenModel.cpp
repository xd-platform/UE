#include "XUTokenModel.h"
#include "XUStorage.h"


void FXUTokenModel::SaveToLocal()
{
	TUDataStorage<FXUStorage>::SaveStruct(FXUStorage::TokenInfo, *this, true);
}

TSharedPtr<FXUTokenModel> FXUTokenModel::GetLocalModel()
{
	return  TUDataStorage<FXUStorage>::LoadStruct<FXUTokenModel>(FXUStorage::TokenInfo);
}

void FXUTokenModel::ClearToken()
{
	TUDataStorage<FXUStorage>::Remove(FXUStorage::TokenInfo);
}



void FXUSyncTokenModel::SaveToLocal()
{
	TUDataStorage<FXUStorage>::SaveStruct(FXUStorage::SessionTokenKey, *this, true);
}

TSharedPtr<FXUSyncTokenModel> FXUSyncTokenModel::GetLocalModel()
{
	return  TUDataStorage<FXUStorage>::LoadStruct<FXUSyncTokenModel>(FXUStorage::SessionTokenKey);
}

void FXUSyncTokenModel::ClearToken()
{
	TUDataStorage<FXUStorage>::Remove(FXUStorage::SessionTokenKey);
}
