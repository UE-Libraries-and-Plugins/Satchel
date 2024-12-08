// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Satchel : ModuleRules
{
	public Satchel(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Context",
				"Core",
				"CoreUObject",
				"DeveloperSettings",
				"Engine",
				"GameplayAbilities",
				"GameplayTags",
				"LilHelper",
				"Projects",
				"UMG",
				"UMGCommon",
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"LilHelper",
				"UMGCommon",
				"Context"
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
