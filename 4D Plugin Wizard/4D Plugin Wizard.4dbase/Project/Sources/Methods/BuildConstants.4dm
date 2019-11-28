//%attributes = {}
C_TEXT:C284($content)
If ([Projects:2]ShowConstants:22)
	QUERY:C277([GroupConstants:7];[GroupConstants:7]Project:2=[Projects:2]ID:1)
	FIRST RECORD:C50([GroupConstants:7])
	
	For ($i;1;Records in selection:C76([GroupConstants:7]))
		
		$content:=put ($content+"// --- "+[GroupConstants:7]Name:3)
		QUERY:C277([Constants:6];[Constants:6]GroupConst:2=[GroupConstants:7]ID:1)
		For ($j;1;Records in selection:C76([Constants:6]))
			$content:=$content+kw ("#define ")
			$content:=$content+(NameFilter ([Constants:6]Name:3;True:C214;False:C215)+vtab)
			Case of 
				: ([Constants:6]Type:4="S")
					$content:=put ($content+Char:C90(34)+[Constants:6]StringValue:5+Char:C90(34))
				: ([Constants:6]Type:4="8")
					$content:=put ($content+String:C10([Constants:6]RealValue:6))
				: ([Constants:6]Type:4="L")
					$content:=put ($content+String:C10([Constants:6]LongintValue:7))
			End case 
			NEXT RECORD:C51([Constants:6])
		End for 
		NEXT RECORD:C51([GroupConstants:7])
	End for 
	$content:=put ($content)
End if 
$0:=CommentColor ($content)