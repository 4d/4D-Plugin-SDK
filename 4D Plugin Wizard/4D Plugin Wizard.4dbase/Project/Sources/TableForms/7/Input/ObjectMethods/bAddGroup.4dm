$nomtheme:=Request:C163("Name ?";"new Group"+String:C10([Projects:2]lastGroupNumber:18))
If (ok=1)
	[Projects:2]lastGroupNumber:18:=[Projects:2]lastGroupNumber:18+1
	CREATE RECORD:C68([GroupConstants:7])
	[GroupConstants:7]ID:1:=Sequence number:C244([GroupConstants:7])
	[GroupConstants:7]Name:3:=$nomtheme
	[GroupConstants:7]Project:2:=[Projects:2]ID:1
	SAVE RECORD:C53([GroupConstants:7])
	APPEND TO LIST:C376(hlist2;[GroupConstants:7]Name:3;[GroupConstants:7]ID:1*10000)
	SET LIST ITEM PROPERTIES:C386(hlist2;0;True:C214;Bold:K14:2;0)
	SELECT LIST ITEMS BY REFERENCE:C630(hlist2;[GroupConstants:7]ID:1*10000)
	_O_REDRAW LIST:C382(hlist2)
	UNLOAD RECORD:C212([GroupConstants:7])
End if 