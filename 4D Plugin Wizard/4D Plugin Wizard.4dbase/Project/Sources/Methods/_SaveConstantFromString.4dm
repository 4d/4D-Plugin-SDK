//%attributes = {}
C_TEXT:C284($1)
C_TEXT:C284($inValue)
$inValue:=$1

C_BOOLEAN:C305($hasDecimal)
C_BOOLEAN:C305($hasExp)
C_BOOLEAN:C305($hasNumber)
C_BOOLEAN:C305($hasMinus)
C_BOOLEAN:C305($hasDot)
C_TEXT:C284($temp)

C_LONGINT:C283($zero)
C_LONGINT:C283($nine)
$zero:=Character code:C91("0")
$nine:=Character code:C91("9")

$hasDecimal:=False:C215
$hasExp:=False:C215
$hasNumber:=False:C215
$hasMinus:=False:C215
$hasDot:=False:C215
C_LONGINT:C283($length)


[Constants:6]Type:4:="S"
[Constants:6]StringValue:5:=$inValue

$length:=Length:C16($inValue)

  // Verify if it's a typed stuff
If ($length>=3)
	$type:=Substring:C12($inValue;$length-2;2)
	If ($type#"::")
		If (Substring:C12($inValue;$length-1;1)=":")
			$type:=Substring:C12($inValue;$length;1)
			Case of 
				: (Position:C15($type;"tTsS")>0)
					[Constants:6]StringValue:5:=Substring:C12($inValue;1;$length-2)
				: (Position:C15($type;"Ll")>0)
					[Constants:6]LongintValue:7:=Num:C11(Substring:C12($inValue;1;$length-2))
					[Constants:6]Type:4:="L"
				: (Position:C15($type;"Rr")>0)
					[Constants:6]RealValue:6:=Num:C11(Substring:C12($inValue;1;$length-2);".")
					[Constants:6]Type:4:="8"
				Else 
					$length:=0
			End case 
		End if 
	End if 
End if 

If ($length>0)
	[Constants:6]Type:4:="L"
	
	For ($i;1;$length)
		$char:=Substring:C12($inValue;$i;1)
		$charCode:=Character code:C91($char)
		Case of 
			: ($char="-")
				If (($hasMinus=True:C214) | ($hasNumber=True:C214) | ($hasDot=True:C214))
					[Constants:6]Type:4:="S"
					$i:=$length+1
				Else 
					$hasMinus:=True:C214
				End if 
				
			: ($char=".")
				If (($hasDot=True:C214) | ($hasExp=True:C214))
					[Constants:6]Type:4:="S"
					$i:=$length+1
				Else 
					$hasDot:=True:C214
					[Constants:6]Type:4:="8"
				End if 
				
			: (($char="E") | ($char="e"))
				If (($hasNumber=False:C215) | ($hasExp=True:C214))
					[Constants:6]Type:4:="S"
					$i:=$length+1
				Else 
					[Constants:6]Type:4:="8"
					$hasMinus:=False:C215
					$hasExp:=True:C214
					$hasNumber:=False:C215  // Can have 'E-' but can't have 'E0'
					hasDot:=False:C215  // Can have 12.123E-3
				End if 
				
			: (($charCode>$zero) & ($charCode<=$nine))
				$hasNumber:=True:C214
				
			: ($char="0")
				  // Enables "0.154151" as a num but "00023" will be a string
				If ($hasNumber=False:C215)
					If (($i=1) & ($length>=2))
						$temp:=Substring:C12($inValue;2;1)
						If ($temp#".")
							[Constants:6]Type:4:="S"
							$i:=$length+1
						End if 
					Else 
						[Constants:6]Type:4:="S"
						$i:=$length+1
					End if 
				End if 
				
			Else 
				[Constants:6]Type:4:="S"
				$i:=$length+1
				
		End case 
	End for 
	
	Case of 
		: ([Constants:6]Type:4="L")
			[Constants:6]LongintValue:7:=Num:C11($inValue)
		: ([Constants:6]Type:4="8")
			[Constants:6]RealValue:6:=Num:C11($inValue;".")
	End case 
End if 
