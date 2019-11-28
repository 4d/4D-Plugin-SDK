_O_PLATFORM PROPERTIES:C365($platform)
C_BOOLEAN:C305(IsPathSDKValid)

onMac:=$platform=2
hasConfirmed:=False:C215
pathPlugin:=Get 4D folder:C485(Database folder:K5:14)+".."+Folder separator:K24:12+".."+Folder separator:K24:12
IsPathSDKValid:=True:C214

If ([Projects:2]exportXCodeProject:10 | [Projects:2]exportSource:12 | [Projects:2]exportResources:13 | [Projects:2]exportPluginAPI:14 | [Projects:2]exportSample:24)
	$path:=Select folder:C670("Please locate destination folder";[Projects:2]path:26)+[Projects:2]Name:2+Folder separator:K24:12
	$testPath:=Test path name:C476($path)
	If (($testPath<0) | ($testPath=1))
		CREATE FOLDER:C475($path)
	End if 
	If (ok=1)
		[Projects:2]path:26:=$path
		
		If ([Projects:2]exportXCodeProject:10)
			If (Test path name:C476($path+"4D Plugin.xcodeproj"+Folder separator:K24:12)<0)
				CREATE FOLDER:C475($path+"4D Plugin.xcodeproj"+Folder separator:K24:12)
			End if 
			ExportFile ($path;"4D Plugin.xcodeproj"+Folder separator:K24:12+"project.pbxproj")
			If (Test path name:C476($path+"English.lproj"+Folder separator:K24:12)<0)
				CREATE FOLDER:C475($path+"English.lproj"+Folder separator:K24:12)
			End if 
			ExportFile ($path;"English.lproj"+Folder separator:K24:12+"InfoPlist.strings")
			ExportFile ($path;"English.lproj"+Folder separator:K24:12+"locversion.plist")
			ExportFile ($path;"Info.plist")
			ExportFile ($path;"4D Plugin_Prefix.pch")
		End if 
		
		If ([Projects:2]exportVCproject2008:25)
			ExportFile ($path;"4D Plugin-VS.vcxproj")
			ExportFile ($path;"4D Plugin-VS.sln")
			ExportFile ($path;"4D Plugin-VS.vcxproj.filters")
		End if 
		
		If ([Projects:2]exportPluginAPI:14)
			If (Test path name:C476($path+"4D Plugin API"+Folder separator:K24:12)<0)
				CREATE FOLDER:C475($path+"4D Plugin API"+Folder separator:K24:12)
			End if 
			
			If (Test path name:C476(pathPlugin+"4D Plugin API"+Folder separator:K24:12+"4DPluginAPI.c")=Is a document:K24:1)
				IsPathSDKValid:=True:C214
			Else 
				pathPlugin:=Select folder:C670("Please select the SDK folder")
				If (Test path name:C476(pathPlugin+"4D Plugin API"+Folder separator:K24:12+"4DPluginAPI.c")#Is a document:K24:1)
					IsPathSDKValid:=False:C215
				End if 
			End if 
			If (IsPathSDKValid=False:C215)
				ALERT:C41("Path of the SDK is not valid")
			Else 
				ExportFileAPI ($path;"4D Plugin API"+Folder separator:K24:12+"4DPluginAPI.c")
				ExportFileAPI ($path;"4D Plugin API"+Folder separator:K24:12+"4DPluginAPI.def")
				ExportFileAPI ($path;"4D Plugin API"+Folder separator:K24:12+"4DPluginAPI.h")
				ExportFileAPI ($path;"4D Plugin API"+Folder separator:K24:12+"EntryPoints.h")
				ExportFileAPI ($path;"4D Plugin API"+Folder separator:K24:12+"Flags.h")
				ExportFileAPI ($path;"4D Plugin API"+Folder separator:K24:12+"PrivateTypes.h")
				ExportFileAPI ($path;"4D Plugin API"+Folder separator:K24:12+"PublicTypes.h")
			End if 
		End if 
		
		If ([Projects:2]exportSource:12)
			$contentArea1:=ST Get plain text:C1092(*;"Variable";ST Text displayed with 4D Expression values:K78:23)
			$contentArea2:=ST Get plain text:C1092(*;"Variable2";ST Text displayed with 4D Expression values:K78:23)
			ExportWriteDoc ($path;"4DPlugin";".cpp";$contentArea1)
			ExportWriteDoc ($path;"4DPlugin";".h";$contentArea2)
		End if 
		
		If ([Projects:2]exportResources:13 | [Projects:2]exportSample:24)
			ExportManifest ($path)
		End if 
		
		ALERT:C41("Files generated. You can now work with them.")
		
		SHOW ON DISK:C922($path;*)
	End if 
End if 
