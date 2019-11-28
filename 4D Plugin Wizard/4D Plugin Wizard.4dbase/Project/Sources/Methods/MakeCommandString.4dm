//%attributes = {}
_O_ARRAY STRING:C218(32;$arNameArgs;0)
_O_ARRAY STRING:C218(32;$arTypeArgs;0)
_O_ARRAY STRING:C218(32;$arInOutArgs;0)
_O_ARRAY STRING:C218(32;$arConstants;0)
_O_C_STRING:C293(32;$returnValue)

$offset:=0
BLOB TO VARIABLE:C533([Commands:4]Parameters:4;$arNameArgs;$offset)
BLOB TO VARIABLE:C533([Commands:4]Parameters:4;$arTypeArgs;$offset)
BLOB TO VARIABLE:C533([Commands:4]Parameters:4;$arInOutArgs;$offset)
BLOB TO VARIABLE:C533([Commands:4]Parameters:4;$arConstants;$offset)
BLOB TO VARIABLE:C533([Commands:4]Parameters:4;$returnValue;$offset)

$0:=[Commands:4]Name:3

If (Size of array:C274($arTypeArgs)>0)
	
	$0:=$0+"("
	
	For ($i;1;Size of array:C274($arTypeArgs))
		
		Case of 
			: ($arTypeArgs{$i}="Date")
				$0:=$0+"&D"
			: ($arTypeArgs{$i}="Hour")
				$0:=$0+"&H"
			: ($arTypeArgs{$i}="Integer")
				$0:=$0+"&I"
			: ($arTypeArgs{$i}="Long")
				$0:=$0+"&L"
			: ($arTypeArgs{$i}="Blob")
				$0:=$0+"&O"
			: ($arTypeArgs{$i}="Picture")
				$0:=$0+"&P"
			: ($arTypeArgs{$i}="Real")
				$0:=$0+"&R"
			: ($arTypeArgs{$i}="Double")  // ACI0088076
				$0:=$0+"&8"
			: ($arTypeArgs{$i}="Text")
				$0:=$0+"&T"
			: ($arTypeArgs{$i}="String")  // ACI0088076
				$0:=$0+"&S"
			: ($arTypeArgs{$i}="UTXT")  // ACI0088076
				$0:=$0+"&U"
			: ($arTypeArgs{$i}="Array")
				$0:=$0+"&Y"
			: ($arTypeArgs{$i}="Object")  // ACI0097817
				$0:=$0+"&J"
			: ($arTypeArgs{$i}="Collection")  // ACI0097817
				$0:=$0+"&C"
			: ($arTypeArgs{$i}="Pointer")
				$0:=$0+"&Z"
		End case 
		
		If ($i<Size of array:C274($arTypeArgs))
			$0:=$0+";"
		End if 
		
	End for 
	
	
	$0:=$0+")"
End if 

Case of 
	: ($returnValue="Date")
		$0:=$0+":D"
	: ($returnValue="Hour")
		$0:=$0+":H"
	: ($returnValue="Integer")
		$0:=$0+":I"
	: ($returnValue="Long")
		$0:=$0+":L"
	: ($returnValue="Blob")
		$0:=$0+":O"
	: ($returnValue="Object")  // ACI0097817
		$0:=$0+":J"
	: ($returnValue="Collection")  // ACI0097817
		$0:=$0+":C"
	: ($returnValue="Picture")
		$0:=$0+":P"
	: ($returnValue="Real")
		$0:=$0+":R"
	: ($returnValue="Text")
		$0:=$0+":T"
	: ($returnValue="String")  // ACI0088076
		$0:=$0+":S"
	: ($returnValue="UTXT")  // ACI0088076
		$0:=$0+":U"
	: ($returnValue="Double")  // ACI0088076
		$0:=$0+":8"
End case 
