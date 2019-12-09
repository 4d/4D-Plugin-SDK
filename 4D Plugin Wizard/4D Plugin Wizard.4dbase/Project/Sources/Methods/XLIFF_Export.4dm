//%attributes = {}
C_TEXT:C284($0)  // Will return $xmlRoot
C_TEXT:C284($xmlRoot)

C_LONGINT:C283($currentID)
C_LONGINT:C283($groupsCount)
C_LONGINT:C283($group)
C_LONGINT:C283($constsCount)
C_LONGINT:C283($const)
C_TEXT:C284($xmlGroup)
C_TEXT:C284($xmlConst)
C_TEXT:C284($xmlSource)
C_TEXT:C284($value)
C_LONGINT:C283($pos)


$currentID:=1
  // <xliff version="1.0" xmlns:d4="http://www.4d.com/d4-ns">
$xmlXLIFFRoot:=DOM Create XML Ref:C861("xliff")
DOM SET XML ATTRIBUTE:C866($xmlXLIFFRoot;"version";"1.0")
DOM SET XML ATTRIBUTE:C866($xmlXLIFFRoot;"xmlns:d4";"http://www.4d.com/d4-ns")


  // Groups loop
QUERY:C277([GroupConstants:7];[GroupConstants:7]Project:2=[Projects:2]ID:1)
FIRST RECORD:C50([GroupConstants:7])
$groupsCount:=Records in selection:C76([GroupConstants:7])
For ($group;1;$groupsCount)
	  // <group restype="x-4DK#">
	If ([Projects:2]PluginID:23>=15000)
		$xmlRoot:=DOM Create XML element:C865($xmlXLIFFRoot;"xliff/group";"restype";"x-4DK#";"id";String:C10($currentID))
	Else 
		$xmlRoot:=DOM Create XML element:C865($xmlXLIFFRoot;"xliff/group";"restype";"x-4DK#";"id";String:C10($currentID);"d4:groupID";String:C10([GroupConstants:7]resID:4))
	End if 
	$currentID:=$currentID+1
	  //  // Add <trans-unit id="$currentID"> for current group
	$xmlGroup:=DOM Create XML element:C865($xmlRoot;"group/trans-unit";"id";String:C10($currentID))
	$currentID:=$currentID+1
	$xmlSource:=DOM Create XML element:C865($xmlGroup;"trans-unit/source")
	DOM SET XML ELEMENT VALUE:C868($xmlSource;[GroupConstants:7]Name:3)
	If ([Projects:2]PluginID:23<15000)
		$xmlSource:=DOM Create XML element:C865($xmlGroup;"trans-unit/target")
		DOM SET XML ELEMENT VALUE:C868($xmlSource;[GroupConstants:7]Name:3)
	End if 
	
	  // Constants loop in the group
	QUERY:C277([Constants:6];[Constants:6]GroupConst:2=[GroupConstants:7]ID:1)
	ORDER BY:C49([Constants:6];[Constants:6]ResIndex:8;>)
	$constsCount:=Records in selection:C76([Constants:6])
	FIRST RECORD:C50([Constants:6])
	For ($const;1;$constsCount)
		  // Add <trans-unit id="$currentID" d4:value="$value">
		  // Value added using DOM SET XML ATTRIBUTE so the real will be written with '.' even on french system.
		$xmlConst:=DOM Create XML element:C865($xmlRoot;"group/trans-unit";"id";String:C10($currentID))
		$currentID:=$currentID+1
		Case of 
			: ([Constants:6]Type:4="S")
				  // A string can finish with ":R"(token to force cast to real), ":L" or contain only numbers like "00000000000000"
				  // hard typing avoids errors or bugs for plugin designer.
				$value:=[Constants:6]StringValue:5
				$length:=Length:C16($value)
				If (($length>0) & (Substring:C12($value;$length;1)#":"))
					$firstChar:=Substring:C12($value;1;1)
					$firstCode:=Character code:C91($firstChar)
					$zero:=Character code:C91("0")
					$nine:=Character code:C91("9")
					  // Test to add ":S" and cast to string
					  // "1123:L": have to store "1123:L:S" so it will be readen as a string "1123:L" and not as the long 1123 a cast ? (something like "123:L" will be casted as a long integer 123)
					  // Fast test for int or real: Anything with '.', '-', [0..9] ("123" will be stored as "123:S", plugin will get "123" in the constant)
					If ((Substring:C12($value;$length-1;1)=":") | ($firstChar=".") | ($firstChar="-") | (($firstCode>=$zero) & ($firstCode<=$nine)))
						$value:=$value+":S"
					End if 
				End if 
				DOM SET XML ATTRIBUTE:C866($xmlConst;"d4:value";$value)
			: ([Constants:6]Type:4="8")
				DOM SET XML ATTRIBUTE:C866($xmlConst;"d4:value";[Constants:6]RealValue:6)
				DOM GET XML ATTRIBUTE BY NAME:C728($xmlConst;"d4:value";$value)
				If ((Position:C15(".";$value)<=0) & (Position:C15("E";$value)<=0) & (Position:C15("e";$value)<=0))
					  // It's a real like 123: can be casted to integer
					  // Change it to 123.0 that will be a real when plugin loads.
					$value:=$value+".0"
					DOM SET XML ATTRIBUTE:C866($xmlConst;"d4:value";$value)
				End if 
				
			: ([Constants:6]Type:4="L")
				DOM SET XML ATTRIBUTE:C866($xmlConst;"d4:value";[Constants:6]LongintValue:7)
		End case 
		
		$xmlSource:=DOM Create XML element:C865($xmlConst;"trans-unit/source")
		DOM SET XML ELEMENT VALUE:C868($xmlSource;[Constants:6]Name:3)
		If ([Projects:2]PluginID:23<15000)
			$xmlSource:=DOM Create XML element:C865($xmlConst;"trans-unit/target")
			DOM SET XML ELEMENT VALUE:C868($xmlSource;[Constants:6]Name:3)
		End if 
		
		NEXT RECORD:C51([Constants:6])
	End for 
	
	NEXT RECORD:C51([GroupConstants:7])
End for 

$0:=$xmlXLIFFRoot