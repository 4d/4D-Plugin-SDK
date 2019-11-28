//%attributes = {}
  // _Prefs_ChangeColor 
_O_C_STRING:C293(255;$1)
C_POINTER:C301($2)

currentColor:=$2->
a:=Open form window:C675([Prefs:8];"ChooseColor";Movable dialog box:K34:7;Horizontally centered:K39:1;Vertically centered:K39:4)
DIALOG:C40([Prefs:8];"ChooseColor")
CLOSE WINDOW:C154
If (ok=1)
	$2->:=_RGBToLong (redThermo;greenThermo;blueThermo)
	OBJECT SET RGB COLORS:C628(*;$1;0;$2->)
End if 