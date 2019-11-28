//%attributes = {}
_O_ARRAY STRING:C218(32;$arNameArgs;0)
_O_ARRAY STRING:C218(32;$arTypeArgs;0)
_O_ARRAY STRING:C218(32;$arInOutArgs;0)
_O_ARRAY STRING:C218(32;$arConstants;0)
_O_C_STRING:C293(32;$returnValue)

$offset:=0
BLOB TO VARIABLE:C533($2;$arNameArgs;$offset)
BLOB TO VARIABLE:C533($2;$arTypeArgs;$offset)
BLOB TO VARIABLE:C533($2;$arInOutArgs;$offset)
BLOB TO VARIABLE:C533($2;$arConstants;$offset)
BLOB TO VARIABLE:C533($2;$returnValue;$offset)

$3->:=$3->+ColorizeText ("void ";gKeyWordColor)
$3->:=$3->+($1+"( PA_PluginParameters params )"+vcr+"{"+vcr)

$4->:=$4->+ColorizeText ("void ";gKeyWordColor)
$4->:=$4->+$1+"( PA_PluginParameters params );"+vcr


  // Declaration of variables
For ($i;1;Size of array:C274($arNameArgs))
	$3->:=$3->+BuildDeclareVariable ($arTypeArgs{$i};NameFilter ($arNameArgs{$i};True:C214;False:C215;False:C215);True:C214)
End for 

If ($returnValue#"None")
	$3->:=$3->+BuildDeclareVariable ($returnValue;"returnValue";False:C215)
End if 

If (($returnValue#"None") | (Size of array:C274($arNameArgs)>0))
	$3->:=put ($3->)
End if 

  // read parameters in input    
$output:=False:C215
For ($i;1;Size of array:C274($arNameArgs))
	$output:=True:C214
	$type:=$arTypeArgs{$i}
	$name:=NameFilter ($arNameArgs{$i};True:C214;False:C215;False:C215)
	$num:=String:C10($i)
	
	If ($type="Text")
		$3->:=$3->+put (vtab+$name+" = PA_GetStringParameter( params, "+$num+" );")
	End if 
	If (True:C214)
		If ($arInOutArgs{$i}#"input/output")
			Case of 
				: ($type="Date")
					$3->:=$3->+put (vtab+"PA_GetDateParameter( params, "+$num+", &amp;"+$name+".fDay, &amp;"+$name+".fMonth, &amp;"+$name+".fYear );")
				: ($type="Hour")
					$3->:=$3->+put (vtab+$name+" = PA_GetTimeParameter( params, "+$num+" );")
				: ($type="Integer")
					$3->:=$3->+put (vtab+$name+" = PA_GetShortParameter( params, "+$num+" );")
				: ($type="Long")
					$3->:=$3->+put (vtab+$name+" = PA_GetLongParameter( params, "+$num+" );")
				: ($type="Blob")
					$3->:=$3->+put (vtab+$name+" = PA_GetBlobHandleParameter( params, "+$num+" );")
				: ($type="Picture")
					$3->:=$3->+put (vtab+$name+" = PA_GetPictureParameter( params, "+$num+" );")
				: ($type="Real")
					$3->:=$3->+put (vtab+$name+" = PA_GetDoubleParameter( params, "+$num+" );")
				: ($type="Object")
					$3->:=$3->+put (vtab+$name+" = PA_GetObjectParameter( params, "+$num+" );")
				: ($type="Collection")
					$3->:=$3->+put (vtab+$name+" = PA_GetCollectionParameter( params, "+$num+" );")
				: ($type="Text")
					$3->:=$3->+put (vtab+$name+"_uchars = PA_GetUnistring( "+$name+" );")
				: ($type="Array")
					$3->:=$3->+put (vtab+$name+" = PA_GetVariableParameter( params, "+$num+" );")
				: ($type="Pointer")
					$3->:=$3->+put (vtab+$name+" = PA_GetPointerParameter( params, "+$num+" );")
			End case 
		End if 
	End if 
End for 

If ($output)
	$3->:=$3->+put ("")
End if 


$3->:=$3->+ColorizeText (vtab+"// --- write the code of "+$1+" here...";gCommentColor)+vcr
  // parametres en sortie
For ($i;1;Size of array:C274($arNameArgs))
	If (($arInOutArgs{$i}="output") | ($arInOutArgs{$i}="input/output"))
		$type:=$arTypeArgs{$i}
		$name:=NameFilter ($arNameArgs{$i};True:C214;False:C215;False:C215)
		$num:=String:C10($i)
		Case of 
			: ($type="Date")
				$3->:=$3->+put (vtab+"PA_SetDateParameter( params, "+$num+", "+$name+".fDay, "+$name+".fMonth, "+$name+".fYear );")
			: ($type="Hour")
				$3->:=$3->+put (vtab+"PA_SetTimeParameter( params, "+$num+", "+$name+" );")
			: ($type="Integer")
				$3->:=$3->+put (vtab+"PA_SetShortParameter( params, "+$num+", "+$name+" );")
			: ($type="Long")
				$3->:=$3->+put (vtab+"PA_SetLongParameter( params, "+$num+", "+$name+" );")
			: ($type="Blob")
				$3->:=$3->+put (vtab+"PA_SetBlobHandleParameter( params, "+$num+", "+$name+" );")
			: ($type="Picture")
				$3->:=$3->+put (vtab+"PA_SetPictureParameter( params, "+$num+", "+$name+" );")
			: ($type="Real")
				$3->:=$3->+put (vtab+"PA_SetDoubleParameter( params, "+$num+", "+$name+" );")
			: ($type="Text")
				$3->:=$3->+put (vtab+"PA_SetUnistring( "+$name+", "+$name+"_uchars );")
			: ($type="Array")
				$3->:=$3->+put (vtab+"PA_SetVariableParameter( params, "+$num+", "+$name+", 1 );")
		End case 
	End if 
End for 

  // return value
Case of 
	: ($returnValue="Date")
		$3->:=$3->+(vtab+"PA_ReturnDate( params, returnValue.fDay, ")
		$3->:=$3->+put ("returnValue.fMonth, returnValue.fYear );"+vcr)
	: ($returnValue="Hour")
		$3->:=$3->+put (vtab+"PA_ReturnTime( params, returnValue );")
	: ($returnValue="Integer")
		$3->:=$3->+put (vtab+"PA_ReturnShort( params, returnValue );")
	: ($returnValue="Long")
		$3->:=$3->+put (vtab+"PA_ReturnLong( params, returnValue );")
	: ($returnValue="Blob")
		$3->:=$3->+put (vtab+"PA_ReturnBlobHandle( params, returnValue );")
	: ($returnValue="Object")
		$3->:=$3->+put (vtab+"PA_ReturnObject( params, returnValue );")
	: ($returnValue="Collection")
		$3->:=$3->+put (vtab+"PA_ReturnCollection( params, returnValue );")
	: ($returnValue="Picture")
		$3->:=$3->+put (vtab+"PA_ReturnPicture( params, returnValue );")
	: ($returnValue="Real")
		$3->:=$3->+put (vtab+"PA_ReturnDouble( params, returnValue );")
	: ($returnValue="Text")
		$3->:=$3->+put (vtab+"PA_ReturnString( params, returnValue_uchars );")
End case 

$3->:=$3->+put ("}"+vcr)
