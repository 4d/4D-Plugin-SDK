//%attributes = {}

vcr:=Char:C90(13)
vtab:=Char:C90(9)


gCommentColor:=String:C10(_LongToRGBHTML (<>gPref_CommentColor))
gKeyWordColor:=String:C10(_LongToRGBHTML (<>gPref_KeyWordColor))
gFontName:=<>gPref_FontName

gPrefFontSize:=String:C10(<>gPref_FontSize)

C_TEXT:C284($contentArea1)
C_TEXT:C284($contentArea2)
$contentArea1:=put (ToComment ("4DPlugin.cpp"))
$contentArea2:=put (ToComment ("4DPlugin.h"))

$contentArea1:=put ($contentArea1+BuildComment ("4DPlugin.cpp"))
$contentArea2:=put ($contentArea2+BuildComment ("4DPlugin.h"))

$contentArea2:=$contentArea2+BuildEnumCmds 
$contentArea2:=$contentArea2+BuildConstants 

$contentArea1:=$contentArea1+BuildInclude ("4DPluginAPI.h";True:C214)
$contentArea1:=$contentArea1+BuildInclude ("4DPlugin.h";True:C214)
$contentArea1:=put ($contentArea1)

If ([Projects:2]useAnsi:3)
	$contentArea1:=$contentArea1+BuildInclude ("stdio.h";False:C215)
	$contentArea1:=$contentArea1+BuildInclude ("string.h";False:C215)
	$contentArea1:=put ($contentArea1)
End if 

If ([Projects:2]useWin32:4)
	$contentArea1:=$contentArea1+BuildDieseIf ("VERSIONWIN")
	$contentArea1:=$contentArea1+BuildInclude ("Windows.h";False:C215)
	$contentArea1:=$contentArea1+BuildDieseEndif 
	$contentArea1:=put ($contentArea1)
End if 

PluginMain (->$contentArea1;->$contentArea2)

ALL RECORDS:C47([orderedCommands:9])
DELETE SELECTION:C66([orderedCommands:9])
QUERY:C277([Themes:3];[Themes:3]Project:2=[Projects:2]ID:1)
FIRST RECORD:C50([Themes:3])
For ($i;1;Records in selection:C76([Themes:3]))
	QUERY:C277([Commands:4];[Commands:4]Theme:2=[Themes:3]ID:1)
	FIRST RECORD:C50([Commands:4])
	For ($j;1;Records in selection:C76([Commands:4]))
		CREATE RECORD:C68([orderedCommands:9])
		[orderedCommands:9]fID:1:=[Commands:4]ID:1
		[orderedCommands:9]fTheme:3:=[Themes:3]Name:3
		SAVE RECORD:C53([orderedCommands:9])
		NEXT RECORD:C51([Commands:4])
	End for 
	NEXT RECORD:C51([Themes:3])
End for 

ALL RECORDS:C47([orderedCommands:9])
ORDER BY:C49([orderedCommands:9];[orderedCommands:9]fID:1;>)
FIRST RECORD:C50([orderedCommands:9])
$Theme:=""
While (End selection:C36([orderedCommands:9])=False:C215)
	If ($Theme#[orderedCommands:9]fTheme:3)
		$Theme:=[orderedCommands:9]fTheme:3
		CenterComment (->$contentArea1;->$contentArea2;$Theme)
	End if 
	QUERY:C277([Commands:4];[Commands:4]ID:1=[orderedCommands:9]fID:1)
	FIRST RECORD:C50([Commands:4])
	BuildMethode (NameFilter ([Commands:4]Name:3;True:C214;False:C215;False:C215);[Commands:4]Parameters:4;->$contentArea1;->$contentArea2)
	NEXT RECORD:C51([orderedCommands:9])
End while 

QUERY:C277([ExternalAreas:5];[ExternalAreas:5]Project:2=[Projects:2]ID:1)

If (Records in selection:C76([ExternalAreas:5])>0)
	CenterComment (->$contentArea1;->$contentArea2;"External Areas")
End if 

FIRST RECORD:C50([ExternalAreas:5])
For ($i;1;Records in selection:C76([ExternalAreas:5]))
	BuildExternalArea (NameFilter ([ExternalAreas:5]Name:3;True:C214;False:C215;False:C215);->$contentArea1;->$contentArea2)
	NEXT RECORD:C51([ExternalAreas:5])
End for 


areaCode:=SetFont ($contentArea1)
areaCode2:=SetFont ($contentArea2)

REDRAW WINDOW:C456
