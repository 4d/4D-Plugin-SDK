$elempos:=Selected list items:C379(hlist)
If (Count list items:C380(hlist)>0)
	
	GET LIST ITEM:C378(hlist;$elempos;$refelem;$textelem;$sublist)
	If ($refelem>0)
		QUERY:C277([GroupConstants:7];[GroupConstants:7]Project:2=[Projects:2]ID:1)
		SELECTION TO ARRAY:C260([GroupConstants:7]Name:3;arConstants)
		INSERT IN ARRAY:C227(arConstants;1;1)
		arConstants{1}:="None"
		ARRAY TO LIST:C287(arConstants;"ConstantsGroups")
		
		themeID:=$refelem\10000
		$f:=Open form window:C675([Commands:4];"Input";5;Horizontally centered:K39:1;Vertically centered:K39:4)
		ADD RECORD:C56([Commands:4];*)
		CLOSE WINDOW:C154
		If (ok=1)
			SELECT LIST ITEMS BY REFERENCE:C630(hlist;themeID*10000)
			$elempos:=Selected list items:C379(hlist)
			GET LIST ITEM:C378(hlist;$elempos;$refelem;$textelem;$sublist)
			If (Not:C34(Is a list:C621($sublist)))
				$sublist:=New list:C375
			End if 
			APPEND TO LIST:C376($sublist;[Commands:4]Name:3;(themeID*10000)+[Commands:4]ID:1)
			SET LIST ITEM:C385(hlist;$refelem;$textelem;$refelem;$sublist;True:C214)
			REDRAW:C174($sublist)
			SELECT LIST ITEMS BY REFERENCE:C630(hlist;(themeID*10000)+[Commands:4]ID:1)
			_O_REDRAW LIST:C382(hlist)
			
			UNLOAD RECORD:C212([Commands:4])
			BuildSource 
		End if 
	End if 
Else 
	OBJECT SET ENABLED:C1123(*;"Bouton2";False:C215)
End if 