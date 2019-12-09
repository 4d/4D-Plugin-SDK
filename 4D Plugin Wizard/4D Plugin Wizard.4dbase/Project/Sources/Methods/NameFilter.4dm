//%attributes = {}
  //routine filter
C_TEXT:C284($0;$1)
C_BOOLEAN:C305($2)  //space -> _ (when used in prototype)
C_BOOLEAN:C305($3)  //is an external area
C_BOOLEAN:C305($4)  //is a plugin
_O_C_INTEGER:C282($i)
_O_C_INTEGER:C282($code)

$0:=""

If ($3)
	If ($4)
		If (Substring:C12($1;1;1)#"_")
			If (Substring:C12($1;1;1)="%")
				$1:=Replace string:C233($1;"%";"")
			End if 
			$1:="_"+$1
		End if 
	Else 
		If (Substring:C12($1;1;1)#"%")
			If (Substring:C12($1;1;1)="_")
				$1:=Replace string:C233($1;"_";"";1)
			End if 
			$1:="%"+$1
		End if 
	End if 
Else 
	If (Substring:C12($1;1;1)="_")
		$1:=Replace string:C233($1;"_";"";1)
	End if 
	If (Substring:C12($1;1;1)="%")
		$1:=Replace string:C233($1;"%";"")
	End if 
End if 

For ($i;1;Length:C16($1))
	$code:=Character code:C91($1[[$i]])
	Case of 
		: (($code>=0x0030) & ($code<=0x0039))  //0 9
			$0:=$0+$1[[$i]]
		: (($code>=0x0041) & ($code<=0x005A))  //A Z
			$0:=$0+$1[[$i]]
		: (($code>=0x0061) & ($code<=0x007A))  //a z
			$0:=$0+$1[[$i]]
		: ($code=0x0020)  //space
			$0:=$0+$1[[$i]]
		: ($code=0x005F)  //_
			$0:=$0+$1[[$i]]
		: ($code=0x0025)  //%
			$0:=$0+$1[[$i]]
	End case 
End for 

If ($2)
	$0:=Replace string:C233($0;" ";"_")
	$0:=Replace string:C233($0;"%";"_")
End if 