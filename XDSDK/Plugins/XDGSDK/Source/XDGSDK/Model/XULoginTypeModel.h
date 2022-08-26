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
		return "";
	}

	static TArray<XULoginTypeModel> GetSDKSupportTypes() {
		TArray<XULoginTypeModel> List;
		List.Add(XULoginTypeModel(XUType::TapTap));
		List.Add(XULoginTypeModel(XUType::Google));
		return List;
	}

	XULoginTypeModel(XUType::LoginType Type) : Type(Type), TypeValue((int)Type)
	{
		TypeName = GetName((int)Type);
	}
	
};
