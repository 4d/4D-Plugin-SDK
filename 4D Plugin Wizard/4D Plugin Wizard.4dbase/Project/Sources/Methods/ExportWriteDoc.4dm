//%attributes = {}
// $1 : pathname
// $2 : filename
// $3 : area

$doit:=True:C214
var $folder : 4D:C1709.Folder
$folder:=Folder:C1567($1; fk platform path:K87:2)
var $file : 4D:C1709.File
$file:=$folder.file($2+$3)
$valueTest:=$file.exists

If (($valueTest=True:C214) & (hasConfirmed=False:C215))
	CONFIRM:C162("Similar files already exists in the folder, do you want to continue ?")
	hasConfirmed:=(ok=1)
	
End if 

If ((hasConfirmed=True:C214) | ($valueTest=False:C215))
	
	If ($valueTest=True:C214)
		$file.delete()
	End if 
	
	$file.setText($4)
End if 
