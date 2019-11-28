//%attributes = {}
C_LONGINT:C283($1)  // 0x00RRGGBB
C_TEXT:C284($r;$g;$b)  //red, green, blue
  //
$r:=String:C10(((($1 & 0x00FF0000) >> 16)/255)*255)
$g:=String:C10(((($1 & 0xFF00) >> 8)/255)*255)
$b:=String:C10((($1 & 0x00FF)/255)*255)
$0:="rgb("+$r+","+$g+","+$b+")"