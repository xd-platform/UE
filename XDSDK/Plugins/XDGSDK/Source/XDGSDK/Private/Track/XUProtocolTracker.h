#pragma once

class XUProtocolTracker {
public:
	
	// 唤起隐私协议
	static void AskPrivacy();
	
	// 同意隐私协议
	static void AgreePrivacy(int32 PrivacyType);

};
