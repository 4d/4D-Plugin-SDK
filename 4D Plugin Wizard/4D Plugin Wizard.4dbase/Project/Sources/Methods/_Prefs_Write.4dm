//%attributes = {}
  // _Prefs_Write
ALL RECORDS:C47([Prefs:8])
If (Records in selection:C76([Prefs:8])=0)
	CREATE RECORD:C68([Prefs:8])
End if 
  // no "Record in use" test in this sample app.

[Prefs:8]KeyWordColor:1:=<>gPref_KeyWordColor
[Prefs:8]CommentColor:2:=<>gPref_CommentColor
[Prefs:8]TextColor:3:=<>gPref_TextColor
[Prefs:8]FontName:4:=<>gPref_FontName
[Prefs:8]FontSize:5:=<>gPref_FontSize
  //
SAVE RECORD:C53([Prefs:8])
UNLOAD RECORD:C212([Prefs:8])