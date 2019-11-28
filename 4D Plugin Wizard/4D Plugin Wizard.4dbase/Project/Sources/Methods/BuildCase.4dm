//%attributes = {}
C_TEXT:C284($content)
$content:=ColorizeText (vtab+vtab+"case";gKeyWordColor)
$content:=put ($content+" "+$1+" :")
$content:=put ($content+vtab+vtab+vtab+$2+";")
$content:=$content+ColorizeText (vtab+vtab+vtab+"break;";gKeyWordColor)+vcr
$content:=put ($content)

$0:=$content