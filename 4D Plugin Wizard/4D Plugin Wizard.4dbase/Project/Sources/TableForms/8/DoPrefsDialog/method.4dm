If (Form event code:C388=On Load:K2:1)
	OBJECT SET RGB COLORS:C628(*;"oKeyWords";0;<>gPref_KeyWordColor)
	OBJECT SET RGB COLORS:C628(*;"oComments";0;<>gPref_CommentColor)
	OBJECT SET RGB COLORS:C628(*;"oText";0;<>gPref_TextColor)
	
	
	ARRAY INTEGER:C220(<>popFontSize;3)
	<>popFontSize{1}:=9
	<>popFontSize{2}:=10
	<>popFontSize{3}:=12
	
	$cdt:=Find in array:C230(<>popFontSize;<>gPref_FontSize)
	If ($cdt>0)
		<>popFontSize:=$cdt
	Else 
		<>gPref_FontSize:=12
		<>popFontSize:=3
	End if 
	
	$cdt:=Find in array:C230(<>popFontName;<>gPref_FontName)
	If ($cdt>0)
		<>popFontName:=$cdt
	Else 
		<>gPref_FontName:="Courier"
		<>popFontName:=Find in array:C230(<>popFontName;<>gPref_FontName)
	End if 
End if 