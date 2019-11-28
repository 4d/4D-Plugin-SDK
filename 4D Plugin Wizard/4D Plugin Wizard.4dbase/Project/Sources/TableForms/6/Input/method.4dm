Case of 
	: (Form event code:C388=On Load:K2:1)
		If (Is new record:C668([Constants:6]))
			[Constants:6]ID:1:=Sequence number:C244([Constants:6])
			[Constants:6]Name:3:="new Constant"+String:C10([Projects:2]lastConstantNumber:19)
			[Constants:6]GroupConst:2:=themeID
			[Constants:6]Type:4:="S"
		End if 
End case 