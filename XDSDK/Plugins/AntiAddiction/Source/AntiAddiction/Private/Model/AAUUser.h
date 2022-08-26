#pragma once
#include "AAUType.h"
#include "AAUUser.generated.h"


class AAUServer;

USTRUCT()
struct FAAUUser {
	friend AAUServer;
public:
	GENERATED_BODY()
	
	UPROPERTY()
	FString UserID;

	UPROPERTY()
	FString AccessToken;

	UPROPERTY()
	int AgeLimit = 0;

	AAUType::AgeLimit GetAgeLimit() {
		if (AgeLimit == AAUType::Child) {
			return AAUType::Child;
		} else if (AgeLimit == AAUType::Teen) {
			return AAUType::Teen;
		} else if (AgeLimit == AAUType::Young) {
			return AAUType::Young;
		} else if (AgeLimit == AAUType::Adult) {
			return AAUType::Adult;
		} else {
			return AAUType::Child;
		}
	}
	FAAUUser() = default;
	FAAUUser(const FString& _UserID, const FString& _AccessToken, AAUType::AgeLimit _AgeLimit)
		: UserID(_UserID), AccessToken(_AccessToken), AgeLimit(_AgeLimit) {}

	static TSharedPtr<FAAUUser>& GetCurrentUser();

private:
	static TSharedPtr<FAAUUser> CurrentUser;
	
};

