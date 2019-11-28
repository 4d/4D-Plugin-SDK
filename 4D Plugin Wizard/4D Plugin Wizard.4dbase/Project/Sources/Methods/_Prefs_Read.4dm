//%attributes = {}
  // _Prefs_Read 

ALL RECORDS:C47([Prefs:8])
If (Records in selection:C76([Prefs:8])=0)
	CREATE RECORD:C68([Prefs:8])
	[Prefs:8]KeyWordColor:1:=0x00FF  // default : Blue
	
	[Prefs:8]CommentColor:2:=0x7F00  // default : half green
	
	[Prefs:8]TextColor:3:=0x0000  // default : black
	
	
	_O_PLATFORM PROPERTIES:C365($plateform)
	If ($plateform=2)
		[Prefs:8]FontName:4:="monaco"
		[Prefs:8]FontSize:5:=9
	Else 
		[Prefs:8]FontName:4:="Courier New"
		[Prefs:8]FontSize:5:=10
	End if 
	SAVE RECORD:C53([Prefs:8])
End if 


<>gPref_KeyWordColor:=[Prefs:8]KeyWordColor:1
<>gPref_CommentColor:=[Prefs:8]CommentColor:2
<>gPref_TextColor:=[Prefs:8]TextColor:3
<>gPref_FontName:=[Prefs:8]FontName:4
<>gPref_FontSize:=[Prefs:8]FontSize:5


UNLOAD RECORD:C212([Prefs:8])