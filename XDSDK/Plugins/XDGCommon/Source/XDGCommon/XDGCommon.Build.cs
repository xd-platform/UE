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

		 	// Adjust
		 	PublicAdditionalFrameworks.Add(
                    new Framework(
						"AdjustSdk",
						"../ThirdParty/iOS/Frameworks/AdjustSdk.zip"
                    )
                );
			// AppsFlyer
			PublicAdditionalFrameworks.Add(
                    new Framework(
						"AppsFlyerLib",
						"../ThirdParty/iOS/Frameworks/AppsFlyerLib.zip"
                    )
                );
			// Facebook
			PublicAdditionalFrameworks.Add(
                    new Framework(
						"FBAEMKit",
						"../ThirdParty/iOS/Frameworks/FBAEMKit.zip"
                    )
                );

			PublicAdditionalFrameworks.Add(
                    new Framework(
						"FBSDKCoreKit_Basics",
						"../ThirdParty/iOS/Frameworks/FBSDKCoreKit_Basics.zip"
                    )
                );
			PublicAdditionalFrameworks.Add(
                    new Framework(
						"FBSDKCoreKit",
						"../ThirdParty/iOS/Frameworks/FBSDKCoreKit.zip"
                    )
                );
			PublicAdditionalFrameworks.Add(
                    new Framework(
						"FBSDKLoginKit",
						"../ThirdParty/iOS/Frameworks/FBSDKLoginKit.zip"
                    )
                );
			PublicAdditionalFrameworks.Add(
                    new Framework(
						"FBSDKShareKit",
						"../ThirdParty/iOS/Frameworks/FBSDKShareKit.zip"
                    )
                );


			// Google
			PublicAdditionalFrameworks.Add(
                    new Framework(
						"FirebaseAnalytics",
						"../ThirdParty/iOS/Frameworks/FirebaseAnalytics.zip"
                    )
                );
			PublicAdditionalFrameworks.Add(
                    new Framework(
						"FirebaseCore",
						"../ThirdParty/iOS/Frameworks/FirebaseCore.zip"
                    )
                );
			PublicAdditionalFrameworks.Add(
                    new Framework(
						"FirebaseCoreDiagnostics",
						"../ThirdParty/iOS/Frameworks/FirebaseCoreDiagnostics.zip"
                    )
                );
			PublicAdditionalFrameworks.Add(
                    new Framework(
						"FirebaseInstallations",
						"../ThirdParty/iOS/Frameworks/FirebaseInstallations.zip"
                    )
                );
			PublicAdditionalFrameworks.Add(
                    new Framework(
						"GoogleAppMeasurement",
						"../ThirdParty/iOS/Frameworks/GoogleAppMeasurement.zip"
                    )
                );
			PublicAdditionalFrameworks.Add(
                    new Framework(
						"GoogleDataTransport",
						"../ThirdParty/iOS/Frameworks/GoogleDataTransport.zip"
                    )
                );
			PublicAdditionalFrameworks.Add(
                    new Framework(
						"GoogleUtilities",
						"../ThirdParty/iOS/Frameworks/GoogleUtilities.zip"
                    )
                );
			PublicAdditionalFrameworks.Add(
                    new Framework(
						"nanopb",
						"../ThirdParty/iOS/Frameworks/nanopb.zip"
                    )
                );
			PublicAdditionalFrameworks.Add(
                    new Framework(
						"PromisesObjC",
						"../ThirdParty/iOS/Frameworks/PromisesObjC.zip"
                    )
                );
			PublicAdditionalFrameworks.Add(
                    new Framework(
						"FirebaseAuth",
						"../ThirdParty/iOS/Frameworks/FirebaseAuth.zip"
                    )
                );
			PublicAdditionalFrameworks.Add(
                    new Framework(
						"FirebaseCrashlytics",
						"../ThirdParty/iOS/Frameworks/FirebaseCrashlytics.zip"
                    )
                );
			PublicAdditionalFrameworks.Add(
                    new Framework(
						"FirebaseInstanceID",
						"../ThirdParty/iOS/Frameworks/FirebaseInstanceID.zip"
                    )
                );
			PublicAdditionalFrameworks.Add(
                    new Framework(
						"FirebaseMessaging",
						"../ThirdParty/iOS/Frameworks/FirebaseMessaging.zip"
                    )
                );
			PublicAdditionalFrameworks.Add(
                    new Framework(
						"AppAuth",
						"../ThirdParty/iOS/Frameworks/AppAuth.zip"
                    )
                );
			PublicAdditionalFrameworks.Add(
                    new Framework(
						"GTMAppAuth",
						"../ThirdParty/iOS/Frameworks/GTMAppAuth.zip"
                    )
                );
			PublicAdditionalFrameworks.Add(
                    new Framework(
						"GTMSessionFetcher",
						"../ThirdParty/iOS/Frameworks/GTMSessionFetcher.zip"
                    )
                );
			PublicAdditionalFrameworks.Add(
                    new Framework(
						"GoogleSignIn",
						"../ThirdParty/iOS/Frameworks/GoogleSignIn.zip"
                    )
                );

			// Line
			PublicAdditionalFrameworks.Add(
                    new Framework(
						"LineSDK",
						"../ThirdParty/iOS/Frameworks/LineSDK.zip"
                    )
                );

			// Twitter
			PublicAdditionalFrameworks.Add(
                    new Framework(
						"TDSGTwitterLoginKit",
						"../ThirdParty/iOS/Frameworks/TDSGTwitterLoginKit.zip"
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
				PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "../ThirdParty/iOS/Libs/libswiftCompatibility51.a"));
                PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "../ThirdParty/iOS/Libs/libswiftCompatibility50.a"));
                PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "../ThirdParty/iOS/Libs/libswiftCompatibilityDynamicReplacements.a")); 

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
						"../ThirdParty/iOS/Frameworks/XDCommonSDK.zip"
                    )
                );

				PublicAdditionalFrameworks.Add(
                    new Framework(
						"XDThirdLoginSDK",
						"../ThirdParty/iOS/Frameworks/XDThirdLoginSDK.zip"
                    )
                );

				PublicAdditionalFrameworks.Add(
                    new Framework(
						"XDThirdTrackSDK",
						"../ThirdParty/iOS/Frameworks/XDThirdTrackSDK.zip"
                    )
                );
            
                AdditionalPropertiesForReceipt.Add(
                    "IOSPlugin", 
                    Path.Combine(ModuleDirectory, "XDGCommon_iOS_UPL.xml")
                );
            }


	}
}
