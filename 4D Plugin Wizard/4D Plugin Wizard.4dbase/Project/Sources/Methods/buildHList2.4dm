//%attributes = {}
C_LONGINT:C283(hlist2)
If (Is a list:C621(hlist2))
	CLEAR LIST:C377(hlist2;*)
End if 
hlist2:=New list:C375
ALL RECORDS:C47([GroupConstants:7])
QUERY:C277([GroupConstants:7];[GroupConstants:7]Project:2=[Projects:2]ID:1)
FIRST RECORD:C50([GroupConstants:7])
For ($i;1;Records in selection:C76([GroupConstants:7]))
	RELATE MANY:C262([GroupConstants:7]ID:1)
	If (Records in selection:C76([Constants:6])>0)
		$hsublist:=New list:C375
		FIRST RECORD:C50([Constants:6])
		For ($j;1;Records in selection:C76([Constants:6]))
			APPEND TO LIST:C376($hsublist;[Constants:6]Name:3;([GroupConstants:7]ID:1*10000)+[Constants:6]ID:1;0;False:C215)
			NEXT RECORD:C51([Constants:6])
		End for 
		_O_REDRAW LIST:C382($hsublist)
	Else 
		$hsublist:=0
	End if 
	APPEND TO LIST:C376(hlist2;[GroupConstants:7]Name:3;([GroupConstants:7]ID:1*10000);$hsublist;True:C214)
	SET LIST ITEM PROPERTIES:C386(hlist2;0;True:C214;Bold:K14:2;0)
	NEXT RECORD:C51([GroupConstants:7])
End for 

SELECT LIST ITEMS BY POSITION:C381(hlist2;1)
_O_REDRAW LIST:C382(hlist2)