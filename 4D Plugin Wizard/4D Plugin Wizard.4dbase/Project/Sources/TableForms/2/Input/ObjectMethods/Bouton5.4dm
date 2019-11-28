C_TEXT:C284($endOf)
C_TEXT:C284($ref)
$ref:=Select folder:C670("Please select a component.4dbase folder";"";2)
$endOf:=Substring:C12($ref;Length:C16($ref)-7;7)
If ((OK=1) & ($endOf=".4dbase"))
	[Projects:2]DialogPath:9:=$ref
Else 
	ALERT:C41("you didn't select a .4dbase folder")
End if 
