#pragma once
#include "NewGRPCClientPluginDescriptorData.h"

class SGRPCClientPluginGenerator : public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SGRPCClientPluginGenerator) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, const TSharedPtr<SDockTab> InOwnerTab);
	
private:
	FReply OnGenerateButtonClicked() const;
	bool IsGenerateButtonEnabled() const;
	
	TWeakPtr<SDockTab> OwnerTab;
	TWeakObjectPtr<UNewGRPCClientPluginDescriptorData> DescriptorData;
};
