//%attributes = {}
C_TEXT:C284($content)
If ([Projects:2]DefineCommand:20)
	
	ALL RECORDS:C47([orderedCommands:9])
	DELETE SELECTION:C66([orderedCommands:9])
	QUERY:C277([Themes:3];[Themes:3]Project:2=[Projects:2]ID:1)
	FIRST RECORD:C50([Themes:3])
	$nbres1:=Records in selection:C76([Themes:3])
	For ($i;1;$nbres1)
		QUERY:C277([Commands:4];[Commands:4]Theme:2=[Themes:3]ID:1)
		FIRST RECORD:C50([Commands:4])
		$nbres2:=Records in selection:C76([Commands:4])
		For ($j;1;$nbres2)
			CREATE RECORD:C68([orderedCommands:9])
			[orderedCommands:9]fID:1:=[Commands:4]ID:1
			[orderedCommands:9]fText:2:=[Commands:4]Name:3
			SAVE RECORD:C53([orderedCommands:9])
			NEXT RECORD:C51([Commands:4])
		End for 
		NEXT RECORD:C51([Themes:3])
	End for 
	$content:=kw ("enum")
	$content:=put ($content+"{")
	
	ALL RECORDS:C47([orderedCommands:9])
	ORDER BY:C49([orderedCommands:9];[orderedCommands:9]fID:1;>)
	FIRST RECORD:C50([orderedCommands:9])
	$nbres1:=Records in selection:C76([orderedCommands:9])
	For ($i;1;$nbres1)
		$content:=$content+(vtab+"eCMD_"+NameFilter ([orderedCommands:9]fText:2;True:C214;False:C215))
		If ($i=1)
			$content:=$content+" = 1"
		End if 
		If ($i#$nbres1)
			$content:=$content+","
		End if 
		$content:=put ($content)
		NEXT RECORD:C51([orderedCommands:9])
	End for 
	$content:=put ($content+"};")
End if 
$0:=$content
