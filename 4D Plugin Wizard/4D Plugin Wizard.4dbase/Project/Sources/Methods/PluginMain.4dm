//%attributes = {}
C_POINTER:C301($1)
C_POINTER:C301($2)

$1->:=$1->+ColorizeText ("void";gKeyWordColor)
$1->:=$1->+" PluginMain( "
$1->:=$1->+ColorizeText ("PA_long32";gKeyWordColor)
$1->:=$1->+" selector, PA_PluginParameters params )"+vcr
$1->:=put ($1->+"{")
$1->:=$1->+ColorizeText (vtab+"switch";gKeyWordColor)
$1->:=put ($1->+"( selector )")
$1->:=put ($1->+vtab+"{")


If ([Projects:2]Init:6)
	$1->:=$1->+BuildCase ("kInitPlugin";"InitPlugin()")
End if 

If ([Projects:2]DeInit:7)
	$1->:=$1->+BuildCase ("kDeinitPlugin";"DeinitPlugin()")
End if 

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
		[orderedCommands:9]fText:2:=[Commands:4]Name:3
		[orderedCommands:9]fTheme:3:=[Themes:3]Name:3
		NEXT RECORD:C51([Commands:4])
		SAVE RECORD:C53([orderedCommands:9])
	End for 
	NEXT RECORD:C51([Themes:3])
End for 
ALL RECORDS:C47([orderedCommands:9])
ORDER BY:C49([orderedCommands:9];[orderedCommands:9]fID:1;>)
FIRST RECORD:C50([orderedCommands:9])
$Theme:=""
$commandID:=1
While (End selection:C36([orderedCommands:9])=False:C215)
	If ($Theme#[orderedCommands:9]fTheme:3)
		$Theme:=[orderedCommands:9]fTheme:3
		
		$1->:=put ($1->+ColorizeText ("// --- "+$Theme;gCommentColor))
	End if 
	If ([Projects:2]DefineCommand:20)
		$1->:=$1->+BuildCase ("eCMD_"+NameFilter ([orderedCommands:9]fText:2;True:C214;False:C215;False:C215);NameFilter ([orderedCommands:9]fText:2;True:C214;False:C215;False:C215)+"( params )")
	Else 
		$1->:=$1->+BuildCase (String:C10($commandID);NameFilter ([orderedCommands:9]fText:2;True:C214;False:C215;False:C215)+"( params )")
	End if 
	$commandID:=$commandID+1
	NEXT RECORD:C51([orderedCommands:9])
End while 
DELETE SELECTION:C66([orderedCommands:9])

QUERY:C277([ExternalAreas:5];[ExternalAreas:5]Project:2=[Projects:2]ID:1)

If (Records in selection:C76([ExternalAreas:5])>0)
	$1->:=put ($1->+ColorizeText ("// --- External areas";gCommentColor))
End if 

QUERY:C277([ExternalAreas:5];[ExternalAreas:5]Project:2=[Projects:2]ID:1)
FIRST RECORD:C50([ExternalAreas:5])
For ($i;1;Records in selection:C76([ExternalAreas:5]))
	$1->:=$1->+BuildCase (String:C10($commandID);NameFilter ([ExternalAreas:5]Name:3;True:C214;False:C215;False:C215)+"( params )")
	$commandID:=$commandID+1
	NEXT RECORD:C51([ExternalAreas:5])
End for 
$1->:=put ($1->+vtab+"}")
$1->:=put ($1->+"}")
$1->:=put ($1->)

If ([Projects:2]Init:6)
	$1->:=$1->+ColorizeText ("void";gKeyWordColor)
	$1->:=put ($1->+" InitPlugin()"+vcr+"{")
	$1->:=put ($1->+vtab+ColorizeText ("// write initialisation code here...";gCommentColor))
	$1->:=put ($1->+"}"+vcr)
	
	$2->:=$2->+ColorizeText ("void";gKeyWordColor)
	$2->:=$2->+" InitPlugin();"+vcr
	
End if 

If ([Projects:2]DeInit:7)
	$1->:=$1->+ColorizeText ("void";gKeyWordColor)
	$1->:=put ($1->+" DeinitPlugin()"+vcr+"{")
	$1->:=$1->+ColorizeText (vtab+"// write deinitialisation code here...";gCommentColor)
	$1->:=put ($1->+vcr+"}"+vcr)
	
	$2->:=$2->+ColorizeText ("void";gKeyWordColor)
	$2->:=$2->+" DeinitPlugin();"+vcr
	
End if 

