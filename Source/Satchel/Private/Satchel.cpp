// Copyright Epic Games, Inc. All Rights Reserved.

#include "Satchel.h"
#include "Misc/MessageDialog.h"
#include "Modules/ModuleManager.h"
#include "Misc/Paths.h"
#include "HAL/PlatformProcess.h"

#define LOCTEXT_NAMESPACE "FSatchelModule"


void FSatchelModule::StartupModule()
{
}

void FSatchelModule::ShutdownModule()
{
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FSatchelModule, Satchel)
