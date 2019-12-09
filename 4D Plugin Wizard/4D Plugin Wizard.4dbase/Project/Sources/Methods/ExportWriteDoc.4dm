//%attributes = {}
  // $1 : pathname
  // $2 : filename
  // $3 : area

$doit:=True:C214
$valueTest:=(Test path name:C476($1+$2)=1)

If (($valueTest=True:C214) & (hasConfirmed=False:C215))
	CONFIRM:C162("Similar files already exists in the folder, do you want to continue ?")
	hasConfirmed:=(ok=1)
	
End if 

If ((hasConfirmed=True:C214) | ($valueTest=False:C215))
	If ($valueTest=True:C214)
		DELETE DOCUMENT:C159($1+$2+$3)
	End if 
	C_TEXT:C284($text)
	$text:=$4
	C_BLOB:C604($blob)
	TEXT TO BLOB:C554($text;$blob;UTF8 text without length:K22:17)
	
	CONVERT FROM TEXT:C1011(Convert to text:C1012($blob;"utf-8");"utf-8";$blob)
	BLOB TO DOCUMENT:C526($1+$2+$3;$blob)
End if 

  //  // $1 : pathname
  //  // $2 : filename
  //  // $3 : area

  //$doit:=True
  //If (Test path name($1+$2+$3)=1)
  //CONFIRM(Char(34)+$2+$3+Char(34)+" already exists in destination folder. OK to overwrite it?")
  //$doit:=(ok=1)
  //If ($doit)
  //DELETE DOCUMENT($1+$2+$3)
  //End if 
  //End if 

  //If ($doit)
  //TEXT TO DOCUMENT($1+$2+$3;$4)
  //End if  