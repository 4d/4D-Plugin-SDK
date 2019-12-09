//%attributes = {}
  // _PointInRect 

_O_C_INTEGER:C282($1;$2;$3;$4;$5;$6)
  // x, y, left-top-right-bottom ("Standard" rect in 4D)

C_BOOLEAN:C305($0)

$0:=(($1>$3) & ($1<$5) & ($2>$4) & ($2<$6))