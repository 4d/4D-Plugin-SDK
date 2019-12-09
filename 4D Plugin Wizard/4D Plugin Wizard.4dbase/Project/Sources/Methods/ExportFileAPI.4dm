//%attributes = {}
  // $1 : dest (full path)
  // $2 : source (relative path in Extra folder)
C_TEXT:C284($extra)
$valueTest:=(Test path name:C476($1+$2)=1)
If ($valueTest=True:C214)
	If (hasConfirmed=False:C215)
		CONFIRM:C162("Similar files already exists in the folder, do you want to continue ?")
		hasConfirmed:=(ok=1)
	End if 
End if 

If ((hasConfirmed=True:C214) | ($valueTest=False:C215))
	COPY DOCUMENT:C541(pathPlugin+$2;$1+$2;*)
	GET DOCUMENT PROPERTIES:C477($1+$2;$lock;$visible;$a;$b;$c;$d)
	SET DOCUMENT PROPERTIES:C478($1+$2;False:C215;False:C215;$a;$b;$c;$d)
End if 
