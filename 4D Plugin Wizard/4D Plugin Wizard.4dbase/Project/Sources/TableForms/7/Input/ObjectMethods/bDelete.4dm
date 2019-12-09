
$elempos:=Selected list items:C379(hlist2)
If ($elempos>0)
	CONFIRM:C162("Are you sure?")
	If (ok=1)
		GET LIST ITEM:C378(hlist2;$elempos;$refelem;$textelem)
		$themeID:=$refelem\10000
		$commandID:=$refelem%10000
		If ($commandID=0)
			QUERY:C277([GroupConstants:7];[GroupConstants:7]ID:1=$themeID;*)
			QUERY:C277([GroupConstants:7]; & ;[GroupConstants:7]Project:2=[Projects:2]ID:1)
			DELETE RECORD:C58([GroupConstants:7])
			DELETE FROM LIST:C624(hlist2;*;*)
		Else 
			QUERY:C277([Constants:6];[Constants:6]ID:1=$CommandID;*)
			QUERY:C277([Constants:6]; & ;[Constants:6]GroupConst:2=$themeID)
			DELETE RECORD:C58([Constants:6])
			DELETE FROM LIST:C624(hlist2;*)
		End if 
		_O_REDRAW LIST:C382(hlist2)
		BuildSource 
	End if 
End if 