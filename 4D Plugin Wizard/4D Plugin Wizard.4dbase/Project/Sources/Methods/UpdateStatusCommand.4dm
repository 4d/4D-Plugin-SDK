//%attributes = {}
$pos:=Selected list items:C379(hlist)
If ($pos>0)
	GET LIST ITEM:C378(hlist;$pos;$refelem;$name)
	
	$themeID:=$refelem\10000
	$commandeID:=$refelem%10000
	If ($themeID>0)
		OBJECT SET ENABLED:C1123(*;"Bouton2";True:C214)
	Else 
		OBJECT SET ENABLED:C1123(*;"Bouton2";False:C215)
	End if 
Else 
	OBJECT SET ENABLED:C1123(*;"Bouton2";False:C215)
End if 