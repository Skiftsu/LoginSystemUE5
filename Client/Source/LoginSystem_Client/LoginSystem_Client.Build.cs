// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class LoginSystem_Client : ModuleRules
{
	public LoginSystem_Client(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"Networking",
			"Sockets",
			"OpenSSL"
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"UMG", 
			"Slate", 
			"SlateCore",
			"libprotobuf"
		});
	}
}
