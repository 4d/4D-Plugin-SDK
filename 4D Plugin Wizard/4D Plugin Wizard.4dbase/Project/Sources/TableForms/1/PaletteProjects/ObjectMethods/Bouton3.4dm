  //Si aucun projet n'est selectionner
If ([Projects:2]ID:1=0)
	
	  //Et qu'il n'y a qu'un seul enregistrement on se positionne dessus
	If (Records in selection:C76([Projects:2])=1)
		FIRST RECORD:C50([Projects:2])
	Else 
		ALERT:C41("Please select a project.")
	End if 
End if 

  //Si un projet est selectionner
If ([Projects:2]ID:1#0)
	$i:=New process:C317("EditProject";0;"Project"+String:C10([Projects:2]ID:1);[Projects:2]ID:1;*)
	If ($i#0)
		BRING TO FRONT:C326($i)
	End if 
End if 