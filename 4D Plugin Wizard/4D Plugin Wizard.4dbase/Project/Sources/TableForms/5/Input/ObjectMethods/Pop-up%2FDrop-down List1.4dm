[ExternalAreas:5]WebEvents:13:=SelectWebMethode
If ([ExternalAreas:5]WebEvents:13<=2)
	[ExternalAreas:5]WebImageMap:14:=False:C215
	OBJECT SET ENTERABLE:C238([ExternalAreas:5]WebImageMap:14;False:C215)
Else 
	OBJECT SET ENTERABLE:C238([ExternalAreas:5]WebImageMap:14;True:C214)
End if 