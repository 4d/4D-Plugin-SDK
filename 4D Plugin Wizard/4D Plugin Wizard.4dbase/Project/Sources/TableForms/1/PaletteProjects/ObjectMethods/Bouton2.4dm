CONFIRM:C162("Are you sure?")
If (ok=1)
	$numproc:=Process number:C372("Project"+String:C10([Projects:2]ID:1))
	If ($numproc>0)
		<>killProcess:=[Projects:2]ID:1
		POST OUTSIDE CALL:C329($numproc)
	Else 
		READ WRITE:C146([Projects:2])
		DELETE RECORD:C58([Projects:2])
		READ ONLY:C145([Projects:2])
		ALL RECORDS:C47([Projects:2])
	End if 
End if 