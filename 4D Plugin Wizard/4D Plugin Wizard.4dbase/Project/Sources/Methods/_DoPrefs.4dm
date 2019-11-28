//%attributes = {}
  // _DoPrefs 
_Prefs_Read 
a:=Open form window:C675([Prefs:8];"DoPrefsDialog";Movable dialog box:K34:7;Horizontally centered:K39:1;Vertically centered:K39:4)
DIALOG:C40([Prefs:8];"DoPrefsDialog")
CLOSE WINDOW:C154
If (OK=1)
	If (<>popFontName>0)
		<>gPref_FontName:=<>popFontName{<>popFontName}
	End if 
	If (<>popFontSize>0)
		<>gPref_FontSize:=<>popFontSize{<>popFontSize}
	End if 
	_Prefs_Write 
End if 

ARRAY INTEGER:C220(<>popFontSize;0)
_O_ARRAY STRING:C218(255;<>popFontName;0)