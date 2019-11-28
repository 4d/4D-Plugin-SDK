If (Form event code:C388=On Load:K2:1)
	If (Is new record:C668([ExternalAreas:5]))
		[ExternalAreas:5]ID:1:=Sequence number:C244([ExternalAreas:5])
		[ExternalAreas:5]Name:3:="Plugin Area"+String:C10([Projects:2]lastExternalAreaNumber:17)
		[ExternalAreas:5]Project:2:=[Projects:2]ID:1
		[ExternalAreas:5]WebEvents:13:=1
		[ExternalAreas:5]WebImageMap:14:=False:C215
	End if 
	SelectWebMethode:=[ExternalAreas:5]WebEvents:13
	If ([ExternalAreas:5]WebEvents:13<=2)
		OBJECT SET ENTERABLE:C238([ExternalAreas:5]WebImageMap:14;False:C215)
		[ExternalAreas:5]WebImageMap:14:=False:C215
	Else 
		OBJECT SET ENTERABLE:C238([ExternalAreas:5]WebImageMap:14;True:C214)
	End if 
End if 