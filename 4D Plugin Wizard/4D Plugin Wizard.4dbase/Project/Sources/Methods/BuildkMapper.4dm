//%attributes = {}
_O_ARRAY STRING:C218(32;$arNameArgs;0)
_O_ARRAY STRING:C218(32;$arTypeArgs;0)
_O_ARRAY STRING:C218(32;$arInOutArgs;0)
_O_ARRAY STRING:C218(32;$arConstants;0)

$1:=$1+"kmapper.xml"
$2:=$2+"kmapper.xml"

$needfile:=True:C214
$doit:=True:C214

If (Test path name:C476($1)=1)
	CONFIRM:C162(Char:C90(34)+$1+Char:C90(34)+" already exists in destination folder. OK to overwrite it?")
	$doit:=(ok=1)
	If ($doit)
		DELETE DOCUMENT:C159($1)
	End if 
End if 

If ($doit)
	ALL RECORDS:C47([orderedCommands:9])
	DELETE SELECTION:C66([orderedCommands:9])
	QUERY:C277([Themes:3];[Themes:3]Project:2=[Projects:2]ID:1)
	While (Not:C34(End selection:C36([Themes:3])))
		QUERY:C277([Commands:4];[Commands:4]Theme:2=[Themes:3]ID:1)
		While (Not:C34(End selection:C36([Commands:4])))
			CREATE RECORD:C68([orderedCommands:9])
			[orderedCommands:9]fID:1:=[Commands:4]ID:1
			[orderedCommands:9]fText:2:=[Commands:4]Name:3
			[orderedCommands:9]fTheme:3:=[Themes:3]Name:3
			SAVE RECORD:C53([orderedCommands:9])
			NEXT RECORD:C51([Commands:4])
		End while 
		NEXT RECORD:C51([Themes:3])
	End while 
	
	ALL RECORDS:C47([orderedCommands:9])
	ORDER BY:C49([orderedCommands:9];[orderedCommands:9]fID:1;>)
	FIRST RECORD:C50([orderedCommands:9])
	$arraySize:=Records in selection:C76([orderedCommands:9])
	For ($currentCommand;1;$arraySize)
		
		$offset:=0
		QUERY:C277([Commands:4];[Commands:4]ID:1=[orderedCommands:9]fID:1)
		FIRST RECORD:C50([Commands:4])
		BLOB TO VARIABLE:C533([Commands:4]Parameters:4;$arNameArgs;$offset)
		BLOB TO VARIABLE:C533([Commands:4]Parameters:4;$arTypeArgs;$offset)
		BLOB TO VARIABLE:C533([Commands:4]Parameters:4;$arInOutArgs;$offset)
		BLOB TO VARIABLE:C533([Commands:4]Parameters:4;$arConstants;$offset)
		
		$firstTime:=True:C214
		
		If ($arConstants{$i}#"None")
			
			If ($needFile)
				$ref:=Create document:C266($1)
				SAX SET XML DECLARATION:C858($ref;"UTF-8";False:C215)
				
				SAX OPEN XML ELEMENT:C853($ref;"application";"xmlns";"http://www.4d.com/namespace/reserved/FP/Constants")
				SAX OPEN XML ELEMENT:C853($ref;"appid")
				SAX ADD XML ELEMENT VALUE:C855($ref;"\"0\"")
				SAX CLOSE XML ELEMENT:C854($ref)
				
				SAX OPEN XML ELEMENT:C853($ref;"fourdversmin")
				SAX ADD XML ELEMENT VALUE:C855($ref;"\"9.0.0\"")
				SAX CLOSE XML ELEMENT:C854($ref)
				
				SAX OPEN XML ELEMENT:C853($ref;"fourdversmax")
				SAX ADD XML ELEMENT VALUE:C855($ref;"\"9.0.0\"")
				SAX CLOSE XML ELEMENT:C854($ref)
				
				$needfile:=False:C215
			End if 
			
			QUERY:C277([GroupConstants:7];[GroupConstants:7]Project:2=[Projects:2]ID:1;*)
			QUERY:C277([GroupConstants:7]; & ;[GroupConstants:7]Name:3=$arConstants{$i})
			
			If (Records in selection:C76([GroupConstants:7])>0)
				
				If ($firstTime)
					SAX OPEN XML ELEMENT:C853($ref;"command";"name";[Commands:4]Name:3;"str";String:C10([Projects:2]PluginID:23);"topic";String:C10($currentCommand))
					$firstTime:=False:C215
				End if 
				
				SAX OPEN XML ELEMENT:C853($ref;"param";"position";String:C10($i);"k";String:C10($currentCommand))
				SAX CLOSE XML ELEMENT:C854($ref)  // param
			End if 
			
		End if 
		
		If (Not:C34($firstTime))
			SAX CLOSE XML ELEMENT:C854($ref)  // command
		End if 
		
		NEXT RECORD:C51([orderedCommands:9])
	End for 
	
	If (Not:C34($needfile))
		SAX CLOSE XML ELEMENT:C854($ref)
		CLOSE DOCUMENT:C267($ref)
		COPY DOCUMENT:C541($1;$2;*)
	End if 
End if 