//%attributes = {}
$pos:=0
C_BLOB:C604($MyBlob;0)
_O_ARRAY STRING:C218(32;$arNameArgs2;0)
_O_ARRAY STRING:C218(32;$arTypeArgs2;0)
_O_ARRAY STRING:C218(32;$arInOutArgs2;0)
_O_ARRAY STRING:C218(32;$arConstantsArgs2;0)
_O_C_STRING:C293(32;$returnValue2)

$returnValue2:="None"
$offset:=0
$k:=0
$len:=Length:C16($1)
If ($len>2)
	While ($pos<$len)
		If (Substring:C12($1;$pos;1)="&")
			$pos:=$pos+1
			$k:=$k+1
			$car:=Substring:C12($1;$pos;1)
			INSERT IN ARRAY:C227($arNameArgs2;Size of array:C274($arNameArgs2)+1;1)
			INSERT IN ARRAY:C227($arTypeArgs2;Size of array:C274($arTypeArgs2)+1;1)
			INSERT IN ARRAY:C227($arInOutArgs2;Size of array:C274($arInOutArgs2)+1;1)
			INSERT IN ARRAY:C227($arConstantsArgs2;Size of array:C274($arConstantsArgs2)+1;1)
			$arNameArgs2{$k}:="Arg"+String:C10($k)
			$arTypeArgs2{$k}:=ParameterTypeLetterConversion ($car)
			$arInOutArgs2{$k}:="input"
			$arConstantsArgs2{$k}:="None"
		End if 
		$pos:=$pos+1
	End while 
	$returnValue2:=ParameterTypeLetterConversion (Substring:C12($1;Length:C16($1);1))
Else 
	If ($len<2)
		$returnValue2:=""
	Else 
		$returnValue2:=ParameterTypeLetterConversion (Substring:C12($1;2;1))
	End if 
End if 

VARIABLE TO BLOB:C532($arNameArgs2;$MyBlob)
VARIABLE TO BLOB:C532($arTypeArgs2;$MyBlob;*)
VARIABLE TO BLOB:C532($arInOutArgs2;$MyBlob;*)
VARIABLE TO BLOB:C532($arConstantsArgs2;$MyBlob;*)
If ($returnValue2#"None")
	VARIABLE TO BLOB:C532($returnValue2;$MyBlob;*)
End if 
$0:=$MyBlob
