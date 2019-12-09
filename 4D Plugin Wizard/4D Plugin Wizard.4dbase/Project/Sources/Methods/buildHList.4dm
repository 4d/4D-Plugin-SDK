//%attributes = {}
C_LONGINT:C283(hlist)
If (Is a list:C621(hlist))
	CLEAR LIST:C377(hlist;*)
End if 
hlist:=New list:C375

FIRST RECORD:C50([Themes:3])
For ($i;1;Records in selection:C76([Themes:3]))
	RELATE MANY:C262([Themes:3]ID:1)
	If (Records in selection:C76([Commands:4])>0)
		$hsublist:=New list:C375
		FIRST RECORD:C50([Commands:4])
		For ($j;1;Records in selection:C76([Commands:4]))
			APPEND TO LIST:C376($hsublist;[Commands:4]Name:3;([Themes:3]ID:1*10000)+[Commands:4]ID:1;0;False:C215)
			NEXT RECORD:C51([Commands:4])
		End for 
		_O_REDRAW LIST:C382($hsublist)
	Else 
		$hsublist:=0
	End if 
	APPEND TO LIST:C376(hlist;[Themes:3]Name:3;([Themes:3]ID:1*10000);$hsublist;True:C214)
	SET LIST ITEM PROPERTIES:C386(hlist;0;True:C214;Bold:K14:2;0)
	NEXT RECORD:C51([Themes:3])
End for 

FIRST RECORD:C50([ExternalAreas:5])
For ($i;1;Records in selection:C76([ExternalAreas:5]))
	APPEND TO LIST:C376(hlist;[ExternalAreas:5]Name:3;-[ExternalAreas:5]ID:1;0;False:C215)
	SET LIST ITEM PROPERTIES:C386(hlist;0;True:C214;Italic:K14:3;0)
	NEXT RECORD:C51([ExternalAreas:5])
End for 

SELECT LIST ITEMS BY POSITION:C381(hlist;1)
_O_REDRAW LIST:C382(hlist)