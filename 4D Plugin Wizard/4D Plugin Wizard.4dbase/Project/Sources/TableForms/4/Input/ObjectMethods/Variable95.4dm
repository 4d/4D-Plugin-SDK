$n:=Size of array:C274(arNameArgs)
If ($n<25)
	$n:=$n+1
	INSERT IN ARRAY:C227(arNameArgs;$n;1)
	INSERT IN ARRAY:C227(arTypeArgs;$n;1)
	INSERT IN ARRAY:C227(arInOutArgs;$n;1)
	INSERT IN ARRAY:C227(arConstantsArgs;$n;1)
	
	arTypeArgs{$n}:="Long"
	arInOutArgs{$n}:="input"
	arConstantsArgs{$n}:="None"
	arNameArgs{$n}:="Param"+String:C10($n)
Else 
	ALERT:C41("You cannot pass more than 25 parameters to a command")
End if 

