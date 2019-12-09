C_TEXT:C284($resourcePath)
C_LONGINT:C283($length)
$resourcePath:=""
$Options:=Alias selection:K24:10+Package open:K24:8+Use sheet window:K24:11
$resourcePath:=Select document:C905("";"rsr;rsrc;json;xlf;xliff";"Select in a plugin";$Options)
$length:=Length:C16($resourcePath)
If ($length>0)
	$resourcePath:=Document
	$length:=Length:C16($resourcePath)
	Case of 
		: (Substring:C12($resourcePath;$length-3;4)="rsrc")
			RSR_Import ($resourcePath)
		: (Substring:C12($resourcePath;$length-2;3)="rsr")
			RSR_Import ($resourcePath)
		: (Substring:C12($resourcePath;$length-3;4)="json")
			JSON_Import ($resourcePath)
		: (Substring:C12($resourcePath;$length-2;3)="xlf")
			XLIFF_Import ($resourcePath)
		: (Substring:C12($resourcePath;$length-4;5)="xliff")
			XLIFF_Import ($resourcePath)
	End case 
End if 
QUERY:C277([Themes:3];[Themes:3]Project:2=[Projects:2]ID:1)
CLEAR LIST:C377(hlist;*)
buildHList 
BuildSource 
