//%attributes = {}
C_OBJECT:C1216($0;$root;$tmp)
C_TEXT:C284($comStr)
ARRAY TEXT:C222($arSTR;0)
ARRAY OBJECT:C1221($commands;0)
ARRAY TEXT:C222($themename;0)

OB SET:C1220($root;"name";[Projects:2]Name:2;"id";[Projects:2]PluginID:23)
ALL RECORDS:C47([orderedCommands:9])
DELETE SELECTION:C66([orderedCommands:9])

QUERY:C277([Themes:3];[Themes:3]Project:2=[Projects:2]ID:1)
FIRST RECORD:C50([Themes:3])
$k:=1
For ($i;1;Records in selection:C76([Themes:3]))
	QUERY:C277([Commands:4];[Commands:4]Theme:2=[Themes:3]ID:1)
	$nbcoms:=Records in selection:C76([Commands:4])
	FIRST RECORD:C50([Commands:4])
	For ($j;1;$nbcoms)
		$comStr:=MakeCommandString 
		CREATE RECORD:C68([orderedCommands:9])
		[orderedCommands:9]fID:1:=[Commands:4]ID:1
		[orderedCommands:9]fText:2:=$comStr
		[orderedCommands:9]fTheme:3:=[Themes:3]Name:3
		[orderedCommands:9]fThreadSafe:4:=[Commands:4]ThreadSafe:7
		SAVE RECORD:C53([orderedCommands:9])
		NEXT RECORD:C51([Commands:4])
	End for 
	NEXT RECORD:C51([Themes:3])
End for 
ALL RECORDS:C47([orderedCommands:9])
ORDER BY:C49([orderedCommands:9];[orderedCommands:9]fID:1;>)
FIRST RECORD:C50([orderedCommands:9])
While (End selection:C36([orderedCommands:9])=False:C215)
	OB SET:C1220($tmp;"theme";[orderedCommands:9]fTheme:3;"syntax";[orderedCommands:9]fText:2;"threadSafe";[orderedCommands:9]fThreadSafe:4)
	APPEND TO ARRAY:C911($commands;OB Copy:C1225($tmp;True:C214))
	NEXT RECORD:C51([orderedCommands:9])
End while 

QUERY:C277([ExternalAreas:5];[ExternalAreas:5]Project:2=[Projects:2]ID:1)
$nbareas:=Records in selection:C76([ExternalAreas:5])
INSERT IN ARRAY:C227($arSTR;Size of array:C274($arSTR)+1;$nbareas)

FIRST RECORD:C50([ExternalAreas:5])
For ($i;1;$nbareas)
	If ([ExternalAreas:5]VisibleInPluginsMenu:4)
		OB SET:C1220($tmp;"theme";"";"syntax";"_"+[ExternalAreas:5]Name:3)
	Else 
		OB SET:C1220($tmp;"theme";"";"syntax";"%"+[ExternalAreas:5]Name:3)
	End if 
	APPEND TO ARRAY:C911($commands;OB Copy:C1225($tmp;True:C214))
	NEXT RECORD:C51([ExternalAreas:5])
End for 

OB SET ARRAY:C1227($root;"commands";$commands)

$0:=$root
