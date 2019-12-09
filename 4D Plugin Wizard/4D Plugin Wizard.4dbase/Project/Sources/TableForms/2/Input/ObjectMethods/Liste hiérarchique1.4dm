
Case of 
	: (Form event code:C388=On Selection Change:K2:29)
		UpdateStatusCommand 
	: (Form event code:C388=On Clicked:K2:4)
		UpdateStatusCommand 
	: (Form event code:C388=On Double Clicked:K2:5)
		$pos:=Selected list items:C379(hlist)
		If ($pos>0)
			GET LIST ITEM:C378(hlist;$pos;$refelem;$name)
			$themeID:=$refelem\10000
			$commandeID:=$refelem%10000
			If ($commandeID>0)
				QUERY:C277([Commands:4];[Commands:4]ID:1=$commandeID;*)
				QUERY:C277([Commands:4]; & ;[Commands:4]Theme:2=$themeID)
				$f:=Open form window:C675([Commands:4];"Input";5;Horizontally centered:K39:1;Vertically centered:K39:4)
				MODIFY RECORD:C57([Commands:4];*)
				CLOSE WINDOW:C154
				If (ok=1)
					$name:=[Commands:4]Name:3
					SET LIST ITEM:C385(hlist;$refelem;$name;$refelem)
					_O_REDRAW LIST:C382(hlist)
					BuildSource 
				End if 
				UNLOAD RECORD:C212([Commands:4])
			End if 
			If ($refelem<0)
				QUERY:C277([ExternalAreas:5];[ExternalAreas:5]ID:1=-$refelem;*)
				QUERY:C277([ExternalAreas:5]; & ;[ExternalAreas:5]Project:2=[Projects:2]ID:1)
				$f:=Open form window:C675([ExternalAreas:5];"Input";5;Horizontally centered:K39:1;Vertically centered:K39:4)
				MODIFY RECORD:C57([ExternalAreas:5];*)
				CLOSE WINDOW:C154
				If (ok=1)
					$name:=[ExternalAreas:5]Name:3
					SET LIST ITEM:C385(hlist;$refelem;$name;$refelem)
					_O_REDRAW LIST:C382(hlist)
					BuildSource 
				End if 
				UNLOAD RECORD:C212([ExternalAreas:5])
			End if 
		End if 
		
	: (Form event code:C388=On Data Change:K2:15)
		If (Is a list:C621(hlist))
			$pos:=Selected list items:C379(hlist)
			If ($pos>0)
				GET LIST ITEM:C378(hlist;$pos;$refelem;$name)
				If (Length:C16($name)>31)
					$name:=Substring:C12($name;1;31)
					SET LIST ITEM:C385(hlist;$refelem;$name;$refelem)
					_O_REDRAW LIST:C382(hlist)
				End if 
				If ($refelem<0)
					QUERY:C277([ExternalAreas:5];[ExternalAreas:5]ID:1=-$refelem;*)
					QUERY:C277([ExternalAreas:5]; & ;[ExternalAreas:5]Project:2=[Projects:2]ID:1)
					[ExternalAreas:5]Name:3:=$name
					SAVE RECORD:C53([ExternalAreas:5])
					UNLOAD RECORD:C212([ExternalAreas:5])
				Else 
					$themeID:=$refelem\10000
					$commandeID:=$refelem%10000
					If ($commandeID=0)
						QUERY:C277([Themes:3];[Themes:3]ID:1=$themeID;*)
						QUERY:C277([Themes:3]; & ;[Themes:3]Project:2=[Projects:2]ID:1)
						[Themes:3]Name:3:=$name
						SAVE RECORD:C53([Themes:3])
						UNLOAD RECORD:C212([Themes:3])
					Else 
						QUERY:C277([Commands:4];[Commands:4]ID:1=$commandeID;*)
						QUERY:C277([Commands:4]; & ;[Commands:4]Theme:2=$themeID)
						[Commands:4]Name:3:=$name
						SAVE RECORD:C53([Commands:4])
						UNLOAD RECORD:C212([Commands:4])
					End if 
				End if 
				BuildSource 
			End if 
		End if 
		
	: (Form event code:C388=On Begin Drag Over:K2:44)
		C_LONGINT:C283($IdTheme;$IdCmd;$refelem)
		$refelem:=Selected list items:C379(hlist;*)
		If ($refelem#0)
			$IdTheme:=$refelem\10000
			$IdCmd:=$refelem%10000
			If ($IdCmd>0)
				C_BLOB:C604($Ref)
				
				CLEAR PASTEBOARD:C402  //Del Conteneur
				
				  //Ajout des identifiant au blob
				LONGINT TO BLOB:C550($IdCmd;$Ref;Native byte ordering:K22:1)
				LONGINT TO BLOB:C550($IdTheme;$Ref;Native byte ordering:K22:1;*)
				
				  // Ajout du blob dans le presse papier avec un type propre
				APPEND DATA TO PASTEBOARD:C403("Cmd_Plugin";$Ref)
			End if 
		End if 
		
	: (Form event code:C388=On Drag Over:K2:13)
		  // Test si le conteneur est celui de la liste 
		If (Pasteboard data size:C400("Cmd_Plugin")>0)
			$0:=0  //C'est bien notre type donc on laisse actif le drag
		Else 
			$0:=-1  //Sinon on descative le drag
		End if 
		
	: (Form event code:C388=On Drop:K2:12)
		  //Declaration des variables
		C_BLOB:C604($Ref)
		C_LONGINT:C283($IdTheme;$IdCmd)
		C_LONGINT:C283($offset)
		C_LONGINT:C283($Pos;$Col)
		  // ré"cupérer la ref de l'élément et de son parent
		  // tester POSITION DEPOSER
		
		  //On recupere notre blob
		GET PASTEBOARD DATA:C401("Cmd_Plugin";$Ref)
		
		  //On recupere les indices dans le blob
		$offset:=0
		$IdCmd:=BLOB to longint:C551($Ref;Native byte ordering:K22:1;$offset)
		$IdTheme:=BLOB to longint:C551($Ref;Native byte ordering:K22:1;$offset)
		
		$Pos:=Drop position:C608
		If ($Pos>0)
			C_LONGINT:C283($IdTheme_Dest;$IdCmd_Dest)
			GET LIST ITEM:C378(hlist;$Pos;$refelem;$name)
			$IdTheme_Dest:=$refelem\10000
			
			If ($IdTheme#$IdTheme_Dest)
				QUERY:C277([Commands:4];[Commands:4]ID:1=$IdCmd;*)
				QUERY:C277([Commands:4]; & ;[Commands:4]Theme:2=$IdTheme)
				If (Records in selection:C76([Commands:4])=1)
					If (Locked:C147([Commands:4]))
						ALERT:C41("Erreur : L'enregistrement est verrouillé")
					Else 
						[Commands:4]Theme:2:=$IdTheme_Dest
						SAVE RECORD:C53([Commands:4])
						UNLOAD RECORD:C212([Commands:4])
						buildHList 
						BuildSource 
						SELECT LIST ITEMS BY REFERENCE:C630(hlist;($IdTheme_Dest*10000)+$IdCmd)
					End if 
				Else 
					ALERT:C41("Erreur : Aucunes ou Plusieurs commandes correspondent à votre requête")
				End if 
			End if 
		End if 
End case 