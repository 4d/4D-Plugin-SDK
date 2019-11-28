//%attributes = {}
C_TEXT:C284($1)
C_TEXT:C284($content)
C_BOOLEAN:C305($2)
$content:=ColorizeText ("#include ";gKeyWordColor)

If ($2)
	$content:=$content+Char:C90(34)+$1+Char:C90(34)+vcr
Else 
	$content:=$content+"&lt;"+$1+"&gt;"+vcr
End if 
$0:=$content
