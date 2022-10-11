// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;
using System;

public class XDGCommon : ModuleRules
{
	public XDGCommon(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		PrivateIncludePaths.Add(Path.GetFullPath(Path.Combine(ModuleDirectory, "Private")));
        PublicIncludePaths.Add(Path.GetFullPath(Path.Combine(ModuleDirectory, "Public")));
		
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
				"Core",
				"Engine",
				"RenderCore",
				"RHI",
				"Core",
				"CoreUObject",
				"Slate",
				"SlateCore",
				"UMG",
				"TapDB"
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"Engine",
				"RenderCore",
				"RHI",
				"Core",
				"CoreUObject",
				"Slate",
				"SlateCore",
				"UMG",
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

		if (Target.Platform == UnrealTargetPlatform.Android){
            PrivateDependencyModuleNames.AddRange(
                new string[]
                {
                        "Launch"
                }
            );
            AdditionalPropertiesForReceipt.Add(
                "AndroidPlugin",
                Path.Combine(ModuleDirectory, "XDGCommon_Android_UPL.xml")
            );
        }


		 if (Target.Platform == UnrealTargetPlatform.IOS) {

			   PublicAdditionalFrameworks.Add(
                    new Framework(
						"AdjustSdk",
						"../ThirdParty/AdjustSdk.embeddedframework.zip"
                    )
                );
				PublicAdditionalFrameworks.Add(
                    new Framework(
						"AppsFlyerLib",
						"../ThirdParty/AppsFlyerLib.embeddedframework.zip"
                    )
                );

				PublicAdditionalFrameworks.Add(
                    new Framework(
						"FBAEMKit",
						"../ThirdParty/FBAEMKit.embeddedframework.zip"
                    )
                );

				PublicAdditionalFrameworks.Add(
                    new Framework(
						"FBSDKCoreKit_Basics",
						"../ThirdParty/FBSDKCoreKit_Basics.embeddedframework.zip"
                    )
                );

				PublicAdditionalFrameworks.Add(
                    new Framework(
						"FBSDKCoreKit",
						"../ThirdParty/FBSDKCoreKit.embeddedframework.zip"
                    )
                );
				PublicAdditionalFrameworks.Add(
                    new Framework(
						"FBSDKLoginKit",
						"../ThirdParty/FBSDKLoginKit.embeddedframework.zip"
                    )
                );
				PublicAdditionalFrameworks.Add(
                    new Framework(
						"FBSDKShareKit",
						"../ThirdParty/FBSDKShareKit.embeddedframework.zip"
                    )
                );
				PublicAdditionalFrameworks.Add(
                    new Framework(
						"LineSDK",
						"../ThirdParty/LineSDK.embeddedframework.zip"
                    )
                );
				PublicAdditionalFrameworks.Add(
                    new Framework(
						"TDSGTwitterLoginKit",
						"../ThirdParty/TDSGTwitterLoginKit.embeddedframework.zip"
                    )
                );

				//17star
				PublicAdditionalFrameworks.Add(
                    new Framework(
						"GoogleSignIn",
						"../ThirdParty/GoogleSignIn.embeddedframework.zip"
                    )
                );
				PublicAdditionalFrameworks.Add(
                    new Framework(
						"AppAuth",
						"../ThirdParty/AppAuth.embeddedframework.zip"
                    )
                );
				PublicAdditionalFrameworks.Add(
                    new Framework(
						"GTMAppAuth",
						"../ThirdParty/GTMAppAuth.embeddedframework.zip"
                    )
                );
				PublicAdditionalFrameworks.Add(
                    new Framework(
						"GTMSessionFetcher",
						"../ThirdParty/GTMSessionFetcher.embeddedframework.zip"
                    )
                );
				PublicAdditionalFrameworks.Add(
                    new Framework(
						"FirebaseAnalytics",
						"../ThirdParty/FirebaseAnalytics.embeddedframework.zip"
                    )
                );
				PublicAdditionalFrameworks.Add(
                    new Framework(
						"FirebaseAuth",
						"../ThirdParty/FirebaseAuth.embeddedframework.zip"
                    )
                );
				PublicAdditionalFrameworks.Add(
                    new Framework(
						"FirebaseCore",
						"../ThirdParty/FirebaseCore.embeddedframework.zip"
                    )
                );
				PublicAdditionalFrameworks.Add(
                    new Framework(
						"FirebaseCoreDiagnostics",
						"../ThirdParty/FirebaseCoreDiagnostics.embeddedframework.zip"
                    )
                );
				PublicAdditionalFrameworks.Add(
                    new Framework(
						"FirebaseCrashlytics",
						"../ThirdParty/FirebaseCrashlytics.embeddedframework.zip"
                    )
                );
				PublicAdditionalFrameworks.Add(
                    new Framework(
						"FirebaseInstallations",
						"../ThirdParty/FirebaseInstallations.embeddedframework.zip"
                    )
                );
				PublicAdditionalFrameworks.Add(
                    new Framework(
						"FirebaseInstanceID",
						"../ThirdParty/FirebaseInstanceID.embeddedframework.zip"
                    )
                );
				PublicAdditionalFrameworks.Add(
                    new Framework(
						"FirebaseMessaging",
						"../ThirdParty/FirebaseMessaging.embeddedframework.zip"
                    )
                );
				PublicAdditionalFrameworks.Add(
                    new Framework(
						"GoogleAppMeasurement",
						"../ThirdParty/GoogleAppMeasurement.embeddedframework.zip"
                    )
                );
				PublicAdditionalFrameworks.Add(
                    new Framework(
						"GoogleDataTransport",
						"../ThirdParty/GoogleDataTransport.embeddedframework.zip"
                    )
                );
				PublicAdditionalFrameworks.Add(
                    new Framework(
						"GoogleUtilities",
						"../ThirdParty/GoogleUtilities.embeddedframework.zip"
                    )
                );
				PublicAdditionalFrameworks.Add(
                    new Framework(
						"nanopb",
						"../ThirdParty/nanopb.embeddedframework.zip"
                    )
                );
				PublicAdditionalFrameworks.Add(
                    new Framework(
						"PromisesObjC",
						"../ThirdParty/PromisesObjC.embeddedframework.zip"
                    )
                );

				PublicFrameworks.AddRange(
					new string[]{
						"Accelerate",
						"SystemConfiguration",
						"WebKit",
						"SystemConfiguration",
						"CoreTelephony",
						"MobileCoreServices",
						"Security",
						"SafariServices",
						"AuthenticationServices"
					}
				);

				PublicSystemLibraryPaths.Add("/usr/lib/swift");
				PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "../ThirdParty/libswiftCompatibility51.a"));
                PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "../ThirdParty/libswiftCompatibility50.a"));
                PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "../ThirdParty/libswiftCompatibilityDynamicReplacements.a")); 

				//bundle
				// AdditionalBundleResources.Add(new BundleResource(Path.Combine(ModuleDirectory, "./iOS/GoogleSignIn.bundle")));
				// AdditionalBundleResources.Add(new BundleResource(Path.Combine(ModuleDirectory, "./iOS/LineSDKResource.bundle")));
				// AdditionalBundleResources.Add(new BundleResource(Path.Combine(ModuleDirectory, "./iOS/XDResources.bundle")));
				// AdditionalBundleResources.Add(new BundleResource(Path.Combine(ModuleDirectory, "./iOS/GoogleService-Info.plist")));
				// AdditionalBundleResources.Add(new BundleResource(Path.Combine(ModuleDirectory, "./Configs/XDConfig.json")));

                // Add framework
                PublicAdditionalFrameworks.Add(
                    new Framework(
						"XDCommonSDK",
						"../ThirdParty/XDCommonSDK.embeddedframework.zip"
                    )
                );

				PublicAdditionalFrameworks.Add(
                    new Framework(
						"XDThirdLoginSDK",
						"../ThirdParty/XDThirdLoginSDK.embeddedframework.zip"
                    )
                );

				PublicAdditionalFrameworks.Add(
                    new Framework(
						"XDThirdTrackSDK",
						"../ThirdParty/XDThirdTrackSDK.embeddedframework.zip"
                    )
                );
            
                AdditionalPropertiesForReceipt.Add(
                    "IOSPlugin", 
                    Path.Combine(ModuleDirectory, "XDGCommon_iOS_UPL.xml")
                );
            }


	}
}
