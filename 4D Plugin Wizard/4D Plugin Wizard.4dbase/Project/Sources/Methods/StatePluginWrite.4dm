//%attributes = {}
  //Declaration des variables
ARRAY LONGINT:C221($tabNumeros;0)
ARRAY TEXT:C222($tabNoms;0)
C_BOOLEAN:C305($Valid_Write)

  //Init des variables
$Valid_Write:=False:C215

  //Init de la liste des plugins present
PLUGIN LIST:C847($tabNumeros;$tabNoms)

$nb_element:=Size of array:C274($tabNoms)
For ($n_Plugin;1;$nb_element)
	If ($tabNoms{$n_Plugin}="4D Write")
		$Valid_Write:=True:C214
	End if 
End for 

CLEAR VARIABLE:C89($tabNoms)
CLEAR VARIABLE:C89($tabNumeros)

If ($Valid_Write=False:C215)
	ALERT:C41("Erreur Plugin Write non detecté !!!")
End if 
$0:=$Valid_Write