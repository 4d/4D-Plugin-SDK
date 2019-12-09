//%attributes = {}
C_LONGINT:C283($1)
SET MENU BAR:C67(1;*)
$ref:=Open form window:C675([Projects:2];"Input";Plain window:K34:13+On the left:K39:2+At the top:K39:5)
QUERY:C277([Projects:2];[Projects:2]ID:1=$1)
MODIFY RECORD:C57([Projects:2];*)
CLOSE WINDOW:C154
POST OUTSIDE CALL:C329(<>ProcessPaletteProjet)
