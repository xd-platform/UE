#pragma once


struct XULoginTypeModel
{

	XUType::LoginType Type;

	FString TypeName;

	int TypeValue;

	XULoginTypeModel() = default;


	static FString GetName(int Type) {
		FString Name = "";
		auto EnumType = XUType::LoginType(Type);
		switch (EnumType) {
		case XUType::Default:
			Name = "Default";
			break;
		case XUType::Guest:
			Name = "Guest";
			break;
		case XUType::Apple:
			Name = "Apple";
			break;
		case XUType::Google:
			Name = "Google";
			break;
		case XUType::TapTap:
			Name = "TapTap";
			break;
		case XUType::Steam:
			Name = "Steam";
			break;
		}
		return Name;
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
