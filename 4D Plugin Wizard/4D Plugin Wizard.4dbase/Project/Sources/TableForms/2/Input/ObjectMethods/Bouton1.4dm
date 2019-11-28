$nomtheme:=Request:C163("Name ?";"new theme"+String:C10([Projects:2]lastThemeNumber:16))
If (ok=1)
	[Projects:2]lastThemeNumber:16:=[Projects:2]lastThemeNumber:16+1
	CREATE RECORD:C68([Themes:3])
	[Themes:3]ID:1:=Sequence number:C244([Themes:3])
	[Themes:3]Name:3:=$nomtheme
	[Themes:3]Project:2:=[Projects:2]ID:1
	SAVE RECORD:C53([Themes:3])
	APPEND TO LIST:C376(hlist;[Themes:3]Name:3;[Themes:3]ID:1*10000)
	SET LIST ITEM PROPERTIES:C386(hlist;0;True:C214;Bold:K14:2;0)
	SELECT LIST ITEMS BY REFERENCE:C630(hlist;[Themes:3]ID:1*10000)
	_O_REDRAW LIST:C382(hlist)
	UNLOAD RECORD:C212([Themes:3])
	OBJECT SET ENABLED:C1123(*;"Bouton2";True:C214)
	
	BuildSource 
End if 