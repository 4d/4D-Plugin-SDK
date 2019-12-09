Case of 
	: (Form event code:C388=On Load:K2:1)
		_O_ARRAY STRING:C218(32;arReturnValue;0)
		LIST TO ARRAY:C288("ReturnTypes";arReturnValue)
		
		If (Is new record:C668([Commands:4]))
			[Commands:4]ID:1:=Sequence number:C244([Commands:4])
			[Commands:4]Name:3:="new Command"+String:C10([Projects:2]lastCommandNumber:15)
			[Commands:4]Theme:2:=themeID
			_O_ARRAY STRING:C218(32;arNameArgs;0)
			_O_ARRAY STRING:C218(32;arTypeArgs;0)
			_O_ARRAY STRING:C218(32;arInOutArgs;0)
			_O_ARRAY STRING:C218(32;arConstantsArgs;0)
			returnValue:="None"
		Else 
			$offset:=0
			BLOB TO VARIABLE:C533([Commands:4]Parameters:4;arNameArgs;$offset)
			BLOB TO VARIABLE:C533([Commands:4]Parameters:4;arTypeArgs;$offset)
			BLOB TO VARIABLE:C533([Commands:4]Parameters:4;arInOutArgs;$offset)
			BLOB TO VARIABLE:C533([Commands:4]Parameters:4;arConstantsArgs;$offset)
			BLOB TO VARIABLE:C533([Commands:4]Parameters:4;returnValue;$offset)
		End if 
		
		arReturnValue:=Find in array:C230(arReturnValue;returnValue)
End case 