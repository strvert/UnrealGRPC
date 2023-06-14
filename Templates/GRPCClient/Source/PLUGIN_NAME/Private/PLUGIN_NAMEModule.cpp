#include "PLUGIN_NAMEModule.h"

#define LOCTEXT_NAMESPACE "PLUGIN_NAMEModule"

void FPLUGIN_NAMEModule::StartupModule()
{
}

void FPLUGIN_NAMEModule::ShutdownModule()
{
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FPLUGIN_NAMEModule, PLUGIN_NAMEEditorMode)