//%attributes = {}
  // $1 : ref to dest resource file
  // $2 : full path of dialog resource file

$ref:=Open resource file:C497($2)
If ($ref>=0)
	RESOURCE TYPE LIST:C499($resType;$ref)
	For ($i;1;Size of array:C274($resType))
		RESOURCE LIST:C500($resType{$i};$resID;$resName;$ref)
		For ($j;1;Size of array:C274($resID))
			$type:=$resType{$i}
			$id:=$resID{$j}
			  // read resource      
			GET RESOURCE:C508($type;$id;$blob;$ref)
			$att:=Get resource properties:C515($type;$id;$ref)
			  // write resource
			_O_SET RESOURCE:C509($type;$id;$blob;$1)
			_O_SET RESOURCE NAME:C514($type;$id;$resName{$j};$1)
			_O_SET RESOURCE PROPERTIES:C516($type;$id;$att;$1)
		End for 
	End for 
End if 

CLOSE RESOURCE FILE:C498($ref)