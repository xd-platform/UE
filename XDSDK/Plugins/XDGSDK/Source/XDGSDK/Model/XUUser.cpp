#include "XUUser.h"
#include "XUStorage.h"

TSharedPtr<FXUUser> FXUUser::CurrentModel = nullptr;

FXUUser::FXUUser() {
	auto TokenPtr = FXUTokenModel::GetLocalModel();
	if (TokenPtr.IsValid()) {
		token = *TokenPtr.Get();
	}
}

void FXUUser::SaveToLocal()
{
	if (this != CurrentModel.Get())
	{
		CurrentModel = MakeShareable(new FXUUser(*this));
	}
	TUDataStorage<FXUStorage>::SaveStruct(FXUStorage::UserInfo, *this, true);
	
}

TSharedPtr<FXUUser>& FXUUser::GetLocalModel()
{
	if (CurrentModel == nullptr)
	{
		CurrentModel = TUDataStorage<FXUStorage>::LoadStruct<FXUUser>(FXUStorage::UserInfo);
	}
	return  CurrentModel;
}

TSharedPtr<FXUTokenModel> FXUUser::Token()
{
	return  FXUTokenModel::GetLocalModel();
}

XUType::LoginType FXUUser::GetLoginType() {
	return (XUType::LoginType)loginType;
}

void FXUUser::ClearUserData()
{
	CurrentModel = nullptr;
	TUDataStorage<FXUStorage>::Remove(FXUStorage::UserInfo);
	FXUSyncTokenModel::ClearToken();
	FXUTokenModel::ClearToken();
}

bool FXUUser::IsPushServiceEnable()
{
	auto user = FXUUser::GetLocalModel();
	if (user.IsValid())
	{
		FString key = user->userId + "_push_key";
		return TUDataStorage<FXUStorage>::LoadBool(key);
	} else
	{
		return false;
	}
}

void FXUUser::SetPushServiceEnable(bool enable)
{
	auto user = FXUUser::GetLocalModel();
	if (user.IsValid())
	{
		FString key = user->userId + "_push_key";
		TUDataStorage<FXUStorage>::SaveBool(key, enable);
	}
}




