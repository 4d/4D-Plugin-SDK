//%attributes = {}
C_LONGINT:C283($1;$2;$3)
C_LONGINT:C283($0)

C_LONGINT:C283($red;$green;$blue)
$red:=Int:C8(($1/65535)*255)
$green:=Int:C8(($2/65535)*255)
$blue:=Int:C8(($3/65535)*255)

$0:=($red << 16) | ($green << 8) | ($blue)