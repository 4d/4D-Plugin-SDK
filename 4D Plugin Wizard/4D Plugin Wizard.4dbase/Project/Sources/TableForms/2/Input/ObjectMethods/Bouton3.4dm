$f:=Open form window:C675([ExternalAreas:5];"Input";5;Horizontally centered:K39:1;Vertically centered:K39:4)
ADD RECORD:C56([ExternalAreas:5];*)
CLOSE WINDOW:C154
If (ok=1)
	[Projects:2]lastExternalAreaNumber:17:=[Projects:2]lastExternalAreaNumber:17+1
	APPEND TO LIST:C376(hlist;[ExternalAreas:5]Name:3;-[ExternalAreas:5]ID:1)
	SET LIST ITEM PROPERTIES:C386(hlist;0;True:C214;Italic:K14:3;0)
	SELECT LIST ITEMS BY REFERENCE:C630(hlist;-[ExternalAreas:5]ID:1)
	_O_REDRAW LIST:C382(hlist)
	UpdateStatusCommand 
	UNLOAD RECORD:C212([ExternalAreas:5])
	BuildSource 
End if 