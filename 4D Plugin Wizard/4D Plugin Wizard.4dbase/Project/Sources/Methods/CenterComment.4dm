//%attributes = {}
  // $1: area1
  // $2: area2

C_POINTER:C301($1)
C_POINTER:C301($2)

$demilen:=Length:C16($3)\2
$1->:=put ($1->+ColorizeText ("// "+("-"*(39-$demilen))+" "+$3+" "+("-"*(39-Length:C16($3)+$demilen));gCommentColor))
$2->:=$2->+vcr+ColorizeText ("// --- "+$3;gCommentColor)+vcr
