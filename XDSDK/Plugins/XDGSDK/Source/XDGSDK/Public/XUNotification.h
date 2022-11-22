#pragma once

class XDGSDK_API XUNotification {
public:
	// code 为 1为成功，Content为Ticket，code值 其余为Error，Content为Error Reason
	DECLARE_DELEGATE_TwoParams(FSteamTicketDelegate, int Code, const FString& Content);

	static FSteamTicketDelegate SteamTicketDelegate;
};
