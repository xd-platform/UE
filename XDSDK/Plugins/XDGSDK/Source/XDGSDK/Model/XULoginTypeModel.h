#pragma once


struct XULoginTypeModel
{

	XUType::LoginType Type;

	FString TypeName;

	int TypeValue;

	XULoginTypeModel() = default;


	static FString GetName(int Type) {
		if (Type == (int)XUType::TapTap) {
			return "TapTap";
		}
		else if (Type == (int)XUType::Guest) {
			return "Guest";
		}
		else if (Type == (int)XUType::Google) {
			return "Google";
		}
		else if (Type == (int)XUType::Apple) {
			return "Apple";
		}
		else if (Type == (int)XUType::Steam) {
			return "Steam";
		}
		return "";
	}

	static TArray<XULoginTypeModel> GetSDKSupportTypes() {
		TArray<XULoginTypeModel> List;
		List.Add(XULoginTypeModel(XUType::TapTap));
		List.Add(XULoginTypeModel(XUType::Google));
		List.Add(XULoginTypeModel(XUType::Apple));
		List.Add(XULoginTypeModel(XUType::Steam));
		return List;
	}

	XULoginTypeModel(XUType::LoginType Type) : Type(Type), TypeValue((int)Type)
	{
		TypeName = GetName((int)Type);
	}
	
};
