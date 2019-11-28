If (Form event code:C388=On Load:K2:1)
	Case of 
		: ([Constants:6]Type:4="S")
			vConstantType:=1
			FORM GOTO PAGE:C247(1)
		: ([Constants:6]Type:4="8")
			vConstantType:=2
			FORM GOTO PAGE:C247(2)
		: ([Constants:6]Type:4="L")
			vConstantType:=3
			FORM GOTO PAGE:C247(3)
	End case 
Else 
	Case of 
		: (vConstantType=1)
			[Constants:6]Type:4:="S"
			[Constants:6]StringValue:5:=""
			FORM GOTO PAGE:C247(1)
		: (vConstantType=2)
			[Constants:6]Type:4:="8"
			[Constants:6]RealValue:6:=0
			FORM GOTO PAGE:C247(2)
		: (vConstantType=3)
			[Constants:6]Type:4:="L"
			[Constants:6]LongintValue:7:=0
			FORM GOTO PAGE:C247(3)
	End case 
End if 