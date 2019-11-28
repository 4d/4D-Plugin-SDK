
Case of 
	: (Form event code:C388=On Outside Call:K2:11)
		If ([Projects:2]ID:1=<>killProcess)
			CANCEL TRANSACTION:C241
			CLEAR LIST:C377(hlist;*)
			CANCEL:C270
			DELETE RECORD:C58([Projects:2])
			POST OUTSIDE CALL:C329(<>ProcessPaletteProjet)
		End if 
	: (Form event code:C388=On Load:K2:1)
		RealTimeChange:=True:C214
		
		If (Is new record:C668([Projects:2]))
			[Projects:2]ID:1:=Sequence number:C244([Projects:2])
			[Projects:2]Name:2:="new Project"
			[Projects:2]exportXCodeProject:10:=True:C214
			[Projects:2]exportVCproject2008:25:=True:C214
			[Projects:2]exportSource:12:=True:C214
			[Projects:2]exportResources:13:=True:C214
			[Projects:2]exportPluginAPI:14:=True:C214
			[Projects:2]exportSample:24:=True:C214
			[Projects:2]lastCommandNumber:15:=1
			[Projects:2]lastThemeNumber:16:=1
			[Projects:2]lastExternalAreaNumber:17:=1
			[Projects:2]lastGroupNumber:18:=1
			[Projects:2]lastConstantNumber:19:=1
			[Projects:2]LiveUpdate:21:=True:C214
			[Projects:2]PluginID:23:=15000
		End if 
		SET WINDOW TITLE:C213("Project : "+[Projects:2]Name:2)
		OBJECT SET ENABLED:C1123(*;"Bouton2";False:C215)  //Disable add command per default at start
		
		buildHList 
		buildHList2 
		BuildSource 
		
		START TRANSACTION:C239
		SAVE RECORD:C53([Projects:2])
	: (Form event code:C388=On Close Box:K2:21)
		answer:=0
		$f:=Open form window:C675([Interface:1];"SaveCancel";5)
		DIALOG:C40([Interface:1];"SaveCancel")
		CLOSE WINDOW:C154
		If (answer=1)
			CANCEL TRANSACTION:C241
			CLEAR LIST:C377(hlist;*)
			hlist:=0
			CANCEL:C270
		End if 
		If (answer=2)
			VALIDATE TRANSACTION:C240
			CLEAR LIST:C377(hlist;*)
			hlist:=0
			ACCEPT:C269
		End if 
End case 
