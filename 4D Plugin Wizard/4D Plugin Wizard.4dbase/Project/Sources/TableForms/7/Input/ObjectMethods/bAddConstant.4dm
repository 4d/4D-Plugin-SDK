$elempos:=Selected list items:C379(hlist2)
If ((Count list items:C380(hlist2)>0) & ($elempos>0))
	GET LIST ITEM:C378(hlist2;$elempos;$refelem;$textelem;$sublist)
	themeID:=$refelem\10000
	$f:=Open form window:C675([Constants:6];"Input";5;Horizontally centered:K39:1;Vertically centered:K39:4)
	ADD RECORD:C56([Constants:6];*)
	CLOSE WINDOW:C154
	If (ok=1)
		SELECT LIST ITEMS BY REFERENCE:C630(hlist2;themeID*10000)
		$elempos:=Selected list items:C379(hlist2)
		GET LIST ITEM:C378(hlist2;$elempos;$refelem;$textelem;$sublist)
		If (Not:C34(Is a list:C621($sublist)))
			$sublist:=New list:C375
		End if 
		APPEND TO LIST:C376($sublist;[Constants:6]Name:3;(themeID*10000)+[Constants:6]ID:1)
		SET LIST ITEM:C385(hlist2;$refelem;$textelem;$refelem;$sublist;True:C214)
		REDRAW:C174($sublist)
		SELECT LIST ITEMS BY REFERENCE:C630(hlist2;(themeID*10000)+[Constants:6]ID:1)
		_O_REDRAW LIST:C382(hlist2)
		[Projects:2]lastConstantNumber:19:=[Projects:2]lastConstantNumber:19+1
		UNLOAD RECORD:C212([Constants:6])
		BuildSource 
	End if 
End if 