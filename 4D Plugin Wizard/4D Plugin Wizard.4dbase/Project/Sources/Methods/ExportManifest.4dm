//%attributes = {}
C_TEXT:C284($1)  // $1 : foldername

C_TEXT:C284($XLIFF)
C_TEXT:C284($JSONstr)
C_OBJECT:C1216($JSON)
C_BOOLEAN:C305($confirmed)

C_TEXT:C284($jsonName)
C_TIME:C306($xliffFile)
C_TIME:C306($jsonFile)
C_TEXT:C284($xliffName)
C_TEXT:C284($basePath)
C_TEXT:C284($pluginPath)
C_TEXT:C284($contentsPath)
C_TEXT:C284($resPath)
C_TEXT:C284($path)
C_TEXT:C284($msg)
C_TEXT:C284($baseName)
$baseName:="sample.4dbase"
$resourcesFolder:=$1+"Resources"+Folder separator:K24:12
$msg:=""

$confirmed:=True:C214

$basePath:=$1+$baseName+Folder separator:K24:12
$pluginPath:=$basePath+"Plugins"+Folder separator:K24:12+"4D Plugin.bundle"+Folder separator:K24:12
$contentsPath:=$pluginPath+"Contents"+Folder separator:K24:12
$resPath:=$contentsPath+"Resources"+Folder separator:K24:12

$jsonName:="manifest.json"
$xliffName:="constants.xlf"

If ([Projects:2]exportResources:13)
	
	If (Test path name:C476($contentsPath+$jsonName)=1)
		$msg:=Char:C90(34)+$jsonName+Char:C90(34)
	End if 
	
	If (Test path name:C476($resPath+$xliffName)=1)
		If (Length:C16($msg)>0)
			$msg:=$msg+" and "
		End if 
		$msg:=$msg+Char:C90(34)+$xliffName+Char:C90(34)
	End if 
	
	
End if 

If (Test path name:C476($basePath)<0)
	CREATE FOLDER:C475($basePath)
End if 

If ([Projects:2]exportSample:24)
	ExportFile ($1;$baseName+Folder separator:K24:12+"sample.4db")
	ExportFile ($1;$baseName+Folder separator:K24:12+"sample.4dd")
End if 

If ([Projects:2]exportResources:13)
	$path:=$basePath+"Plugins"+Folder separator:K24:12
	If (Test path name:C476($path)<0)
		CREATE FOLDER:C475($path)
	End if 
	
	If (Test path name:C476($pluginPath)<0)
		CREATE FOLDER:C475($pluginPath)
	End if 
	
	If (Test path name:C476($contentsPath)<0)
		CREATE FOLDER:C475($contentsPath)
	End if 
	
	If (Test path name:C476($resPath)<0)
		CREATE FOLDER:C475($resPath)
	End if 
	
	If (Test path name:C476($resourcesFolder)<0)
		CREATE FOLDER:C475($resourcesFolder)
	End if 
	
	If ($confirmed)
		$XLIFF:=XLIFF_Export 
		DOM EXPORT TO FILE:C862($XLIFF;$resourcesFolder+$xliffName)
		DOM CLOSE XML:C722($XLIFF)
	End if 
	
	If ($confirmed)
		$JSON:=JSON_Export 
		$JSONstr:=JSON Stringify:C1217($JSON;*)
		TEXT TO DOCUMENT:C1237($resourcesFolder+$jsonName;$JSONstr)
	End if 
	
	$confirmed:=True:C214
	If ([Projects:2]use4DDialogs:8 & (Length:C16([Projects:2]DialogPath:9)>0) & (Test path name:C476([Projects:2]DialogPath:9)=Is a folder:K24:2))
		$path:=$resPath+"Internal Components"+Folder separator:K24:12
		If (Test path name:C476($path)<0)
			CREATE FOLDER:C475($path)
		Else 
			CONFIRM:C162($path+" already exists. OK to overwrite ?")
			$confirmed:=(ok=1)
		End if 
		
		If ($confirmed)
			COPY DOCUMENT:C541([Projects:2]DialogPath:9;$path;*)
		End if 
	End if 
	
End if 
