#pragma once
#include "TUMomentImpl.h"

class TUMomentPCImpl: public TUMomentImpl {
public:
	virtual void Init(const TUMomentType::Config& InitConfig) override;
	virtual void OpenWebTopic() override;

};
