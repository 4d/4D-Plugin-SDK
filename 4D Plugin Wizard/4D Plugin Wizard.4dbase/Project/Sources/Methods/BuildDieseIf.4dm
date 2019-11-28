//%attributes = {}
C_TEXT:C284($1)
C_TEXT:C284($content)
$content:=ColorizeText ("#if ";gKeyWordColor)
$content:=$content+ColorizeText ($1+vcr;"000000")
$0:=$content