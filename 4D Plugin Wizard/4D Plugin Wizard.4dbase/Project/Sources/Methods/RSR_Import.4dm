//%attributes = {}
C_TEXT:C284($1)
C_TEXT:C284($resourcePath)
ARRAY LONGINT:C221($ArrayID;0)
ARRAY TEXT:C222($arrayName;0)
C_BLOB:C604($myBlob)
C_BLOB:C604($MyBlob2)
C_LONGINT:C283($RSR_ID)
C_LONGINT:C283($count)
C_LONGINT:C283($id)
ARRAY TEXT:C222($constArray;0)
ARRAY TEXT:C222($MyTab;0)
C_BLOB:C604($constBlob)
C_LONGINT:C283($offset)
C_TEXT:C284($constName)
C_LONGINT:C283($constCount)
C_TEXT:C284($typeDef)
C_LONGINT:C283($constID)

$resourcePath:=$1

  // Arguments specified by the user :
$RSR_ID:=15000

$ImportFile:=Open resource file:C497($resourcePath)
If (Ok=1)
	MESSAGE:C88("Openned the resource file, work in progress..."+Char:C90(Carriage return:K15:38))
	RESOURCE LIST:C500("4BNX";$ArrayID;$arrayName;$ImportFile)
	If (Size of array:C274($ArrayID)>0)
		$RSR_ID:=$ArrayID{1}
		[Projects:2]PluginID:23:=$ArrayID{1}
		[Projects:2]Name:2:=$arrayName{1}
		GET RESOURCE:C508("FON#";$RSR_ID;$MyBlob;$ImportFile)
		GET RESOURCE:C508("THM#";$RSR_ID;$MyBlob2;$ImportFile)
		STRING LIST TO ARRAY:C511($RSR_ID;$MyTab;$ImportFile)
		
		If (BLOB size:C605($myBlob)>0)
			  // let's read the different Themes and create them in our database...
			$offset2:=0
			$NbThemes:=BLOB to integer:C549($MyBlob2;0;$offset2)
			ARRAY LONGINT:C221($ThId;$NbThemes+1)
			  // Create them 0: for deleted commands
			CREATE RECORD:C68([Themes:3])
			[Themes:3]ID:1:=Sequence number:C244([Themes:3])
			MESSAGE:C88(String:C10([Themes:3]ID:1))
			$ThId{1}:=[Themes:3]ID:1
			[Themes:3]Project:2:=[Projects:2]ID:1
			[Themes:3]Name:3:=""
			SAVE RECORD:C53([Themes:3])
			For ($cpt2;2;$NbThemes+1)
				CREATE RECORD:C68([Themes:3])
				[Themes:3]ID:1:=Sequence number:C244([Themes:3])
				MESSAGE:C88(String:C10([Themes:3]ID:1))
				$ThId{$cpt2}:=[Themes:3]ID:1
				[Themes:3]Project:2:=[Projects:2]ID:1
				[Themes:3]Name:3:=BLOB to text:C555($MyBlob2;1;$offset2)
				SAVE RECORD:C53([Themes:3])
			End for 
		Else 
			CREATE RECORD:C68([Themes:3])
			[Themes:3]ID:1:=Sequence number:C244([Themes:3])
			MESSAGE:C88(String:C10([Themes:3]ID:1))
			[Themes:3]Project:2:=[Projects:2]ID:1
			[Themes:3]Name:3:="Theme 1"
			SAVE RECORD:C53([Themes:3])
		End if 
		
		$offset:=0
		For ($cpt;1;Size of array:C274($MyTab))
			CREATE RECORD:C68([Commands:4])
			[Commands:4]ID:1:=Sequence number:C244([Commands:4])
			MESSAGE:C88(String:C10([Commands:4]ID:1))
			
			If (BLOB size:C605($myBlob)>0)
				[Commands:4]Theme:2:=$ThId{$MyBlob{$cpt-1}+1}
			Else 
				[Commands:4]Theme:2:=[Themes:3]ID:1
			End if 
			
			$ind:=Position:C15("(";$MyTab{$cpt})
			If ($ind#0)
				[Commands:4]Name:3:=Substring:C12($MyTab{$cpt};1;$ind-1)
				[Commands:4]Parameters:4:=ArgExtractor (Substring:C12($MyTab{$cpt};$ind))
			Else 
				$ind:=Position:C15(":";$MyTab{$cpt})
				If ($ind#0)
					[Commands:4]Name:3:=Substring:C12($MyTab{$cpt};1;$ind-1)
					[Commands:4]Parameters:4:=ArgExtractor (Substring:C12($MyTab{$cpt};$ind))
				Else 
					[Commands:4]Name:3:=$MyTab{$cpt}
					[Commands:4]Parameters:4:=ArgExtractor ("")
				End if 
			End if 
			SAVE RECORD:C53([Commands:4])
		End for 
	End if 
	
	RESOURCE LIST:C500("4DK#";$ArrayID;$arrayName;$ImportFile)
	$count:=Size of array:C274($ArrayID)
	For ($id;1;$count)
		$constID:=1
		CREATE RECORD:C68([GroupConstants:7])
		[GroupConstants:7]Project:2:=[Projects:2]ID:1
		[GroupConstants:7]Name:3:=$arrayName{$id}
		[GroupConstants:7]ID:1:=Sequence number:C244([GroupConstants:7])
		[GroupConstants:7]resID:4:=$ArrayID{$id}
		SAVE RECORD:C53([GroupConstants:7])
		GET RESOURCE:C508("4DK#";$ArrayID{$id};$constBlob;$ImportFile)
		$constCount:=BLOB size:C605($constBlob)
		$offset:=2
		While ($offset<$constCount)
			$constName:=BLOB to text:C555($constBlob;Mac Pascal string:K22:8;$offset)
			  // create the constant
			CREATE RECORD:C68([Constants:6])
			[Constants:6]GroupConst:2:=[GroupConstants:7]ID:1
			[Constants:6]ID:1:=Sequence number:C244([Constants:6])
			MESSAGE:C88(String:C10([Constants:6]ID:1))
			[Constants:6]ResIndex:8:=$constID
			$constID:=$constID+1
			$length:=Length:C16($constName)
			$constType:=Substring:C12($constName;$length;1)
			$pos:=Position:C15(":";$constName)
			$constValue:=Substring:C12($constName;$pos+1;$length-$pos-2)
			$constName:=Substring:C12($constName;1;$pos-1)
			Case of 
				: (Position:C15($constType;"SsTt")>0)
					[Constants:6]Type:4:="S"
					[Constants:6]StringValue:5:=$constValue
				: (Position:C15($constType;"Ll")>0)
					[Constants:6]Type:4:="L"
					[Constants:6]LongintValue:7:=Num:C11($constValue;".")
				: (Position:C15($constType;"Rr")>0)
					[Constants:6]Type:4:="8"
					[Constants:6]RealValue:6:=Num:C11($constValue;".")
			End case 
			[Constants:6]Name:3:=$constName
			SAVE RECORD:C53([Constants:6])
		End while 
	End for 
	MESSAGE:C88(Char:C90(Carriage return:K15:38))
	MESSAGE:C88("Please wait until this window close...")
	MESSAGE:C88(Char:C90(Carriage return:K15:38))
	CLOSE RESOURCE FILE:C498($ImportFile)
End if 

