Case of 
	: (Form event code:C388=On Double Clicked:K2:5)
		$pos:=Selected list items:C379(hlist2)
		If ($pos>0)
			GET LIST ITEM:C378(hlist2;$pos;$refelem;$name)
			$GroupConstID:=$refelem\10000
			$constantID:=$refelem%10000
			QUERY:C277([Constants:6];[Constants:6]ID:1=$constantID;*)
			QUERY:C277([Constants:6]; & ;[Constants:6]GroupConst:2=$GroupConstID)
			$f:=Open form window:C675([Constants:6];"Input";5;Horizontally centered:K39:1;Vertically centered:K39:4)
			MODIFY RECORD:C57([Constants:6];*)
			CLOSE WINDOW:C154
			If (ok=1)
				$name:=[Constants:6]Name:3
				SET LIST ITEM:C385(hlist2;$refelem;$name;$refelem)
				_O_REDRAW LIST:C382(hlist2)
				BuildSource 
			End if 
			UNLOAD RECORD:C212([Constants:6])
		End if 
		
	: (Form event code:C388=On Data Change:K2:15)
		If (Is a list:C621(hlist2))
			$pos:=Selected list items:C379(hlist2)
			If ($pos>0)
				GET LIST ITEM:C378(hlist2;$pos;$refelem;$name)
				If (Length:C16($name)>31)
					$name:=Substring:C12($name;1;31)
					SET LIST ITEM:C385(hlist2;$refelem;$name;$refelem)
					_O_REDRAW LIST:C382(hlist2)
				End if 
				$GroupConstID:=$refelem\10000
				$constantID:=$refelem%10000
				If ($constantID=0)
					QUERY:C277([GroupConstants:7];[GroupConstants:7]ID:1=$GroupConstID;*)
					QUERY:C277([GroupConstants:7]; & ;[GroupConstants:7]Project:2=[Projects:2]ID:1)
					[GroupConstants:7]Name:3:=$name
					SAVE RECORD:C53([GroupConstants:7])
					UNLOAD RECORD:C212([GroupConstants:7])
				Else 
					QUERY:C277([Constants:6];[Constants:6]ID:1=$constantID;*)
					QUERY:C277([Constants:6]; & ;[Constants:6]GroupConst:2=$GroupConstID)
					[Constants:6]Name:3:=$name
					SAVE RECORD:C53([Constants:6])
					UNLOAD RECORD:C212([Constants:6])
				End if 
				BuildSource 
			End if 
		End if 
End case 