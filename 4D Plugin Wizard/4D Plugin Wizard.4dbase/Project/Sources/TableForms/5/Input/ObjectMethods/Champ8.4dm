If ([ExternalAreas:5]Focusable:5)
	OBJECT SET ENTERABLE:C238([ExternalAreas:5]SelectEvt:12;True:C214)
	OBJECT SET ENTERABLE:C238([ExternalAreas:5]DeselectEvt:7;True:C214)
Else 
	OBJECT SET ENTERABLE:C238([ExternalAreas:5]SelectEvt:12;False:C215)
	OBJECT SET ENTERABLE:C238([ExternalAreas:5]DeselectEvt:7;False:C215)
	[ExternalAreas:5]SelectEvt:12:=False:C215
	[ExternalAreas:5]DeselectEvt:7:=False:C215
End if 