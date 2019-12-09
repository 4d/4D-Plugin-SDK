If ([ExternalAreas:5]Dropable:6)
	OBJECT SET ENTERABLE:C238([ExternalAreas:5]CustomizeDrag:21;True:C214)
Else 
	OBJECT SET ENTERABLE:C238([ExternalAreas:5]CustomizeDrag:21;False:C215)
	[ExternalAreas:5]CustomizeDrag:21:=False:C215
End if 