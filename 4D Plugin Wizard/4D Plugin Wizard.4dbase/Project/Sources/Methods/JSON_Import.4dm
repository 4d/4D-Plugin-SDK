//%attributes = {}
C_TEXT:C284($1)
C_TEXT:C284($jsonPath)
$jsonPath:=$1

C_TIME:C306($jsonFile)
ARRAY OBJECT:C1221($methodsArray;0)
ARRAY TEXT:C222($themes;0)
C_OBJECT:C1216($jsonStuff)
C_TEXT:C284($jsonStr)
C_LONGINT:C283($arraySize)
C_LONGINT:C283($pos)
C_LONGINT:C283($pluginID)
C_TEXT:C284($pluginName)
C_BOOLEAN:C305($threadSafe)
  // Open the file
$jsonFile:=Open document:C264($jsonPath)
If ($jsonFile#?00:00:00?)
	  // Get methods
	$jsonStr:=Document to text:C1236(Document)
	CLOSE DOCUMENT:C267($jsonFile)
	$jsonStuff:=JSON Parse:C1218($jsonStr)
	
	If (OK=1)
		  // get the methods
		OB GET ARRAY:C1229($jsonStuff;"commands";$methodsArray)
		  // commands loop
		$arraySize:=Size of array:C274($methodsArray)
		For ($i;1;$arraySize)
			$strValue:=OB Get:C1224($methodsArray{$i};"theme")
			QUERY:C277([Themes:3];[Themes:3]Project:2=[Projects:2]ID:1;*)
			QUERY:C277([Themes:3]; & ;[Themes:3]Name:3=$strValue)
			If (Records in selection:C76([Themes:3])>0)
				FIRST RECORD:C50([Themes:3])
			Else 
				CREATE RECORD:C68([Themes:3])
				[Themes:3]ID:1:=Sequence number:C244([Themes:3])
				[Themes:3]Project:2:=[Projects:2]ID:1
				[Themes:3]Name:3:=$strValue
				SAVE RECORD:C53([Themes:3])
			End if 
			
			CREATE RECORD:C68([Commands:4])
			$threadSafe:=False:C215
			If (OB Is defined:C1231($methodsArray{$i};"threadSafe"))
				$threadSafe:=OB Get:C1224($methodsArray{$i};"threadSafe")
			End if 
			$strValue:=OB Get:C1224($methodsArray{$i};"syntax")
			[Commands:4]Theme:2:=[Themes:3]ID:1
			[Commands:4]ID:1:=Sequence number:C244([Commands:4])
			[Commands:4]ThreadSafe:7:=$threadSafe
			$pos:=Position:C15("(";$strValue)
			If ($pos#0)
				[Commands:4]Name:3:=Substring:C12($strValue;1;$pos-1)
				[Commands:4]Parameters:4:=ArgExtractor (Substring:C12($strValue;$pos))
			Else 
				$pos:=Position:C15(":";$strValue)
				If ($pos#0)
					[Commands:4]Name:3:=Substring:C12($strValue;1;$pos-1)
					[Commands:4]Parameters:4:=ArgExtractor (Substring:C12($strValue;$pos))
				Else 
					[Commands:4]Name:3:=$strValue
					[Commands:4]Parameters:4:=ArgExtractor ("")
				End if 
			End if 
			SAVE RECORD:C53([Commands:4])
			
		End for 
	End if 
End if 
