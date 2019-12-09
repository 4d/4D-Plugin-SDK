CONFIRM:C162("Are you sure?")
If (ok=1)
	$elempos:=Selected list items:C379(hlist)
	If ($elempos>0)
		GET LIST ITEM:C378(hlist;$elempos;$refelem;$textelem)
		If ($refelem>0)
			$themeID:=$refelem\10000
			$commandID:=$refelem%10000
			If ($commandID=0)
				QUERY:C277([Themes:3];[Themes:3]ID:1=$themeID;*)
				QUERY:C277([Themes:3]; & ;[Themes:3]Project:2=[Projects:2]ID:1)
				DELETE RECORD:C58([Themes:3])
				DELETE FROM LIST:C624(hlist;*;*)
			Else 
				QUERY:C277([Commands:4];[Commands:4]ID:1=$CommandID;*)
				QUERY:C277([Commands:4]; & ;[Commands:4]Theme:2=$themeID)
				DELETE RECORD:C58([Commands:4])
				DELETE FROM LIST:C624(hlist;*)
			End if 
		Else 
			QUERY:C277([ExternalAreas:5];[ExternalAreas:5]ID:1=-$refelem;*)
			QUERY:C277([ExternalAreas:5]; & ;[ExternalAreas:5]Project:2=[Projects:2]ID:1)
			DELETE RECORD:C58([ExternalAreas:5])
			DELETE FROM LIST:C624(hlist;*)
		End if 
		_O_REDRAW LIST:C382(hlist)
		UpdateStatusCommand 
		BuildSource 
	End if 
End if 