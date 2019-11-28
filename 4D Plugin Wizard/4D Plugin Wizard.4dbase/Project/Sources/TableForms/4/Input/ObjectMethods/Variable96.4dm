CONFIRM:C162("Are you sure?";"Yes";"No")
If (OK=1)
	$n:=arTypeArgs
	DELETE FROM ARRAY:C228(arNameArgs;$n;1)
	DELETE FROM ARRAY:C228(arTypeArgs;$n;1)
	DELETE FROM ARRAY:C228(arInOutArgs;$n;1)
	DELETE FROM ARRAY:C228(arConstantsArgs;$n;1)
End if 
