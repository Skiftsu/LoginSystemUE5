using UnrealBuildTool;
using System;
using System.IO;
using System.Collections.Generic;

public class libprotobuf : ModuleRules
{
	public libprotobuf(ReadOnlyTargetRules Target) : base(Target)
	{
		Type = ModuleType.External;

		string protobufPath = ModuleDirectory;

		bool IsSupported = false;
		if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			IsSupported = true;
			PublicAdditionalLibraries.Add(Path.Combine(protobufPath, "lib", "libprotobuf.lib"));
		}

		if (IsSupported) PublicSystemIncludePaths.Add(Path.Combine(protobufPath, "include"));
	}
}
