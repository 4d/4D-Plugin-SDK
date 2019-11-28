//%attributes = {}
SET MENU BAR:C67(1;*)
$ref:=Open form window:C675([Projects:2];"Input";Plain window:K34:13+On the left:K39:2+At the top:K39:5)
ADD RECORD:C56([Projects:2];*)
UNLOAD RECORD:C212([Projects:2])
CLOSE WINDOW:C154
POST OUTSIDE CALL:C329(<>ProcessPaletteProjet)
