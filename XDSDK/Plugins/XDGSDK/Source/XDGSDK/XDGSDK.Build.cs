// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;
using System;
using Tools.DotNETCommon;

public class XDGSDK : ModuleRules
{
	public XDGSDK(ReadOnlyTargetRules Target) : base(Target)
	{

		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		PrivateIncludePaths.Add(Path.GetFullPath(Path.Combine(ModuleDirectory, "Private")));
		PrivateIncludePaths.Add(Path.GetFullPath(Path.Combine(ModuleDirectory, "Model")));
		PrivateIncludePaths.Add(Path.GetFullPath(Path.Combine(ModuleDirectory, "Server")));

		PublicIncludePaths.Add(Path.GetFullPath(Path.Combine(ModuleDirectory, "Public")));
		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
			}
			);
		string ConfigFile = Path.Combine(PluginDirectory, "Content/XDGAssets/**");
		//RuntimeDependencies.Add(new RuntimeDependency(ConfigFile));
		RuntimeDependencies.Add(ConfigFile);
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"Http",
				"Json",
				"UMG",
				// "OpenSSL",
				"JsonUtilities",
				"ApplicationCore",
				"TapBootstrap",
				"TapCommon",
				"TapLogin",
				"HTTPServer",
				"WebBrowser",
				"TapDB"
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
	
	public string GetUproject(string dir) 
	{ 
		try 
		{ 
			string[] files = Directory.GetFiles(dir); // 得到文件 
			foreach (string file in files) // 循环文件 
			{
			    Console.WriteLine(file);
				if (file.EndsWith(".uproject"))
				{
					return file;
				} 
			} 
		} 
		catch
		{
			return "";
		} 
		return "";
	} 
}
