#include "AAUUser.h"

TSharedPtr<FAAUUser> FAAUUser::CurrentUser = nullptr;

TSharedPtr<FAAUUser>& FAAUUser::GetCurrentUser() {
	return CurrentUser;
}
